#pragma once

#include "string"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

namespace pt = boost::property_tree;

enum  ResponseType
{
  SendKey,
  Register,
  INVALID,
  COUNT = INVALID,
};

namespace m2 {
namespace server {


class Manager
{
 public:
  struct Error{
    int code;
    std::string jsonBody;
  };
public:
  virtual int doAction (const std::string &data, std::string &response) = 0;
  static const ResponseType m_response_type = ResponseType::INVALID;
 protected:
  std::string createError(const std::string message);
 };


}
}

