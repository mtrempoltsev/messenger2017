#pragma once

#include <string>
#include <functional>
#include <memory>
#include <list>

#include "safe_log.h"
#include "http_connection.h"
#include "crypto_pki.h"
namespace m2 {
namespace core {
class LoginManager {
public:
  LoginManager();
  using AsyncRegisterUserHandler = std::function<void(int)>;
  std::string Login(const HttpConnectionPtr & connection);
  bool RegisterUser(const HttpConnectionPtr & connection);
  void SetHttpClient (HttpClient);
  std::string GetChosenServerDomain();
  std::string GetChosenServerUuid();

  std::list<std::string> GetServerList();
private:
  void CheckKey(PerformResult result, HttpResponsePtr && response);
  void FilnalRegistration(PerformResult result, HttpResponsePtr &&response);
  std::unique_ptr<crypto::common::OpenSSL_RSA_CryptoProvider> publicKey_;
  std::unique_ptr<crypto::common::OpenSSL_RSA_CryptoProvider> privateKey_;
  //std::pair<crypto::common::OpenSSL_RSA_CryptoProvider, crypto::common::OpenSSL_RSA_CryptoProvider> crypto_;
  std::vector<char> httpBuffer_;

  safelog::SafeLog logger_;
  bool result_;
  bool inProcess_;
  HttpConnectionPtr currentConnection_;
  std::string userUuid_;
};
} // core
} // m2
