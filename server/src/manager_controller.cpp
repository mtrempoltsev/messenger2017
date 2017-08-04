#include "manager_controller.h"
#include "Registration/RegisterManager.h"
#include "Registration/RegisterSendKeyManager.h"
#include "Auth/LoginManager.h"
#include "Auth/LoginSendKeyManager.h"

namespace m2 {
namespace server {

    ManagerController::ManagerController()
    {
    }

    responsePtr ManagerController::doProcess(requestPtr request)
    {
        responsePtr answer = std::make_shared<HttpResponse>();

        std::string uri = request->getHeader().uri_;
        std::string data = request->getData();
        std::string response;
        int code = 200;

        // http://localhost:8282/some/command1
        if (uri == "/user/register/sendKey") {
            // do something
            RegisterSendKeyManager sendKeyManager;
            code = sendKeyManager.doAction(data, response);
            answer->setData(response, code);
        }
        else if (uri == "/user/register") {
            // do something
            RegisterManager registerManager;
            code = registerManager.doAction(data, response);
            answer->setData(response, code);
        }
        else if (uri == "/user/auth/sendKey") {
            // do something
            LoginSendKeyManager sendKeyManager;
            code = sendKeyManager.doAction(data, response);
            answer->setData(response, code);
        }
        else if (uri == "/user/auth") {
            // do something
            LoginManager loginManager;
            code = loginManager.doAction(data, response);
            answer->setData(response, code);
        }
        else {
            code = 403;
            answer->setData(Manager::createError("Something wrong"), code);
            // do something
           // answer->setData(data, HttpResponse::HeaderType::NOT_FOUND);
        }

        return answer;
    }

}} // m2::server
