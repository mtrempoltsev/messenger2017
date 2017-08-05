import functools
import aiohttp_debugtoolbar

from typing import Dict, Tuple, Iterable
import uuid

from aiohttp import web

from kily.net.helpers import HandlerEntry
from kily.net.server import Server
from kily.system.core import SystemDelegate, Module, ModuleStatus
from m2.routes.registration import RegistrationProcessModule


class MockServer(Server):
    DEPENDENCIES = Server.DEPENDENCIES + [RegistrationProcessModule.NAME]

    def __init__(self):
        super().__init__()
        self._registration = None  # type: RegistrationProcessModule

    def load(self, system: 'SystemDelegate', **deps: 'Module') -> ModuleStatus:
        self._registration = deps[RegistrationProcessModule.NAME]
        super().load(system, **deps)
        self._context.load_context_named("db", {})  # make db in AppContext
        return self._status

    def get_handlers(self) -> Iterable[HandlerEntry]:
        return [
            self._registration
        ]

    def run(self, **kwargs) -> None:
        aiohttp_debugtoolbar.setup(self._server)
        super().run(**kwargs)


