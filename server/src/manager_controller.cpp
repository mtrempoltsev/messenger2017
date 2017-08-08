#include "manager_controller.h"
#include "Registration/RegisterManager.h"
#include "Registration/RegisterSendKeyManager.h"
#include "Auth/LoginManager.h"
#include "Auth/LoginSendKeyManager.h"

namespace m2 {
namespace server {

    ManagerController::ManagerController(Database *database, Session* session)
    : db(database)
    , session_(session)
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
            std::cout<<"RESPONSE: "<<response<<std::endl;
        }
        else if (uri == "/user/register") {

            RegisterManager registerManager(db);
            code = registerManager.doAction(data, response);
        }
        else if (uri == "/user/auth/sendUuid") {

            LoginSendKeyManager sendKeyManager(db);
            code = sendKeyManager.doAction(data, response, userUid);
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

    ManagerController::~ManagerController()
    {
        db->DeleteSession(userUid);
    }

}} // m2::server
