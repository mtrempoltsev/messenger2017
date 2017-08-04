#pragma once

#include "Manager.h"

namespace m2 {
namespace server {

class RegisterManager : public Manager {
 public:

  virtual int doAction(const std::string &data, std::string &response) final;

 public:
  static const ResponseType m_response_type = ResponseType::Register;

 private:
  void save(const std::string &data);
  StringsPair deserialize(const std::string &data);

  std::string createResponse();
};
}
}
