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
  ptree pt;
  pt.put("public_key", publicKey_->str_key());
  std::ostringstream ss;
  write_json(ss, pt);
  std::string json = ss.str();
  std::vector<char> responseData(json.begin(), json.end());

  PerformResult result;
  HttpResponsePtr response;

  std::unique_lock<std::mutex> lock(mutex_);
  currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)}, responseData, httpBuffer_,
             std::bind(&LoginManager::CheckKey, this, std::placeholders::_1, std::placeholders::_2, std::ref(result), std::ref(response)));
  hasResponse_.wait(lock);

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

  std::string json_str(httpBuffer_.begin(), httpBuffer_.end());
  std::istringstream ss(json_str);
  ptree pt;
  bool returnAfterCatch = false;
  try {
      read_json(ss, pt);
  }
  catch (json_parser_error & ex) {
    logger_(SL_ERROR) << ex.what();
    returnAfterCatch = true;
  }
  if (returnAfterCatch)
      return Error(Error::Code::NetworkError, std::string("the \"/user/checkKey\" response data has not json format"));
  if(pt.find(c_server_string) == pt.not_found() ||
     pt.find(c_client_string) == pt.not_found())
  {
    Error registrationError(Error::Code::NoError, std::string());
    if(pt.find(c_server_string) == pt.not_found()) {
      logger_(SL_ERROR) << "Bad server_string JSON in RegisterUser on line=" + std::to_string(__LINE__);
      registrationError.message += "the \"server_string\" field has not founded\n";
    }
    if(pt.find(c_client_string) == pt.not_found()) {
      logger_(SL_ERROR) << "Bad client_string JSON in RegisterUser on line=" + std::to_string(__LINE__);
      registrationError.message += "the \"client_string\" field has not founded\n";
    }
    registrationError.code = Error::Code::NetworkError;
    return registrationError;
  }
  pt.clear();
  std::string serverString = pt.get<std::string>("server_string");
  std::string clientString = pt.get<std::string>("client_string");
  clientString = privateKey_->decrypt(clientString);
  pt.put(c_client_string, clientString); //decrypt client string with privateKey_ and put into tree
  pt.put(c_server_string, serverString);

  std::ostringstream ss;
  write_json(ss, pt);
  std::string json = ss.str();
  std::vector<char> responseData(json.begin(), json.end());
  httpBuffer_.clear();

  std::unique_lock<std::mutex> lock(mutex_);
  currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)}, responseData, httpBuffer_,
             std::bind(&LoginManager::FilnalRegistration, this, std::placeholders::_1, std::placeholders::_2, std::ref(result), std::ref(response)));
  hasResponse_.wait(lock);

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
  std::string json_str(httpBuffer_.begin(), httpBuffer_.end());
  std::istringstream ss(json_str);
  ptree pt;
  returnAfterCatch = false;
  try {
      read_json(ss, pt);
  }
  catch (json_parser_error & ex) {
    logger_(SL_ERROR) << ex.what();
    returnAfterCatch = true;
  }
  if (returnAfterCatch)
      return Error(Error::Code::NetworkError, std::string("the \"/user/register\" response data has not json format"));
  if(pt.find(c_uuid_string) == pt.not_found()) {
    logger_(SL_ERROR) << "Bad uuid_string JSON in RegisterUser on line=" + std::to_string(__LINE__);
    return Error(Error::Code::NetworkError, std::string("the \"" + c_uuid_string + "\" field has not founded\n"));
  }
  userUuid_ = pt.get<std::string>(c_uuid_string);
  return Error(Error::Code::NoError, std::string());
}

void LoginManager::CheckKey(PerformResult result_in, HttpResponsePtr && response_in, PerformResult &result_out, HttResponsePtr & response_out) {
    response_out = result_in;
    response_out = response_in;
    hasResponse_.notify_one();
}

void LoginManager::FilnalRegistration(m2::PerformResult result, m2::HttpResponsePtr &&response)
{
    response_out = result_in;
    response_out = response_in;
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

