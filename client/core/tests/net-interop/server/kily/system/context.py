import json
from typing import Dict, Any

from copy import copy

import kily.utils.io as uio
from kily.system.config import ModuleNames
from kily.system.core import Module, SystemDelegate, ModuleStatus


class AppContext(Module):
    NAME = ModuleNames.AppContextModule

    def __init__(self, load_fs_context: bool = True, local_context: dict = None):
        super().__init__()
        self._target_name = None  # type: str
        self._target_path_mask = None  # type: str
        self._params = {}  # type: Dict[str, Any]
        self._named_contexts = {}

        self.__local_context = local_context
        self.__load_fs_context = load_fs_context
        if not self.__load_fs_context and (self.__local_context is None or not len(self.__local_context)):
            raise ValueError("Cannot have LocalContext as None with FS context disabled")

    def load(self, system: 'SystemDelegate', **deps: 'Module') -> ModuleStatus:
        self._system = system
        self._status = ModuleStatus.LOADED

        self.load_context_json(uio.readf(self._system.sets.CONFIG_PATH))
        return self._status

    def load_context_json(self, json_context: bytes):
        json_context = json_context.decode('utf-8')
        self.load_context(json.loads(json_context))

    def load_context(self, context: dict):
        self._params = copy(context)

    def load_context_named(self, name: str, context: dict):
        self._named_contexts[name] = context

    def load_context_named_json(self, name: str, json_context: bytes):
        json_context = json_context.decode('utf-8')
        json_context = json.loads(json_context)
        self.load_context_named(name, json_context)

    def get_named(self, name: str) -> dict:
        return self._named_contexts[name]

    def get(self, name: str, default: Any = None):
        return self._params.get(name, default)

    @property
    def params(self):
        return self._params
