from abc import abstractmethod
from typing import Callable, Tuple, Iterable, Union, Coroutine, Any, Optional

from aiohttp import web

import kily.system.config
from kily.net.config import ModuleNames, ModuleSettingsKeys
from kily.net.helpers import HandlerHelper, HandlerEntry
from kily.system.context import AppContext
from kily.system.core import Module, SystemDelegate, RunnableModule, ModuleStatus

import aiohttp.web


class Server(RunnableModule):
    NAME = ModuleNames.ServerModule
    NAMED_RES_SYSTEM_KEY = "sys"
    DEPENDENCIES = [kily.system.config.ModuleNames.AppContextModule]
    LOCAL_ADDR = "127.0.0.1"

    def __init__(self):
        super().__init__()
        self._port = None  # type: int
        self._address = None  # type: str
        self._server = None  # type: aiohttp.web.Application
        self._context = None  # type: AppContext

    def load(self, system: 'SystemDelegate', **deps: 'Module'):
        self._context = deps[AppContext.NAME]  # type: AppContext
        self._port = self._context.get(ModuleSettingsKeys.SERVER_PORT)  # type: int
        self._address = self._context.get(ModuleSettingsKeys.SERVER_ADDR, self.LOCAL_ADDR)  # type: int
        self._system = system
        self._server = aiohttp.web.Application(logger=self._system.logger, secure_proxy_ssl_header="X-SSL-HEADER")
        for handler in self.get_handlers():
            if isinstance(handler, HandlerHelper):
                for um in handler.url_mapping():
                    self._server.router.add_route(um[0], um[1], um[2])
            else:
                self._server.router.add_route(handler[0], handler[1], handler[2])
        self._server[self.NAMED_RES_SYSTEM_KEY] = self._system
        self._status = ModuleStatus.LOADED
        return self._status

    @abstractmethod
    def get_handlers(self) -> Iterable[HandlerEntry]:
        raise NotImplementedError()

    def run(self, **kwargs) -> None:
        self._system.get_event_loop(another_thread=True,
                                    run_pre=lambda l:
                                    aiohttp.web.run_app(
                                        self._server, host=self._address, port=self._port, loop=l)
                                    )


