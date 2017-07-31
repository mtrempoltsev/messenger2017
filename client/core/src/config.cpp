#include "../include/config.h"

#include <iostream>
#include <string>

using boost::property_tree::write_json;
using m2::Uuid;
using namespace m2::core;

/* PUBLIC METHODS */

Config::Config() : isValid_(false) {
  // здесь специально ничего не происходит
}

/** ***********************************
 *  fill config from json string
 *  @author Alexander
 **************************************/
ConfigError Config::ReadFromString(std::string jsonString) {
  std::istringstream iss(jsonString);
  if (iss.good()) return Read(iss);
  return ConfigError::InvalidString;
}

/** ***********************************
 * Open File and call Read function
 *
 * @author Vladimir
 **************************************/
ConfigError Config::ReadFromFile(const std::string& jsonFileName) {
  std::ifstream fs;
  fs.open(jsonFileName);
  if (fs.is_open()) {
    return Read(fs);
  }
  return ConfigError::InvalidFile;
}

/**************************************
 * Open File for writing and call Write function
 *
 * by Vladimir
 **************************************/
ConfigError Config::WriteToFile(const std::string& fileName) {
  ConfigError result = ConfigError::InvalidFile;
  std::ofstream fs(fileName);  // try to open file
  if (fs.is_open()) {
    result = Write(fs);
    fs.close();
  }
  return result;
}

/***********************************************************************
        return current Config state (fields) as Json String
        Note: if isValid_ == false, method returns an empty string
By Alexander
************************************************************************/
std::string Config::ToJsonString() {
  std::string jsonString;
  if (isValid_) {
    std::ostringstream oss;
    if (Write(oss) != ConfigError::WriteError) return oss.str();
  }

  return jsonString;
}

ConfigError Config::Write(std::ostream& jsonStream) {
  ConfigError error = ConfigError::NoErrors;
  return error;
};
/* PRIVATE METHODS */

/** ***********************************
 *
 *  @author Alexander & Neilana
 **************************************/
ConfigError Config::Read(std::istream& jsonStream) {
  ConfigError error = ConfigError::NoErrors;

  boost::property_tree::read_json(jsonStream, propertiesTree_);

  std::string emptyString = std::string();

  std::string bufServerGuid = propertiesTree_.get<std::string>("ServerGuid", emptyString);
  std::string bufClientGuid = propertiesTree_.get<std::string>("ClientGuid", emptyString);
  std::string bufServerName = propertiesTree_.get<std::string>("ServerName", emptyString);

  if ((bufServerGuid != emptyString) && (bufClientGuid != emptyString) && (bufServerName != emptyString)) {
    fields_.serverGuid_.set(bufServerGuid);
    fields_.clientGuid_.set(bufClientGuid);
    fields_.serverName_ = bufServerName;

    isValid_ = true;
  } else {
    isValid_ = false;
    error = ConfigError::InvalidFields;
  }

  return error;
}
