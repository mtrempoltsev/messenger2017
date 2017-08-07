#include "../include/login_manager.h"

#include <functional>
#include <boost/property_tree/ptree.hpp>

#include "../../../shared/include/crypto_pki.h"
#include "../../../shared/include/http_client.h"
#include "../../../shared/include/http_connection.h"
const int KEYSIZE = 2048;

using namespace m2::core;
using namespace  m2::crypto::common;
using namespace boost::property_tree;
LoginManager::LoginManager()
{

}

void LoginManager::RegisterUser(AsyncRegisterUserHandler handler, const std::string & serverDomain)
{
  //костыль-стайл. временно? Проверить криптоту на сучествование.
  crypto_ = OpenSSL_RSA_CryptoProvider::make(KEYSIZE);
  ptree pt;
  pt.put("public_key", crypto_.first.str_key());
  //магия отправки всего этого мусора в http client
  HttpClient*  hc = new HttpClient; //костыль времени.
  HttpConnection c1 = hc->connect(serverDomain);
  c1.perform({"/user/sendKey", timeout/*, data*/}, httpBuffer_,
             std::bind(&LoginManager::CheckKey, this, std::placeholders::_1, std::placeholders::_2));
}

void LoginManager::CheckKey(PerformResult result, HttpResponsePtr &&response)
{

}
