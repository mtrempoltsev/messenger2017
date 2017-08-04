from typing import Iterable
from aiohttp import web
import python_jsonschema_objects as pjs
from aiohttp.web_exceptions import *
from Crypto.PublicKey import RSA, ECC
from Crypto.Cipher import AES, PKCS1_OAEP
from Crypto.Protocol import KDF
from Crypto.Random import get_random_bytes
from kily.net.config import ModuleNames as NetModuleNames
from kily.net.helpers import HandlerHelper, StdHandlerEntry
from kily.system.config import ModuleNames
from kily.system.context import AppContext
from kily.system.core import Module, SystemDelegate, ModuleStatus


class RegistrationProcessModule(Module, HandlerHelper):
    DEPENDENCIES = [ModuleNames.AppContextModule]
    NAME = "kily::m2::RegistrationProcessModule"

    def __init__(self):
        super().__init__()
        self._context = None  # type: AppContext

    def load(self, system: SystemDelegate, **deps: Module):
        self._system = system
        self._context = deps[ModuleNames.AppContextModule]  # type: AppContext
        self._status = ModuleStatus.LOADED
        return self._status

    async def handle_registration_intent(self, req: web.Request) -> web.Response:
        params = await req.json()
        if "public_key" not in params:
            raise HTTPBadRequest(body={"error_msg": "'public_key' is obligatory"})
        else:
            key = RSA.import_key(params["public_key"])
            message = get_random_bytes(32)
            cipher_rsa = PKCS1_OAEP.new(key)
            secret_key = KDF.PBKDF2(self._context.get("SECRET_KEY"), "mysalt")
            cipher_aes = AES.new(secret_key, AES.MODE_EAX)
            return web.Response(body={
                "server_string": cipher_rsa.encrypt(),
                "client_string": cipher_aes.encrypt(message+key.encode('utf-8')),
            })

    async def handle_registration_confirmation(self, req: web.Request) -> web.Response:
        params = await req.json()
        raise HTTPException(body="not implemented :P")

    def url_mapping(self) -> Iterable['StdHandlerEntry']:
        return [
            ("POST", "/user/register/sendKey", self.handle_registration_intent),
            ("PUT", "/user/register", self.handle_registration_confirmation),
        ]

