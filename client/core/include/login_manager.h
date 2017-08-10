#pragma once

#include <string>
#include <functional>
#include <memory>
#include <list>
#include <mutex>
#include <condition_variable>

#include "safe_log.h"
#include "http_connection.h"
#include "crypto_pki.h"
#include "error.h"

namespace m2 {
namespace core {
class LoginManager {
public:
  LoginManager();
  std::string Login(const HttpConnectionPtr & connection);
  Error RegisterUser(const HttpConnectionPtr & connection);
  void SetHttpClient (HttpClient);
  std::string GetChosenServerDomain();
  std::string GetChosenServerUuid();

  std::list<std::string> GetServerList();
private:
  void CheckKey(PerformResult result_in, HttpResponsePtr && response_in, PerformResult &result_out, HttpResponsePtr & response_out);
  void FilnalRegistration(PerformResult result_in, HttpResponsePtr && response_in, PerformResult &result_out, HttpResponsePtr & response_out);
  std::unique_ptr<crypto::common::OpenSSL_RSA_CryptoProvider> publicKey_;
  std::unique_ptr<crypto::common::OpenSSL_RSA_CryptoProvider> privateKey_;
  //std::pair<crypto::common::OpenSSL_RSA_CryptoProvider, crypto::common::OpenSSL_RSA_CryptoProvider> crypto_;
  std::vector<char> httpBuffer_;

  safelog::SafeLog logger_;
  bool inProcess_;
  HttpConnectionPtr currentConnection_;
  std::string userUuid_;

  Error registrationError_;

  std::mutex mutex_;
  std::condition_variable hasResponse_;
};
} // core
} // m2
