#pragma once

#include <set>
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

#include <boost/property_tree/ptree.hpp>

namespace m2 {
namespace core {
class LoginManager {

public:
  LoginManager();

  std::string Login(const HttpConnectionPtr & connection);
  Error RegisterUser(const HttpConnectionPtr & connection);
  std::string GetServerDomain() { return serverDomain_;}
  void SetServerDomain(const std::string & severDomain) { serverDomain_ = severDomain; }
  //std::string GetChosenServerUuid();

  std::list<std::string> GetServerList();
  void ReadLoginInfo();
  void WriteLoginInfo();

private:
  void UniveralCallback(PerformResult result_in, HttpResponsePtr && response_in, PerformResult &result_out, HttpResponsePtr & response_out);
  Error CheckServerResponse(PerformResult & result, HttpResponsePtr & response, const std::string & requestName, int lineNum);
  Error CheckJsonValidFormat(const std::list<std::string>& jsomParams, int lineNum, boost::property_tree::ptree & jsonPt);

  std::vector<char> httpBuffer_;

  safelog::SafeLog logger_;

  //thread control
  std::mutex mutex_;
  std::condition_variable hasResponse_;
  bool inProcess_;

  HttpConnectionPtr currentConnection_;
  //login info
  std::string loginFilePath_;
  std::string userUuid_;
  std::string serverDomain_;
  std::unique_ptr<crypto::common::OpenSSL_RSA_CryptoProvider> publicKey_;
  std::unique_ptr<crypto::common::OpenSSL_RSA_CryptoProvider> privateKey_;

};
}  // core
}  // m2
