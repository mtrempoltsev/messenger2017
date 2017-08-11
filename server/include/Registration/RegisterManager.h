#pragma once

#include "Manager.h"


namespace m2
{
namespace server
{

class RegisterManager: public Manager
{

public:
    RegisterManager(ManagerController* controller);

    virtual HttpResponse::Code doAction(const std::string &data, std::string &response) final;

public:
    static const ResponseType m_response_type = ResponseType::Register;

private:
    StringsPair deserialize(const std::string &data);


    std::string createResponse (const StringsPair &pair, userInfo &result);
};
}
}
