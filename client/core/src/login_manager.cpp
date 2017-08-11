#include "../include/login_manager.h"

#include <sstream>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "path_settings.h"

#define DEFAULT_ERROR Error(Error::Code::NoError, std::string());

//int definition
const int KEYSIZE = 2048;
const int TIMEOUT = 1000;

//json fields definition
const std::string c_server_string = "server_string";
const std::string c_client_string = "client_string";
const std::string c_uuid_string = "uuid";

const std::string c_server_field = "server_domain";
const std::string c_user_uuid_field = "user_uuid";
const std::string c_private_key_field = "private_key";
const std::string c_public_key_field = "public_key";

//using namspaces
using namespace m2::core;
using namespace  m2::crypto::common;
using namespace boost::property_tree;
using namespace m2::safelog;
using m2::Error;


LoginManager::LoginManager() :
    logger_(GetManagerPath("logs").append("login_manager.log")),
    loginFilePath_(GetManagerPath("login").append("loginfo.json"))
    //registrationError_(Error::Code::NoError, std::string())
{
    ReadLoginInfo();
}

Error LoginManager::RegisterUser(const HttpConnectionPtr &connection)
{
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

  currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)},
                                    responseDataCheckKey, httpBuffer_,
                                    std::bind(&LoginManager::UniveralCallback, this,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::ref(result), std::ref(response)));

  hasResponse_.wait(lockSendKey);

  Error checkKeyError = CheckServerResponse(result, response, "/user/sendKey", __LINE__);
  if (checkKeyError.code != Error::Code::NoError) {
    return checkKeyError;
  }

  ptree jsonPt;
  Error jsonError = CheckJsonValidFormat({ c_server_string, c_client_string }, __LINE__, jsonPt);
  if (jsonError.code != Error::Code::NoError) {
    return jsonError;
  }

  std::string serverString = jsonPt.get<std::string>(c_server_string);
  std::string clientString = jsonPt.get<std::string>(c_client_string);
  clientString = privateKey_->decrypt(clientString);
  jsonPt.put(c_client_string, clientString); //decrypt client string with privateKey_ and put into tree
  jsonPt.put(c_server_string, serverString);

  std::ostringstream ssConfirmKey;
  try {
    write_json(ssConfirmKey, jsonPt);
  }
  catch (json_parser_error & ex) {
    logger_(SL_ERROR) << ex.what();
    return Error(Error::Code::RegistationError, std::string("SOME ERROR I DO NOT KNOW!"));
  }
  
  std::string jsonConfirmKey = ssConfirmKey.str();
  std::vector<char> responseDataConfirmKey(jsonConfirmKey.begin(), jsonConfirmKey.end());
  httpBuffer_.clear();

  std::unique_lock<std::mutex> lockConfirmKey(mutex_);
  currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)},
                                    responseDataConfirmKey, httpBuffer_,
                                    std::bind(&LoginManager::UniveralCallback, this,
                                              std::placeholders::_1, std::placeholders::_2,
                                              std::ref(result), std::ref(response)));

  hasResponse_.wait(lockConfirmKey);


  Error filnalRegistrationError = CheckServerResponse(result, response, "/user/register", __LINE__);
  if (filnalRegistrationError.code != Error::Code::NoError) {
    return filnalRegistrationError;
  }

  jsonPt.clear();
  jsonError = CheckJsonValidFormat({ c_uuid_string }, __LINE__, jsonPt);
  if (jsonError.code != Error::Code::NoError) {
    return jsonError;
  }

  userUuid_ = jsonPt.get<std::string>(c_uuid_string);
  WriteLoginInfo();
  return Error(Error::Code::NoError, std::string());
}

void LoginManager::UniveralCallback(PerformResult result_in, HttpResponsePtr && response_in, PerformResult &result_out, HttpResponsePtr & response_out) {
    result_out = result_in;
    response_out = std::move(response_in);
    hasResponse_.notify_one();
}

