#pragma once

#include "Manager.h"
#include "tuple"

namespace m2 {
namespace server {

class RegisterManager : public Manager {
 public:
  struct StringsPair { // неудачное название, переименуйте на что-нибудь более
    // понятное
    std::string serverString;
    std::string clientString;
  };

  virtual int doAction(const std::string &data, std::string &response) final;

 public:
  static const ResponseType m_response_type = ResponseType::Register;

 protected:
  StringsPair deserializeDecrypted(const std::string &data);

 private:
  void save(const std::string &data);
  std::string createResponse(const std::string &server_string,const std::string &client_string);
};
}
}
