#include "manager_controller.h"
#include "Registration/RegisterManager.h"
#include "Registration/RegisterSendKeyManager.h"
#include "Auth/LoginManager.h"
#include "Auth/LoginSendKeyManager.h"
#include "iostream"

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

            RegisterSendKeyManager sendKeyManager;
            code = sendKeyManager.doAction(data, response);
            std::cout<<"REQUEST: "<<response<<std::endl;
        }
        else if (uri == "/user/register") {

            RegisterManager registerManager;
            code = registerManager.doAction(data, response);
        }
        else if (uri == "/user/auth/sendKey") {

            LoginSendKeyManager sendKeyManager;
            code = sendKeyManager.doAction(data, response);
        }
        else if (uri == "/user/auth") {

            LoginManager loginManager;
            code = loginManager.doAction(data, response);
        }
        else {
            code = 403;
            response = Manager::createError("Something wrong");
        }

        answer->setData(response, code);

        return answer;
    }

}} // m2::server