Error LoginManager::CheckServerResponse(PerformResult & result, HttpResponsePtr & response, const std::string & requestName, int lineNum) {
  if (result == PerformResult::NetworkError)
  {
    logger_(SL_ERROR) << "Network Error in RegisterUser on line=" + std::to_string(lineNum);
    return Error(Error::Code::NetworkError, std::string("the \"" + requestName +"\" request failed"));
  }
  if (response->code != 200)
  {
    logger_(SL_ERROR) << "Bad response in RegisterUser on line=" + std::to_string(lineNum);
    return Error(Error::Code::NetworkError, std::string("the \"" + requestName + "\" response ended with code " + std::to_string(response->code)));
  }
  return DEFAULT_ERROR;
}

Error LoginManager::CheckJsonValidFormat(const std::list<std::string>& jsonParams, int lineNum, ptree & jsonPt) {
  std::istringstream iss(std::string(httpBuffer_.begin(), httpBuffer_.end()));
  try {
    read_json(iss, jsonPt);
  }
  catch (json_parser_error & ex) {
    logger_(SL_ERROR) << ex.what();
    //return Error(Error::Code::RegistationError, std::string("the \"" + requestName + "\" response data has not json format"));
    return Error(Error::Code::RegistationError, std::string("the \\ response data has not json format"));
  }

  Error jsonParamNotFoundError(Error::Code::NoError, std::string());
  for (auto & jsonParam : jsonParams) {
   if (jsonPt.find(jsonParam) == jsonPt.not_found()) {
     logger_(SL_ERROR) << "Bad " + jsonParam + " JSON in RegisterUser on line=" + std::to_string(lineNum);
     jsonParamNotFoundError.message += "the \"" + jsonParam + "\" field has not found";
   }
  }
  if (!jsonParamNotFoundError.message.empty()) {
    jsonParamNotFoundError.code = Error::Code::NetworkError;
    return jsonParamNotFoundError;
  }
  return DEFAULT_ERROR;
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

std::string LoginManager::Login(const m2::HttpConnectionPtr &connection)
{

}

void LoginManager::ReadLoginInfo() {
    std::ifstream loginFile(loginFilePath_);
    if(loginFile.is_open()) {
        ptree pt;
        try {
          read_json(loginFile, pt);
        }
        catch (json_parser_error & ex) {
          logger_(SL_ERROR) << ex.what();
          loginFile.close();
          return;
        }
        if(pt.find(c_user_uuid_field) == pt.not_found() || pt.find(c_server_field) == pt.not_found() ||
           pt.find(c_private_key_field) == pt.not_found() || pt.find(c_public_key_field) == pt.not_found()){
          logger_(SL_ERROR) << "Login info file is corrupted";
          loginFile.close();
          return;
        }

        try {
            privateKey_ = std::make_unique<OpenSSL_RSA_CryptoProvider>(OpenSSL_RSA_CryptoProvider(pt.get<std::string>(c_private_key_field), false));
            publicKey_ = std::make_unique<OpenSSL_RSA_CryptoProvider>(OpenSSL_RSA_CryptoProvider(pt.get<std::string>(c_public_key_field), true));
        }
        catch (std::exception & ex) {
            logger_(SL_ERROR) << ex.what();
            loginFile.close();
            return;
        }
        serverDomain_ = pt.get<std::string>(c_server_field);
        userUuid_ = pt.get<std::string>(c_user_uuid_field);
        loginFile.close();
    }
    else {
        logger_(SL_ERROR) << "Could not open login info file";
    }
}

void LoginManager::WriteLoginInfo() {
    std::ofstream loginFile(loginFilePath_);
    if(loginFile.is_open()) {
        ptree pt;
        pt.put(c_server_field, serverDomain_);
        pt.put(c_user_uuid_field, userUuid_);
        pt.put(c_private_key_field, privateKey_->str_key());
        pt.put(c_public_key_field, publicKey_->str_key());
        serverDomain_ = pt.get<std::string>(c_server_field);
        userUuid_ = pt.get<std::string>(c_user_uuid_field);

        try {
          write_json(loginFile, pt);
        }
        catch (json_parser_error & ex) {
          logger_(SL_ERROR) << ex.what();
        }
        loginFile.close();
    }
    else {
        logger_(SL_ERROR) << "Could not open login info file";
    }
}
