#ifndef M2_SERVER_LOGINSENDKEY_H
#define M2_SERVER_LOGINSENDKEY_H
#include "Manager.h"
namespace m2
{
namespace server
{
class LoginSendKeyManager: public Manager
{
public:
    LoginSendKeyManager(ManagerController* controller);
    virtual HttpResponse::Code doAction(const std::string &data, std::string &response) final;
protected:
    uuids::uuid deserialize(const std::string &data);

private:
    std::string createResponse(const uuids::uuid &uuid);

    ManagerController* controller;
};
}
}

#endif //M2_SERVER_LOGINSENDKEY_H
