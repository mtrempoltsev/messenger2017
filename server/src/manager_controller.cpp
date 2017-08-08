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
        managerProcessor = {
            {"/user/register/sendKey", new RegisterSendKeyManager(this)},
            {"/user/register", new RegisterManager(this)},
            {"/user/auth/sendUuid", new LoginSendKeyManager(this)},
            {"/user/auth", new LoginManager(this)},
        };
    }

    responsePtr ManagerController::doProcess(requestPtr request)
    {
        responsePtr answer = std::make_shared<HttpResponse>();

        std::string uri = request->getHeader().uri_;
        std::string data = request->getData();
        std::string response;
        HttpResponse::Code code = HttpResponse::Code::OK;

        if (managerProcessor.count(uri) > 0) {
            managerProcessor.at(uri)->doAction(data, response);
        }
        else {
            code = HttpResponse::Code::NOT_FOUND;
            response = Manager::createError("Something wrong");
        }
        
        std::cout << "RESPONSE: " << response << std::endl;

        // http://localhost:8282/some/command1
        // if (uri == "/user/register/sendKey") {

        //     RegisterSendKeyManager sendKeyManager(db);
        //     code = sendKeyManager.doAction(data, response);
        // }
        // else if (uri == "/user/register") {

        //     RegisterManager registerManager(db);
        //     code = registerManager.doAction(data, response);
        // }
        // else if (uri == "/user/auth/sendUuid") {

        //     LoginSendKeyManager sendKeyManager(db);
        //     code = sendKeyManager.doAction(data, response, &userUid);
        // }
        // else if (uri == "/user/auth") {

        //     LoginManager loginManager(db);
        //     code = loginManager.doAction(data, response);
        // }
        // else {
        //     code = HttpResponse::Code::NOT_FOUND;
        //     response = Manager::createError("Something wrong");
        // }

        answer->setData(response, code);

        return answer;
    }

    ManagerController::~ManagerController()
    {
        db->DeleteSession(userUid);
    }

}} // m2::server
