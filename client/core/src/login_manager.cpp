#include "../include/login_manager.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "base64.h"
#include "path_settings.h"

#define DEFAULT_ERROR Error(Error::Code::NoError, std::string());

// int definition
const int KEYSIZE = 2048;
const int TIMEOUT = 1000;

// json fields definition
const std::string c_server_string = "server_string";
const std::string c_client_string = "client_string";
const std::string c_uuid_string = "uuid";

const std::string c_server_field = "server_domain";
const std::string c_user_uuid_field = "user_uuid";
const std::string c_private_key_field = "private_key";
const std::string c_public_key_field = "public_key";
const std::string c_error_text_field = "error_msg";

// error messages
const std::string c_write_json_error = "some error text";

// request names
const std::string c_send_key_request = "/user/sendKey";
const std::string c_register_request = "/user/register";

// using namspaces
using namespace m2::core;
using namespace m2::crypto::common;
using namespace boost::property_tree;
using namespace m2::safelog;
using m2::Error;

LoginManager::LoginManager()
    : logger_(GetManagerPath("logs").append("login_manager.log")),
      loginFilePath_(GetManagerPath("login").append("loginfo.json"))
// registrationError_(Error::Code::NoError, std::string())
{
  ReadLoginInfo();
}

Error LoginManager::RegisterUser(const HttpConnectionPtr connection) {
  logger_(SL_INFO) << "RegisterUser";
  currentConnection_ = connection;
  crypto_ = OpenSSL_RSA_CryptoProvider::make(KEYSIZE);

  // publicKey_ =
  // std::make_unique<OpenSSL_RSA_CryptoProvider>(crypto.first->str_key(),
  // true);
  // privateKey_ =
  // std::make_unique<OpenSSL_RSA_CryptoProvider>(crypto.second->str_key(),
  // false);

  Error error = TalkWithServer(c_send_key_request, c_register_request,
                               crypto_.first->str_key());
  if (error.code != Error::Code::NoError)
    return error;

  userUuid_ = error.message;
  WriteLoginInfo();

  return DEFAULT_ERROR;
}

Error LoginManager::Login(const HttpConnectionPtr connection) {
  currentConnection_ = connection;
  Error error =
      TalkWithServer(c_send_key_request, c_register_request, userUuid_);
  if (error.code != Error::Code::NoError)
    return error;
  ptree jsonPt;
  error =
      SendRequestProccess(c_send_key_request, {{c_public_key_field, userUuid_}},
                          {c_server_string, c_client_string}, jsonPt);
  if (error.code != Error::Code::NoError)
    return error;

  return DEFAULT_ERROR;
}

Error LoginManager::TalkWithServer(const std::string &firstRequestName,
                                   const std::string &secondRequestName,
                                   const std::string &keyStr) {
  ptree jsonPt;
  Error error =
      SendRequestProccess(firstRequestName, {{c_public_key_field, keyStr}},
                          {c_server_string, c_client_string}, jsonPt);
  if (error.code != Error::Code::NoError)
    return error;

  std::string serverString = jsonPt.get<std::string>(c_server_string);
  std::string clientString =
      base64::base64_encode(crypto_.second->decrypt_from_b64(
          base64::base64_decode(jsonPt.get<std::string>(c_client_string))));

  httpBuffer_.clear();
  jsonPt.clear();

  error = SendRequestProccess(
      secondRequestName,
      {{c_client_string, clientString}, {c_server_string, serverString}}, {},
      jsonPt);
  if (error.code != Error::Code::NoError)
    return error;
  return Error(Error::Code::NoError, jsonPt.get<std::string>(c_uuid_string));
}

Error LoginManager::SendRequestProccess(
    const std::string &requestName,
    const std::map<std::string, std::string> &jsonKeyValues,
    const std::list<std::string> &jsonParams,
    boost::property_tree::ptree &jsonPt) {
  PerformResult result;
  HttpResponsePtr response;
  std::vector<char> httpRequestData;
  Error prepareError =
      PrepareHttpRequest(jsonKeyValues, httpRequestData, c_write_json_error);
  if (prepareError.code == Error::Code::NoError) {
    std::unique_lock<std::mutex> lockSendKey(mutex_);
    currentConnection_->perform(
        {requestName, std::chrono::milliseconds(TIMEOUT)}, httpRequestData,
        httpBuffer_,
        std::bind(&LoginManager::UniveralCallback, this, std::placeholders::_1,
                  std::placeholders::_2, std::ref(result), std::ref(response)));
    hasResponse_.wait(lockSendKey);
    Error checkKeyError =
        CheckServerResponse(result, response, requestName, __LINE__);
    if (checkKeyError.code != Error::Code::NoError) {
      return checkKeyError;
    }
    Error jsonError = CheckJsonValidFormat(jsonParams, __LINE__, jsonPt);
    if (jsonError.code != Error::Code::NoError) {
      return jsonError;
    }
  }
  return prepareError;
}

Error LoginManager::PrepareHttpRequest(
    const std::map<std::string, std::string> &jsonKeyValues,
    std::vector<char> &httpRequestData,
    const std::string &jsonWriteErrorMessage) {
  ptree jsonPt;
  for (auto &jsonIter : jsonKeyValues) {
    jsonPt.put(jsonIter.first, jsonIter.second);
  }
  std::ostringstream oss;
  try {
    write_json(oss, jsonPt);
  } catch (json_parser_error &ex) {
    logger_(SL_ERROR) << ex.what();
    return Error(Error::Code::RegistationError,
                 std::string(jsonWriteErrorMessage));
  }
  std::string resultString = oss.str();
  std::copy(resultString.begin(), resultString.end(),
            std::back_inserter(httpRequestData));
  return DEFAULT_ERROR;
}

