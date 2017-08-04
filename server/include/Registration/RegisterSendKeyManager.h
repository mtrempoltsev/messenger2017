#pragma once

#include "Manager.h"
#include "tuple"

#include <openssl/rsa.h>

namespace m2 {
namespace server {

class RegisterSendKeyManager : public Manager {
public:
  virtual int doAction(const std::string &data, std::string &response) final;

public:
  static const ResponseType m_response_type = ResponseType::SendKey;

protected:
  std::string deserialize(const std::string &data);

private:
  std::string createResponse(const std::string &publicKey);

  std::string rsaEncrypt(RSA *pubKey, const std::string str) const;

  std::unique_ptr<RSA> createRSAWithFilename(std::string filename, bool isKeyPublic);///< temporary function for testing
  std::unique_ptr<RSA> createRSAWithPublicKey(const std::string &key);
};
}
}
