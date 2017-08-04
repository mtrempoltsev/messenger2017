import unittest

import sys

from kily.system.context import AppContext
from kily.system.system import System
from m2.mock_srv import MockServer
from m2.routes.registration import RegistrationProcessModule
from test.core import TestCaseWithSystem


class ServerTestSmoke(TestCaseWithSystem):
    MODULES_TO_LOAD = [AppContext(), MockServer(), RegistrationProcessModule()]

    def test_smoke(self):
        self.system.logger.info("Loading classes...")
        self.system.load()
        self.system.logger.info("Running server...")
        self.system.run()
        self.system.logger.info("Run was successful, now wait for a server to close...")
        self.system.join()
