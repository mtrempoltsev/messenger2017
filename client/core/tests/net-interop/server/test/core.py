import datetime
import logging
import unittest

import os

import sys

from kily.utils.io import ensure_path
from kily.system.core import ModuleStatus, Module, SystemDelegate
from kily.system.system import System
from m2.config import DefaultConfig

os.chdir(os.path.dirname(os.path.abspath(__file__)))


class TestConfig(DefaultConfig):
    CONFIG_PATH = "test-configs/app.json"
    LOGS_PATH = "logs"

class TestCaseWithSystem(unittest.TestCase):
    TEST_NAME = "test-system"
    MODULES_TO_LOAD = []

    def __int__(self):
        self.system = None
        self.stream_handler = None

    def setUp(self):
        self.stream_handler = None
        self.system = System(TestConfig, *self.MODULES_TO_LOAD, logger=self.makeTestLogger(f"{self.TEST_NAME}-logger"))

    def makeTestLogger(self, name):
        logger = logging.getLogger(name)
        self.stream_handler = logging.StreamHandler(sys.stdout)
        # create formatter and add it to the handlers
        formatter = logging.Formatter('[%(asctime)s] [%(levelname)s] [%(name)s] %(message)s')
        self.stream_handler.setLevel(logging.DEBUG)
        self.stream_handler.setFormatter(formatter)
        logger.addHandler(self.stream_handler)
        logger.setLevel(logging.DEBUG)
        ensure_path(TestConfig.LOGS_PATH)
        file_handler = logging.FileHandler(
            f"{TestConfig.LOGS_PATH}/test-logs.{datetime.datetime.now().strftime('%d.%m.%y')}.log", mode='w')
        file_handler.setFormatter(formatter)
        logger.addHandler(file_handler)
        return logger


class SimpleModule(Module):
    def load(self, system: 'SystemDelegate', **deps: 'Module') -> ModuleStatus:
        return ModuleStatus.LOADED
