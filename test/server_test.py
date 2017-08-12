from typing import Tuple, Any

from Crypto.PublicKey import RSA, ECC
from Crypto.Cipher import AES, PKCS1_OAEP, PKCS1_v1_5 as PKCS1
from Crypto.Protocol import KDF
from Crypto.Random import get_random_bytes
import unittest
import requests
import base64
import hashlib
import uuid


class ClientTest(unittest.TestCase):
    SERVER_ADDR = "192.168.43.230"
    # 43.230
    SERVER_PORT = 8283

    """
    INSERT NEW UUID HERE FROM self.testRegistration()
    """
    STORED_UID = '00000069-0000-507c-bfff-ffd0ffffffc9'
    """
    INSERT NEW KEY HERE FROM self.testRegistration()
    """
    STORED_KEY = b'-----BEGIN RSA PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArbLoJwnlK9uoxXmN1Zw9\nADc+3Gr2pwKz8G0EVv3Zdwd+ToqH9CcDxRd71uzyFo49vziHJ/FCU15BAUh7lylS\nKuddab3JcPqlCRj3pxt2FxIu1EpjXDdqTT8Q+Ix+kEPTCDPCNjeidQRWiTRC2yC9\n6pKF7H74hbpSaXmviH3qR0UFW21ZA2Gzt8MMtwud6MZHixWXm0ZmSP0w9VJO1PEl\ncLefBKvGnEIzw+b/YJPy/W27mBOd7A/RtnAi87DexSd2B8B1fEs/Y7HYd5XyOQw8\nKxMDAWkagQPmWowTfqBF/ouLTUNaVK4Oz0oCaVxwks3CQKaL/kOgT0naCOS/pzAQ\n3wIDAQAB\n-----END RSA PUBLIC KEY-----'

    def url(self, relative, https: bool = False):
        return "{}://{}:{}{}".format("https" if https else "http",
                                      self.SERVER_ADDR, self.SERVER_PORT, relative)

    def register(self) -> Tuple[str, Any]:
        key = RSA.generate(2048)
        cryptor = PKCS1_OAEP.new(key)
        my_public = key.publickey().exportKey('PEM')
        with requests.Session() as sess:
            step_ret = sess.post(self.url("/user/register/sendKey"), json={
                "public_key": my_public.decode('ascii').replace(' RSA ', ' ')
            })  # type: requests.Response
            step_ret = step_ret.json()
            msg = cryptor.decrypt(base64.b64decode(step_ret["client_string"]))
            step_ret = sess.post(self.url("/user/register"), json={
                "server_string": step_ret["server_string"],
                "client_string": base64.b64encode(msg).decode('utf-8')
            })
            uid = step_ret.json()["uuid"]
        return uid, key

    def authorize(self, uid: str, key: any) -> requests.Session:
        uid, key = self.register()
        cryptor = PKCS1_OAEP.new(key)
        sess = requests.Session()
        sess.stream = True  # keep-alive
        step_ret = sess.post(self.url("/user/auth/sendUuid"), json={
            "uuid": uid
        })  # type: requests.Response
        print("1st authorization step > response: {}".format(step_ret.json()))
        if step_ret.status_code != requests.codes.OK:
            raise ConnectionError("Failed to continue authorization: status is not ok")
        step_ret = step_ret.json()
        msg = cryptor.decrypt(base64.b64decode(step_ret["client_string"]))
        step_ret = sess.post(self.url("/user/auth"), json={
            "server_string": step_ret["server_string"],
            "client_string": base64.b64encode(msg).decode('utf-8')
        })
        if step_ret.status_code != requests.codes.OK:
            raise ConnectionError("Failed to finish authorization: status is not ok")
        return sess


    def test_registration(self):
        key = RSA.generate(2048)
        my_public = key.publickey().exportKey('PEM')
        cryptor = PKCS1_OAEP.new(key)
        sess = requests.Session()
        step_ret = sess.post(self.url("/user/register/sendKey"), json={
            "public_key": my_public.decode('ascii').replace(' RSA ', ' ')
        })  # type: requests.Response
        self.assertTrue(step_ret.status_code, requests.codes.OK)
        step_ret = step_ret.json()
        print(step_ret)
        print("LENGTH: {}".format(len(step_ret["client_string"])))
        msg = cryptor.decrypt(base64.b64decode(step_ret["client_string"]))
        step_ret = sess.post(self.url("/user/register"), json= {
            "server_string": step_ret["server_string"],
            "client_string": base64.b64encode(msg).decode('utf-8')
        })
        uid = step_ret.json()["uuid"]
        print("SERVER RESPONSE: {}".format(step_ret.json()["uuid"]))
        print("CLIENT RESPONSE: {}".format(hashlib.sha1(my_public).hexdigest()))
        print("SAVE THIS KEY: {} ".format(key.exportKey('PEM')))
        print("SAVE THIS UUID: {} ".format(uid))

    def test_authorization(self):
        uid, key = self.register()
        cryptor = PKCS1_OAEP.new(key)
        with requests.Session() as sess:
            step_ret = sess.post(self.url("/user/auth/sendUuid"), json={
                "uuid": uid
            })  # type: requests.Response
            print("1st authorization step > response: {}".format(step_ret.json()))
            self.assertTrue(step_ret.status_code, requests.codes.OK)
            step_ret = step_ret.json()
            msg = cryptor.decrypt(base64.b64decode(step_ret["client_string"]))
            step_ret = sess.post(self.url("/user/auth"), json={
                "server_string": step_ret["server_string"],
                "client_string": base64.b64encode(msg).decode('utf-8')
            })
            self.assertTrue(step_ret.status_code, requests.codes.OK)

    def test_authorization_persistent(self):
        key = RSA.import_key(self.STORED_KEY)
        uid = self.STORED_UID
        cryptor = PKCS1_OAEP.new(key)
        with requests.Session() as sess:
            step_ret = sess.post(self.url("/user/auth/sendUuid"), json={
                "uuid": uid
            })  # type: requests.Response
            print("1st authorization step > response: {}".format(step_ret.json()))
            self.assertEqual(step_ret.status_code, requests.codes.OK)
            step_ret = step_ret.json()
            msg = cryptor.decrypt(base64.b64decode(step_ret["client_string"]))
            step_ret = sess.post(self.url("/user/auth"), json={
                "server_string": step_ret["server_string"],
                "client_string": base64.b64encode(msg).decode('utf-8')
            })
            self.assertEqual(step_ret.status_code, requests.codes.OK)

    def test_user_info(self):
        u1, u2_key = self.register()
        u2, u2_key = self.register()


