#ifndef M2_SERVER_LOGINMANAGER_H
#define M2_SERVER_LOGINMANAGER_H
#include "Manager.h"

namespace m2
{
namespace server
{
class LoginManager: public Manager
{
public:
    LoginManager(Database *database);
    virtual HttpResponse::Code
    doAction(const std::string &data, std::string &response) final;

private:
    StringsPair deserialize(const std::string &data);

    std::string createResponse(const std::string &server_string, const std::string &client_string);
};
}
}
#endif //M2_SERVER_LOGINMANAGER_H
