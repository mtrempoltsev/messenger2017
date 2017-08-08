#pragma once

#include "http_request.h"
#include "http_response.h"


namespace m2     {
namespace server {

    class Database;
    class Session;

    class ManagerController
    {
    public:
        ManagerController(Database* database, Session* session);

        responsePtr doProcess(requestPtr request);

    private:
        Database* db;
        Session* session_;
    };


}} // m2::server
