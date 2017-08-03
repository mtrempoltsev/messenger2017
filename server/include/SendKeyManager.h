#pragma once

#include "Manager.h"
#include "tuple"

namespace m2 {
namespace server {

class SendKeyManager : public Manager {
public:
  virtual int doAction(const std::string &data, std::string &response) final;

public:
  static const ResponseType m_response_type = ResponseType::SendKey;

protected:
  std::string deserializePublicKey(const std::string &data);

private:
  void save(const std::string &data);
};
}
}
