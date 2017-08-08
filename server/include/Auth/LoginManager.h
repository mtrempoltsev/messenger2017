#pragma once
#include "Manager.h"

namespace m2
{
namespace server
{
class LoginManager: public Manager
{
public:
    LoginManager(ManagerController* controller);
    virtual HttpResponse::Code
    doAction(const std::string &data, std::string &response) final;

private:
    StringsPair deserialize(const std::string &data);

    std::string createResponse(const StringsPair &pair, userInfo &result);
};
}
}

