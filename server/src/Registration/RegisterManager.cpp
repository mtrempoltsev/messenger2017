#include "Registration/RegisterManager.h"

using namespace m2::server;
#include "../Data/User/Users.h"

RegisterManager::StringsPair RegisterManager::deserialize(const std::string &data) {
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
    info = deserialize(data);

    response = createResponse();
  } catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl;
    response = createError("client_string and decrypted server_string");
    return 403;
  }
  response = createResponse();
  //m2::data::user::AUsers users;

  //TODO save
  return 200;
}
std::string RegisterManager::createResponse() {
  pt::ptree tree;
  std::stringstream stream;
  boost::property_tree::write_json(stream, tree); //empty tree
  return stream.str();
}
void RegisterManager::save(const std::string &data) {}
