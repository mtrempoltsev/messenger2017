#pragma once

#include "Manager.h"
#include "tuple"

#include <openssl/rsa.h>

namespace m2
{
namespace server
{

class RegisterSendKeyManager: public Manager
{
public:
    RegisterSendKeyManager(Database *database);
    virtual HttpResponse::Code doAction(const std::string &data, std::string &response) final;

public:
    static const ResponseType m_response_type = ResponseType::SendKey;

protected:
    std::string deserialize(const std::string &data);

private:
    std::string createResponse(const std::string &publicKey);

    std::string rsaEncryptPublic(RSA *pubKey, const std::string str) const;
    std::string rsaEncryptPrivate(RSA *pubKey, const std::string str) const;

    RSA *createRSAWithFilename(std::string filename, bool isKeyPublic);///< temporary function for testing
    RSA *createRSAWithPublicKey(const std::string &key);
    RSA *createRSAWithPrivateKey(const std::string &key);

};
}
}
