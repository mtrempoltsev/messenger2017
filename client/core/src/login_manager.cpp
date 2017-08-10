#include "../include/login_manager.h"

#include <functional>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <32/bits/atomic_word.h>

#include "path_settings.h"

const int KEYSIZE = 2048;
const int TIMEOUT = 1000;
const std::string c_server_string = "server_string";
const std::string c_client_string = "client_string";
const std::string c_uuid_string = "uuid";

using namespace m2::core;
using namespace  m2::crypto::common;
using namespace boost::property_tree;
using namespace m2::safelog;

using m2::Error;


LoginManager::LoginManager() :
    logger_(GetManagerPath("log").append("login_manager.log")),
    registrationError_(Error::Code::NoError, std::string())
{

}

std::string LoginManager::Login(const m2::HttpConnectionPtr &connection)
{

}

Error LoginManager::RegisterUser(const HttpConnectionPtr &connection)
{
  registrationError_ = Error(Error::Code::NoError, std::string());
  currentConnection_ = connection;
  inProcess_ = true;
  std::pair<OpenSSL_RSA_CryptoProvider, OpenSSL_RSA_CryptoProvider> crypto = OpenSSL_RSA_CryptoProvider::make(KEYSIZE);

  publicKey_ = std::make_unique<OpenSSL_RSA_CryptoProvider>(crypto.first.str_key(), true);
  privateKey_ = std::make_unique<OpenSSL_RSA_CryptoProvider>(crypto.second.str_key(), false);
  ptree ptreeSendKey;
  ptreeSendKey.put("public_key", publicKey_->str_key());
  std::ostringstream ssSendKey;
  write_json(ssSendKey, ptreeSendKey);
  std::string jsonSendKey = ssSendKey.str();
  std::vector<char> responseDataCheckKey(jsonSendKey.begin(), jsonSendKey.end());

  PerformResult result;
  HttpResponsePtr response;

  logger_(SL_DEBUG) << "Prev mutex";
  std::unique_lock<std::mutex> lockSendKey(mutex_);
  logger_(SL_DEBUG) << "Mutex locked";
  currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)}, responseDataCheckKey, httpBuffer_,
             std::bind(&LoginManager::CheckKey, this, std::placeholders::_1, std::placeholders::_2, std::ref(result), std::ref(response)));
  hasResponse_.wait(lockSendKey);

  if(result == PerformResult::NetworkError)
  {
    logger_(SL_ERROR) << "Network Error in RegisterUser on line=" + std::to_string(__LINE__);
    return Error(Error::Code::NetworkError, std::string("the \"/user/sendKey\" request failed"));
  }
  if(response->code != 200)
  {
      logger_(SL_ERROR) << "Bad response code in RegisterUser on line=" + std::to_string(__LINE__);
      return Error(Error::Code::NetworkError, std::string("the \"/user/checkKey\" response ended with code " + std::to_string(response->code)));
  }

  std::string jsonCheckKey(httpBuffer_.begin(), httpBuffer_.end());
  std::istringstream ssCheckKey(jsonCheckKey);
  ptree ptreeCheckKey;
  bool returnAfterCatch = false;
  try {
      read_json(ssCheckKey, ptreeCheckKey);
  }
  catch (json_parser_error & ex) {
    logger_(SL_ERROR) << ex.what();
    returnAfterCatch = true;
  }
  if (returnAfterCatch)
      return Error(Error::Code::NetworkError, std::string("the \"/user/checkKey\" response data has not json format"));
  if(ptreeCheckKey.find(c_server_string) == ptreeCheckKey.not_found() ||
     ptreeCheckKey.find(c_client_string) == ptreeCheckKey.not_found())
  {
    Error registrationError(Error::Code::NoError, std::string());
    if(ptreeCheckKey.find(c_server_string) == ptreeCheckKey.not_found()) {
      logger_(SL_ERROR) << "Bad server_string JSON in RegisterUser on line=" + std::to_string(__LINE__);
      registrationError.message += "the \"server_string\" field has not founded\n";
    }
    if(ptreeCheckKey.find(c_client_string) == ptreeCheckKey.not_found()) {
      logger_(SL_ERROR) << "Bad client_string JSON in RegisterUser on line=" + std::to_string(__LINE__);
      registrationError.message += "the \"client_string\" field has not founded\n";
    }
    registrationError.code = Error::Code::NetworkError;
    return registrationError;
  }
  ptreeCheckKey.clear();
  std::string serverString = ptreeCheckKey.get<std::string>("server_string");
  std::string clientString = ptreeCheckKey.get<std::string>("client_string");
  clientString = privateKey_->decrypt(clientString);
  ptreeCheckKey.put(c_client_string, clientString); //decrypt client string with privateKey_ and put into tree
  ptreeCheckKey.put(c_server_string, serverString);

  std::ostringstream ssConfirmKey;
  write_json(ssConfirmKey, ptreeCheckKey);
  std::string jsonConfirmKey = ssConfirmKey.str();
  std::vector<char> responseDataConfirmKey(jsonConfirmKey.begin(), jsonConfirmKey.end());
  httpBuffer_.clear();

  std::unique_lock<std::mutex> lockConfirmKey(mutex_);
  currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)}, responseDataConfirmKey, httpBuffer_,
             std::bind(&LoginManager::FilnalRegistration, this, std::placeholders::_1, std::placeholders::_2, std::ref(result), std::ref(response)));
  hasResponse_.wait(lockConfirmKey);

  if(result == PerformResult::NetworkError)
  {
      logger_(SL_ERROR) << "Network Error in RegisterUser on line=" + std::to_string(__LINE__);
      return Error(Error::Code::NetworkError, std::string("the \"/user/register\" request failed"));
  }
  if(response->code != 200)
  {
      logger_(SL_ERROR) << "Bad response in RegisterUser on line=" + std::to_string(__LINE__);
      return Error(Error::Code::NetworkError, std::string("the \"/user/register\" response ended with code " + std::to_string(response->code)));
  }
  std::string jsonFinalRegistration(httpBuffer_.begin(), httpBuffer_.end());
  std::istringstream ssFinalRegistration(jsonFinalRegistration);
  ptree ptreeFinalRegistration;
  returnAfterCatch = false;
  try {
      read_json(ssFinalRegistration, ptreeFinalRegistration);
  }
  catch (json_parser_error & ex) {
    logger_(SL_ERROR) << ex.what();
    returnAfterCatch = true;
  }
  if (returnAfterCatch)
      return Error(Error::Code::NetworkError, std::string("the \"/user/register\" response data has not json format"));
  if(ptreeFinalRegistration.find(c_uuid_string) == ptreeFinalRegistration.not_found()) {
    logger_(SL_ERROR) << "Bad uuid_string JSON in RegisterUser on line=" + std::to_string(__LINE__);
    return Error(Error::Code::NetworkError, std::string("the \"" + c_uuid_string + "\" field has not founded\n"));
  }
  userUuid_ = ptreeFinalRegistration.get<std::string>(c_uuid_string);
  return Error(Error::Code::NoError, std::string());
}

void LoginManager::CheckKey(PerformResult result_in, HttpResponsePtr && response_in, PerformResult &result_out, HttpResponsePtr & response_out) {
    result_out = result_in;
    response_out = std::move(response_in);
    hasResponse_.notify_one();
}

void LoginManager::FilnalRegistration(PerformResult result_in, HttpResponsePtr && response_in, PerformResult &result_out, HttpResponsePtr & response_out)
{
    result_out = result_in;
    response_out = std::move(response_in);
    hasResponse_.notify_one();
}

std::list<std::string> LoginManager::GetServerList()
{
  std::list<std::string> serverList;
  std::ifstream serverListStream(GetManagerPath().append("server_list.json"));
  if (serverListStream.is_open()) {
      ptree pt;
      try {
        read_json(serverListStream, pt);
        for (auto & iter : pt.get_child("servers"))
            serverList.push_back(iter.second.get<std::string>(""));
      }
      catch (json_parser_error & ex) {
        logger_(SL_ERROR) << ex.what();
      }
  }
  else {
      logger_(SL_ERROR) << "the \"Server List\" file did not opened!";
  }
  return serverList;
}

