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
    LoginSendKeyManager(Database *database);
    virtual HttpResponse::Code doAction(const std::string &data, std::string &response) final;

protected:
    std::string deserialize(const std::string &data);

private:
    std::string createResponse(const std::string &publicKey);
};
}
}

#endif //M2_SERVER_LOGINSENDKEY_H
