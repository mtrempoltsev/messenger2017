#include "RegisterManager.h"

using namespace m2::server;

RegisterManager::Decrypted
RegisterManager::deserializeDecrypted(const std::string &data) {
  pt::ptree request;
  Decrypted info;
  boost::iostreams::stream<boost::iostreams::array_source> stream(data.c_str(),
                                                                  data.size());
  try {
    boost::property_tree::read_json(stream, request);
    info.publicKey = request.get<std::string>("debug.public_key");
    info.serverString = request.get<std::string>("debug.server_string");
    info.clientString = request.get<std::string>("debug.client_string");
  } catch (const pt::ptree_error &e) {
    std::cout << e.what() << std::endl; // TODO придумать как это обрабатывать
  }
  return info;
}

int RegisterManager::doAction(const std::string &data, std::string &response) {
  Decrypted info = deserializeDecrypted(data);

  return 0;
}
void RegisterManager::save(const std::string &data) {}
