import abc
import json
from typing import Tuple, List, Optional, Iterable, Union, Callable, Any, Coroutine

import aiohttp
from aiohttp import web
from aiohttp.web_exceptions import HTTPNotAcceptable, HTTPBadRequest

from kily.system.core import SystemDelegate





class AcceptChooser:
    def __init__(self):
        self._accepts = {}

    async def do_route(self, request: web.Request):
        for accept in request.headers.getall('ACCEPT', []):
            acceptor = self._accepts.get(accept)
            if acceptor is not None:
                return await acceptor(request)
        raise HTTPNotAcceptable()

    def reg_acceptor(self, accept, handler):
        self._accepts[accept] = handler


def combine(*url_packs: List['HandlerEntry']) -> List['HandlerEntry']:
    pack = []
    for url_pack in url_packs:
        pack += url_pack
    return pack


def accept_guard(request: web.Request, *accepts: str):
    for accept in request.headers.getall('ACCEPT', []):
        if accept in accepts:
            return
    raise HTTPNotAcceptable(text=f"Accepted types: {','.join(accepts)}")


class HandlerHelper(abc.ABC):
    OBLIGATORY_FIELDS = ["public_key"]

    def clean(self, req: dict) -> dict:
        for k in self.OBLIGATORY_FIELDS:
            if k not in req:
                raise HTTPBadRequest(text=f"Missing obligatory field {k}")
        return req

    def validate(self, req: str) -> Optional[dict]:
        req = json.loads(req)
        return self.clean(req)

    @abc.abstractmethod
    def url_mapping(self) -> Iterable['StdHandlerEntry']:
        raise NotImplementedError()


# TYPE DECLARATIONS

Handler = Union[
        Callable[[Optional[Any], aiohttp.web.Request], aiohttp.web.Response],
        Coroutine,
]
StdHandlerEntry = Tuple[str, str, Callable]
HandlerEntry = Union[HandlerHelper, StdHandlerEntry]