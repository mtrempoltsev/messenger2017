import functools
import aiohttp_debugtoolbar

from typing import Dict, Tuple, Iterable
import uuid

from aiohttp import web

from kily.net.helpers import HandlerEntry
from kily.net.server import Server
from kily.system.core import SystemDelegate, Module
from m2.routes.registration import RegistrationProcessModule


class MockServer(Server):
    DEPENDENCIES = Server.DEPENDENCIES + [RegistrationProcessModule.NAME]

    def __init__(self):
        super().__init__()
        self._registration = None  # type: RegistrationProcessModule

    def load(self, system: 'SystemDelegate', **deps: 'Module'):
        super().load(system, **deps)
        self._registration = deps[RegistrationProcessModule.NAME]
        self._context.load_context_named("db", {})  # make db in AppContext

    def get_handlers(self) -> Iterable[HandlerEntry]:
        return [
            self._registration
        ]

    def run(self, **kwargs) -> None:
        aiohttp_debugtoolbar.setup(self._server)
        super().run(**kwargs)

    async def handle_register_intent(self, req: web.Request) -> web.Response:
        return web.Response(status=403, reason="GOVNO KOLYANA")

    async def handle_registration_challenge(self, req: web.Request) -> web.Response:
        return web.Response(status=403, reason="GOVNO KOLYANA")

    async def handle_register(self, req: web.Request) -> web.Response:
        return web.Response(status=403, reason="GOVNO KOLYANA")

