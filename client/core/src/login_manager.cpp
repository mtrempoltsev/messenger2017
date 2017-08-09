#include "../include/login_manager.h"

#include <functional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

#include "path_settings.h"

const int KEYSIZE = 2048;
const int TIMEOUT = 1000;

using namespace m2::core;
using namespace  m2::crypto::common;
using namespace boost::property_tree;
using namespace m2::safelog;

LoginManager::LoginManager() :
  logger_(GetManagerPath("log").append("login_manager.log"))
{

}

bool LoginManager::RegisterUser()
{
  inProcess_ = true;
  result_ = true;
  //костыль-стайл. временно? Проверить криптоту на сучествование.
  //crypto_ = OpenSSL_RSA_CryptoProvider::make(KEYSIZE);//РАКОМЕНТИТЬ, ЕПТА
  ptree pt;
  //pt.put("public_key", crypto_.first.str_key());//РАКОМЕНТИТЬ, ЕПТА
  std::ostringstream ss;
  write_json(ss, pt);
  std::string json = ss.str();
  std::vector<char> responseData(json.begin(), json.end());
  //магия отправки всего этого мусора в http client
  //currentConnection_.get()->perform({"/user/sendKey", std::chrono::milliseconds(TIMEOUT)}, responseData, httpBuffer_,
  //           std::bind(&LoginManager::CheckKey, this, std::placeholders::_1, std::placeholders::_2));
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
    if(result)
    {
        if(!pt.find("server_string") || !pt.find("client_string") || !pt.find("encr_public_key"))
        {
          if(!pt.find("server_list"))
            logger_(SL_ERROR) << "Bad server_string JSON in CheckKey";
          if(!pt.find("client_string"))
            logger_(SL_ERROR) << "Bad client_string JSON in CheckKey";
          if(!pt.find("encr_public_key"))
            logger_(SL_ERROR) << "Bad encr_public_key JSON in CheckKey";
          result_ = false;
          inProcess_ = false;
          return;
        }
        //pt.put() //decrypt client string with privateKey_ and put into tree
        //pt.put() //put our publicKey_
        //
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