void LoginManager::UniveralCallback(PerformResult result_in,
                                    HttpResponsePtr &&response_in,
                                    PerformResult &result_out,
                                    HttpResponsePtr &response_out) {
  result_out = result_in;
  response_out = std::move(response_in);
  hasResponse_.notify_one();
}

Error LoginManager::CheckServerResponse(PerformResult &result,
                                        HttpResponsePtr &response,
                                        const std::string &requestName,
                                        int lineNum) {
  if (result == PerformResult::NetworkError) {
    logger_(SL_ERROR) << "Network Error in RegisterUser on line=" +
                             std::to_string(lineNum);
    return Error(Error::Code::NetworkError,
                 std::string("the \"" + requestName + "\" request failed"));
  }
  if (response->code != 200) {
    ptree errorPt;
    Error error = CheckJsonValidFormat({c_error_text_field}, lineNum, errorPt);
    if (error.code == Error::Code::NoError)
      return Error(Error::Code::ServerError,
                   std::string(errorPt.get<std::string>(c_error_text_field)));
    else
      return error;
  }
  return DEFAULT_ERROR;
}

Error LoginManager::CheckJsonValidFormat(
    const std::list<std::string> &jsonParams, int lineNum, ptree &jsonPt) {
  if (jsonParams.empty())
    return DEFAULT_ERROR;
  std::istringstream iss(std::string(httpBuffer_.begin(), httpBuffer_.end()));
  try {
    read_json(iss, jsonPt);
  } catch (json_parser_error &ex) {
    logger_(SL_ERROR) << ex.what();
    // return Error(Error::Code::RegistationError, std::string("the \"" +
    // requestName + "\" response data has not json format"));
    return Error(Error::Code::RegistationError,
                 std::string("the \\ response data has not json format"));
  }

  Error jsonParamNotFoundError(Error::Code::NoError, std::string());
  for (auto &jsonParam : jsonParams) {
    if (jsonPt.find(jsonParam) == jsonPt.not_found()) {
      logger_(SL_ERROR) << "Bad " + jsonParam +
                               " JSON in RegisterUser on line=" +
                               std::to_string(lineNum);
      jsonParamNotFoundError.message +=
          "the \"" + jsonParam + "\" field has not found";
    }
  }
  if (!jsonParamNotFoundError.message.empty()) {
    jsonParamNotFoundError.code = Error::Code::NetworkError;
    return jsonParamNotFoundError;
  }
  return DEFAULT_ERROR;
}

std::list<std::string> LoginManager::GetServerList() {
  std::list<std::string> serverList;
  std::ifstream serverListStream(GetManagerPath().append("server_list.json"));
  if (serverListStream.is_open()) {
    ptree pt;
    try {
      read_json(serverListStream, pt);
      for (auto &iter : pt.get_child("servers"))
        serverList.push_back(iter.second.get<std::string>(""));
    } catch (json_parser_error &ex) {
      logger_(SL_ERROR) << ex.what();
    }
  } else {
    logger_(SL_ERROR) << "the \"Server List\" file did not opened!";
  }
  return serverList;
}

void LoginManager::ReadLoginInfo() {
  std::ifstream loginFile(loginFilePath_);
  if (loginFile.is_open()) {
    ptree pt;
    try {
      read_json(loginFile, pt);
    } catch (json_parser_error &ex) {
      logger_(SL_ERROR) << ex.what();
      loginFile.close();
      return;
    }
    if (pt.find(c_user_uuid_field) == pt.not_found() ||
        pt.find(c_server_field) == pt.not_found() ||
        pt.find(c_private_key_field) == pt.not_found() ||
        pt.find(c_public_key_field) == pt.not_found()) {
      logger_(SL_ERROR) << "Login info file is corrupted";
      loginFile.close();
      return;
    }

    try {
      privateKey_ = std::make_unique<OpenSSL_RSA_CryptoProvider>(
          OpenSSL_RSA_CryptoProvider(pt.get<std::string>(c_private_key_field),
                                     false));
      publicKey_ = std::make_unique<OpenSSL_RSA_CryptoProvider>(
          OpenSSL_RSA_CryptoProvider(pt.get<std::string>(c_public_key_field),
                                     true));
    } catch (std::exception &ex) {
      logger_(SL_ERROR) << ex.what();
      loginFile.close();
      return;
    }
    serverDomain_ = pt.get<std::string>(c_server_field);
    userUuid_ = pt.get<std::string>(c_user_uuid_field);
    loginFile.close();
  } else {
    logger_(SL_ERROR) << "Could not open login info file";
  }
}

void LoginManager::WriteLoginInfo() {
  std::ofstream loginFile(loginFilePath_);
  if (loginFile.is_open()) {
    ptree pt;
    pt.put(c_server_field, serverDomain_);
    pt.put(c_user_uuid_field, userUuid_);
    pt.put(c_private_key_field, privateKey_->str_key());
    pt.put(c_public_key_field, publicKey_->str_key());
    serverDomain_ = pt.get<std::string>(c_server_field);
    userUuid_ = pt.get<std::string>(c_user_uuid_field);

    try {
      write_json(loginFile, pt);
    } catch (json_parser_error &ex) {
      logger_(SL_ERROR) << ex.what();
    }
    loginFile.close();
  } else {
    logger_(SL_ERROR) << "Could not open login info file";
  }
}
