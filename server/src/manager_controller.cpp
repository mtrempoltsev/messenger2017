#include "manager_controller.h"
#include "Registration/RegisterManager.h"
#include "Registration/RegisterSendKeyManager.h"
#include "Auth/LoginManager.h"
#include "Auth/LoginSendKeyManager.h"
#include "iostream"

namespace m2 {
namespace server {

   ManagerController::ManagerController(Database *database): db(database)
   {

   }
    responsePtr ManagerController::doProcess(requestPtr request)
    {
        responsePtr answer = std::make_shared<HttpResponse>();

        std::string uri = request->getHeader().uri_;
        std::string data = request->getData();
        std::string response;
        HttpResponse::Code code = HttpResponse::Code::OK;

        // http://localhost:8282/some/command1
        if (uri == "/user/register/sendKey") {

            RegisterSendKeyManager sendKeyManager(db);
            code = sendKeyManager.doAction(data, response);
            std::cout<<"REQUEST: "<<response<<std::endl;
        }
        else if (uri == "/user/register") {

            RegisterManager registerManager(db);
            code = registerManager.doAction(data, response);
        }
        else if (uri == "/user/auth/sendKey") {

            LoginSendKeyManager sendKeyManager(db);
            code = sendKeyManager.doAction(data, response);
        }
        else if (uri == "/user/auth") {

            LoginManager loginManager(db);
            code = loginManager.doAction(data, response);
        }
        else {
            code = HttpResponse::Code::NOT_FOUND;
            response = Manager::createError("Something wrong");
        }

        answer->setData(response, code);

        return answer;
    }

}} // m2::server
