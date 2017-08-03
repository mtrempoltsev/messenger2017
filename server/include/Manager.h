#pragma once

#include "string"
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

namespace pt = boost::property_tree;

enum class ResponseType
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
  virtual int doAction (const std::string &data, std::string &response) = 0;
  static const ResponseType m_response_type = ResponseType::INVALID;
};


}
}

