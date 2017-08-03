#include "SendKeyManager.h"

using namespace m2::server;

std::string SendKeyManager::deserializePublicKey(const std::string &data) {
  pt::ptree request;
  std::string publicKey;
  std::stringstream stream;
  stream << data;
  boost::property_tree::read_json(stream, request);
  publicKey = request.get<std::string>("public_key");

  return publicKey;
}

int SendKeyManager::doAction(const std::string &data, std::string &response) {
  std::string publicKey;
  try {
    publicKey = deserializePublicKey(data);
  }
  catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl;
    response = createError("Unknown algorithm");
    return 403;
  }

  return 200;
}
std::string SendKeyManager::createResponse(const std::string &publicKey) {
  std::string server_string = "test"; //TODO
  std::string client_string = "test2"; //TODO
  pt::ptree tree;
  std::stringstream stream;
  tree.put("server_string", server_string);
  tree.put("client_string", client_string);
  boost::property_tree::write_json(stream, tree);
  return stream.str();
}
