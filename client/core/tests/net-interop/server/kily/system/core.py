from typing import List, Dict, Sequence, Optional, Any, Callable
from abc import ABC, abstractmethod, abstractproperty

import logging
import enum


class ModuleStatus(enum.Enum):
    NOT_LOADED = 0x00
    ERROR = 0xFF
    LOADED = 0x01


class ModuleIntegrity(enum.Enum):
    CORE_MNR = 0x01
    CORE_CRT = 0x08

    PLUGIN_CRT = 0x11
    PLUGIN_MNR = 0x18


class ModuleLoadPriority(enum.Enum):
    DEFAULT = 0x00
    PRE     = 0x01
    POST    = 0x01


class DuplicateModuleError(ValueError):
    def __init__(self, mod):
        assert isinstance(mod, Module)
        super().__init__("Duplicate module: %s" % mod.NAME)


class SystemDelegate:
    def __init__(self, system, logger):
        assert isinstance(logger, logging.Logger)
        self._status = ModuleStatus.NOT_LOADED
        self._system = system

    def modules_by_type(self, type_name: str) -> List['Module']:
        return self._system._types_to_modules.get(type_name, [])

    @property
    def logger(self) -> logging.Logger:
        return self._system._logger

    @property
    def modules(self) -> Dict[str, 'Module']:
        return self._system._modules

    @property
    def module_types(self) -> Dict[str, List['Module']]:
        return self._system._types_to_modules

    @property
    def status(self) -> 'ModuleStatus':
        return self._status

    @property
    def sets(self) -> Any:
        return self._system.sets

    def get_event_loop(self, another_thread: bool = False, run_pre: Callable[[Any], None] = None):
        return self._system.get_event_loop(another_thread=another_thread, run_pre=run_pre)

    def push_to_thread(self, func):
        return self._system.push_to_thread(func=func)


class Module(ABC):
    # default values - override if needed
    DEPENDENCIES = []  # type: List[str]
    NAME = None  # type: str
    TYPE = None  # type: Optional[str]
    INTEGRITY = ModuleIntegrity.CORE_CRT  # type: ModuleIntegrity
    LOAD_PRIORITY = ModuleLoadPriority.DEFAULT  # type: ModuleLoadPriority

    def __init__(self):
        self._status = ModuleStatus.NOT_LOADED
        self._system = None  # type: SystemDelegate

    @classmethod
    def ModuleName(cls) -> str:
        return cls.NAME

    @classmethod
    def ModuleDependencies(cls) -> List[str]:
        return cls.DEPENDENCIES

    @classmethod
    def ModuleLoadPriority(cls) -> ModuleLoadPriority:
        return cls.LOAD_PRIORITY

    @classmethod
    def ModuleIntegrity(cls) -> ModuleIntegrity:
        return cls.INTEGRITY

    @abstractmethod
    def load(self, system: 'SystemDelegate', **deps: 'Module') -> ModuleStatus:
        raise NotImplementedError()

    @property
    def status(self) -> ModuleStatus:
        return self._status


class RunnableModule(Module):
    RUN_AFTER = []
    RUN_AFTER_DEPENDENCIES = True

    @abstractmethod
    def run(self, **kwargs) -> None:
        """
        Perform a 'run' action on a module. Module should have status LOADED
        for a System to actually call this method.
        :param kwargs: additional arguments. Module mustn't make assumptions on
        the contents of this dictionary.
        To have consistent configuration, module should use System's "sets" property
        or an ApplicationContext classq
        """
        raise NotImplementedError()

