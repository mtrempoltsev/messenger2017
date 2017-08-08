#include "login_manager.h"
#include "path_settings.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <fstream>

using namespace m2::core;
using namespace boost::property_tree;

std::set<std::string> LoginManager::GetServerSet() {
  std::set<std::string> serversSet;

  // FIXME : GetManagerPath
  // std::ifstream stream(GetManagerPath().append("server_list.json"));
  std::ifstream stream("server_list.json");

  if (stream.is_open()) {
    ptree pt;
    try {
      read_json(stream, pt);
      for (auto& iter : pt.get_child("servers")) {
        // serversSet.push_back(iter.second.get<std::string>(""));  // wtf
      }
    } catch (json_parser_error& ex) {
      // log_(SL_ERROR) << ex.what();
    }
  } else {
    // log_(SL_ERROR) << "file did not opened!";
  }
  return serversSet;
}
