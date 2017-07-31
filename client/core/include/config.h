#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <string>

#include "guid.h"

namespace m2 {
namespace core {
// possible errors that can occur while reading configuration form file or stream
enum class ConfigError {
  NoErrors,       ///< no errors occured
  InvalidFile,    ///< error when trying to open a file
  InvalidString,  ///< error when reading from a stream
  InvalidJson,    ///< invalid JSON format
  InvalidFields,  ///< configuration field contains invalid data
  WriteError      ///< error while writing config into a file/stream
};
class Config {
  // additional class for possible fields in the config file
  class ConfigFields {
   public:
    m2::Uuid serverGuid_;
    m2::Uuid clientGuid_;
    std::string serverName_;
  } fields_;

 public:
  Config();
  bool IsValid() { return isValid_; }

  // read/write functions
  ConfigError ReadFromString(std::string jsonString);         ///< update current configuration from JSON-formatted string
  ConfigError ReadFromFile(const std::string &jsonFileName);  ///< update current configuration from JSON-formatted file
  ConfigError WriteToFile(const std::string &fileName);       ///< write current configuration into JSON-formatted file
  ConfigError Write(std::ostream &jsonStream);
  std::string ToJsonString();

  // server guid getters
  const m2::Uuid GetServerGuid() const { return fields_.serverGuid_; }

  // client guid getters
  const m2::Uuid GetClientGuid() const { return fields_.clientGuid_; }

  // server guid setters
  void SetServerGuid(const std::string &guid);
  void SetServerGuid(const m2::Uuid &guid);

  // client guid setters
  void SetClientGuid(const std::string &guid);
  void SetClientGuid(const m2::Uuid &guid);

 private:
  ConfigError Read(std::istream &jsonStream);  ///< update current configuration from JSON-formatted stream

  //  m2::Uuid serverGuid_;
  //  m2::Uuid clientGuid_;
  //  std::string serverName_;

  // ConfigFields fields_;

  bool isValid_;  ///< is current configuration correct
  boost::property_tree::ptree propertiesTree_;

  std::string fileName_;
};
}
}

// class ConfigFields {
// public:
//  m2::Uuid serverGuid_;
//  m2::Uuid clientGuid_;
//  std::string serverName_;
//}

// enum class FieldsNames {
// serverGuid;
// clientGuid;
// serverName;
//}

// map <FieldsNames, std::string> fieldToName;
// fieldToName[FieldsName::ServerGuid] = "ServerGUID"
// fieldToName[FieldsName::ClientGuid] = "ClientGUID"

// for (auto fieldToName : a)
