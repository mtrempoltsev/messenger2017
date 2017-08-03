#include "manager_controller.h"
#include "RegisterManager.h"
#include "SendKeyManager.h"

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
            SendKeyManager sendKeyManager;
            code = sendKeyManager.doAction(data, response);
            answer->setData(response);
        }
        else if (uri == "/user/register") {
            // do something
            RegisterManager registerManager;
            code = registerManager.doAction(data, response);
            answer->setData(response);
        }
        else {
            // do something
           // answer->setData(data, HttpResponse::HeaderType::NOT_FOUND);
        }

        return answer;
    }

}} // m2::server
