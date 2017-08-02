#include "../include/manager_controller.h"


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

        // http://localhost:8282/some/command1
        if (uri == "/some/command1") {
            // do something
            answer->setData(data);
        }
        else if (uri == "/some/command2") {
            // do something
            answer->setData(data);
        }
        else {
            // do something
            answer->setData(data, HttpResponse::HeaderType::NOT_FOUND);
        }

        return answer;
    }

}} // m2::server
