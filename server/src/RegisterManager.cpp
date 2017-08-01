#include "RegisterManager.h"

using namespace m2::server;

std::string RegisterManager::deserializePublicKey(const std::string &data) {
  pt::ptree request;
  std::string publicKey;
  const std::string dd = data;
  boost::iostreams::stream<boost::iostreams::array_source> stream(data.c_str(), data.size());
  try {
    boost::property_tree::read_json(stream, request);
    publicKey = request.get<std::string>("public_key");
  }
  catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl; //TODO придумать как это обрабатывать
  }
  return publicKey;
}

std::string RegisterManager::doAction(const std::string &data, kindAction kind) {
  std::string answer;
  std::string temp = deserializePublicKey(data);
  std::cout << "public_key:" << temp << std::endl;

  switch (kind) {
    case createEncrypted: {
      //тут создавать ответ с зашифрованным ключом
      break;
    }
    case registerUser : {
      Decrypted info = deserializeDecrypted(data);
      //проверять ответ
      save("smth");
      break;
    }

  }
  return std::string();
}
void RegisterManager::save(const std::string &data) {

}

RegisterManager::Decrypted RegisterManager::deserializeDecrypted(const std::string &data) {
  pt::ptree request;
  Decrypted info;
  boost::iostreams::stream<boost::iostreams::array_source> stream(data.c_str(), data.size());
  try {
    boost::property_tree::read_json(stream, request);
    info.publicKey = request.get<std::string>("debug.public_key");
    info.serverString = request.get<std::string>("debug.server_string");
    info.clientString = request.get<std::string>("debug.client_string");
  }
  catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl; //TODO придумать как это обрабатывать
  }
  return info;
}
