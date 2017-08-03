#include "SendKeyManager.h"


using namespace m2::server;

std::string SendKeyManager::deserializePublicKey(const std::string &data) {
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

int SendKeyManager::doAction(const std::string &data, std::string &response) {
  std::string temp = deserializePublicKey(data);
  std::cout << "public_key:" << temp << std::endl;

  //тут создавать ответ с зашифрованным ключом

  return 0;
  // Decrypted info = deserializeDecrypted(data);
}
void SendKeyManager::save(const std::string &data)
{

}
