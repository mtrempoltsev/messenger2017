#pragma once

#include "Manager.h"


namespace m2
{
namespace server
{

class RegisterManager: public Manager
{
    struct userInfo
    {
        uuids::uuid fingerprint;
        std::string clientPublicKey;
    };
public:
    RegisterManager(Database *db);

    virtual HttpResponse::Code doAction(const std::string &data, std::string &response) final;

public:
    static const ResponseType m_response_type = ResponseType::Register;

private:
    StringsPair deserialize(const std::string &data);



    response_result createResponse (const StringsPair &pair, userInfo &result);
};
}
}
