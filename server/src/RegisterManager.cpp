#include "RegisterManager.h"

using namespace m2::server;

RegisterManager::StringsPair
RegisterManager::deserializeDecrypted(const std::string &data) {
  pt::ptree request;
  StringsPair info;
  std::stringstream stream;
  stream << data;

  boost::property_tree::read_json(stream, request);
  info.serverString = request.get<std::string>("server_string");
  info.clientString = request.get<std::string>("client_string");

  return info;
}

int RegisterManager::doAction(const std::string &data, std::string &response) {
  StringsPair info;
  try {
    info = deserializeDecrypted(data);
  } catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl;
    response = createError("client_string and decrypted server_string");
    return 403;
  }
  response = createResponse(info.serverString, info.clientString);
  //TODO save
  return 200;
}
std::string RegisterManager::createResponse(const std::string &server_string, const std::string &client_string) {
  std::string session_id = "test"; //TODO
  pt::ptree tree;
  std::stringstream stream;
  tree.put("session_id", session_id);
  boost::property_tree::write_json(stream, tree);
  return stream.str();
}
void RegisterManager::save(const std::string &data) {}
