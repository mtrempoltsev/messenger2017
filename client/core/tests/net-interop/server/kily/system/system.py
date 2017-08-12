import asyncio
import logging
import sys
from threading import Thread, current_thread, Lock
from traceback import format_exception
from typing import Dict, Deque, List, Any

from kily.system.core import SystemDelegate, DuplicateModuleError, RunnableModule
from kily.system.dependency import *


class System(SystemDelegate):
    def __init__(self, settings: Any, *modules, logger: logging.Logger):
        super().__init__(self, logger)
        if __debug__:
            for elem in modules:
                assert isinstance(elem, Module), "module must be of type Module"
        self._settings = settings
        self._logger = logger
        self._modules = {}  # type: Dict[str, Module]
        self._types_to_modules = {}  # type: Dict[str, List[Module]]
        self._threads = []  # type: List[Thread]
        self._lock = Lock()  # type: Lock
        for mdl in modules:
            self.register(mdl)

    def register(self, mod: Module, sync_status=True):
        if self._modules.get(mod.NAME):
            raise DuplicateModuleError(mod)
        else:
            if mod.TYPE is not None:
                if mod.TYPE not in self._types_to_modules:
                    self._types_to_modules[mod.TYPE] = [mod]
                else:
                    self._types_to_modules[mod.TYPE].append(mod)
            self.modules[mod.NAME] = mod
        if sync_status:
            if self._status.value >= ModuleStatus.LOADED.value:
                mod.load(self.delegate, **self.find_dependencies(mod))

    @property
    def delegate(self):
        return SystemDelegate(self, logger=self._logger)

    @staticmethod
    def _build_dependency_queue(*modules, mod_filter: Callable[[Module], bool] = None,
                                mod_deps: Callable[[Module], Iterable[str]] = None) -> Deque[Module]:
        dep = DependencyModuleTreeForest()
        dep.build(*modules, mod_filter=mod_filter, mod_deps=mod_deps)
        return dep.to_queue()

    def load(self):
        """
        Loads all registered modules with dependencies recursively
        """
        delegate = self.delegate
        load_queue = System._build_dependency_queue(*self._modules.values())  #
        for mod in load_queue:
            status = mod.load(delegate, **self.find_dependencies(mod))
            # TODO: if the status isn't LOADED - remove it from active modules and remove every module which is
            # dependent on it.
        self._status = ModuleStatus.LOADED

    def run(self, **kwargs):
        """
        Starts all registered and loaded modules
        :param kwargs: additional arguments to run
        """
        run_queue = System._build_dependency_queue(*self._modules.values(),
                                                   mod_filter=lambda x: isinstance(x, RunnableModule) and
                                                                        x.status == ModuleStatus.LOADED,
                                                   mod_deps=lambda x: self._run_dependencies(x))
        for mod in run_queue:
            mod.run(**kwargs)

    def _run_dependencies(self, mod: Module) -> Iterable[str]:
        assert isinstance(mod, RunnableModule)
        deps = list(mod.RUN_AFTER)
        if mod.RUN_AFTER_DEPENDENCIES:
            deps += mod.DEPENDENCIES
        return deps

    def find_dependencies(self, mod: Module):
        found = {}
        for ur in mod.DEPENDENCIES:
            found[ur] = self._modules[ur]
        return found

    # TODO add the ability to stop modules and unload them
    def unload(self):
        self._status = ModuleStatus.NOT_LOADED

    @property
    def sets(self) -> Any:
        return self._settings

    def thread_eventloop_fn(self, loop, run_pre: Callable[[Any], None]):
        # noinspection PyBroadException
        try:
            asyncio.set_event_loop(loop)
            if run_pre is not None:
                run_pre(loop)
            loop.run_forever()
        except Exception as e:
            exc_info, exc_val, tb, = sys.exc_info()
            self.logger.error(f"Exception in a thread ({current_thread().ident}): "
                              f"{''.join(format_exception(exc_info, exc_val, tb))}")

    def _mkthread(self, name, target, args: tuple = ()):
        self._lock.acquire()
        thr = Thread(
            name=name,
            target=target, args=args)
        self._threads.append(thr)
        self._lock.release()
        return thr

    def get_event_loop(self, another_thread: bool = False,
                       run_pre: Callable[[Any], None] = None):
        if not another_thread:
            return asyncio.get_event_loop()
        else:
            loop = asyncio.new_event_loop()
            thr = self._mkthread(name=f"ev-thread-{len(self._threads)}",
                                 target=self.thread_eventloop_fn,
                                 args=(loop, run_pre))

            thr.start()
            return loop

    def push_to_thread(self, func):
        thr = self._mkthread(name=f"wk-thread-{len(self._threads)}",
                             target=func)
        thr.start()

    def join(self, timeout_per_thread=None):
        i = 0
        # because threads can be added later
        while i < len(self._threads):
            self._threads[i].join(timeout=timeout_per_thread)
