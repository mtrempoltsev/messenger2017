#include "../include/login_manager.h"

#include <functional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

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

LoginManager::LoginManager() :
  logger_(GetManagerPath("log").append("login_manager.log"))
{

}

std::string LoginManager::Login(const m2::HttpConnectionPtr &connection)
{

}

bool LoginManager::RegisterUser(const HttpConnectionPtr &connection)
{
  currentConnection_ = connection;
  inProcess_ = true;
  result_ = true;
  std::pair<crypto::common::OpenSSL_RSA_CryptoProvider, crypto::common::OpenSSL_RSA_CryptoProvider> crypto = OpenSSL_RSA_CryptoProvider::make(KEYSIZE);
  publicKey_ = std::move(crypto.first);
  privateKey_ = std::move(crypto.second);
  ptree pt;
  pt.put("public_key", publicKey_.first.str_key());
  std::ostringstream ss;
  write_json(ss, pt);
  std::string json = ss.str();
  std::vector<char> responseData(json.begin(), json.end());
  //магия отправки всего этого мусора в http client
  currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)}, responseData, httpBuffer_,
             std::bind(&LoginManager::CheckKey, this, std::placeholders::_1, std::placeholders::_2));
  while(inProcess_);
  return result_;
}

void LoginManager::CheckKey(PerformResult result, HttpResponsePtr &&response)
{
    if(result == PerformResult::NetworkError)
    {
        logger_(SL_ERROR) << "Network Error in CheckKey";
        result_ = false;
        inProcess_ = false;
        return;
    }
    if(response->code != 200)
    {
        logger_(SL_ERROR) << "Bad response code in CheckKey";
        result_ = false;
        return;
    }
    std::string json_str(httpBuffer_.begin(), httpBuffer_.end());
    std::istringstream ss(json_str);
    ptree pt;
    try {
        read_json(ss, pt);
    }
    catch (json_parser_error & ex) {
      logger_(SL_ERROR) << ex.what();
      result_ = false;
      inProcess_ = false;
    }
    if(result == PerformResult::Success)
    {
        if(pt.find(c_server_string) == pt.not_found() ||
           pt.find(c_client_string) == pt.not_found())
        {
          if(pt.find(c_server_string) == pt.not_found())
            logger_(SL_ERROR) << "Bad server_string JSON in CheckKey";
          if(pt.find(c_client_string) == pt.not_found())
            logger_(SL_ERROR) << "Bad client_string JSON in CheckKey";
          result_ = false;
          inProcess_ = false;
          return;
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
        //магия отправки всего этого мусора в http client
        currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)}, responseData, httpBuffer_,
                   std::bind(&LoginManager::FilnalRegistration, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void LoginManager::FilnalRegistration(m2::PerformResult result, m2::HttpResponsePtr &&response)
{
    if(result == PerformResult::NetworkError)
    {
        logger_(SL_ERROR) << "Network Error in CheckKey";
        result_ = false;
        inProcess_ = false;
        return;
    }
    if(response->code != 200)
    {
        logger_(SL_ERROR) << "Bad response code in CheckKey";
        result_ = false;
        return;
    }
    std::string json_str(httpBuffer_.begin(), httpBuffer_.end());
    std::istringstream ss(json_str);
    ptree pt;
    try {
        read_json(ss, pt);
    }
    catch (json_parser_error & ex) {
      logger_(SL_ERROR) << ex.what();
      result_ = false;
      inProcess_ = false;
    }
    if(result == PerformResult::Success){
        if(pt.find(c_uuid_string) == pt.not_found()) {
            logger_(SL_ERROR) << "Bad uuid_string JSON in CheckKey";
            result_ = false;
            inProcess_ = false;
            return;
        }
        userUuid_ = pt.get<std::string>(c_uuid_string);
        inProcess_ = false; //we are get all info
        result_ = true;
        return;
    }
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

