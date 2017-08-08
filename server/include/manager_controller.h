#pragma once

#include "http_request.h"
#include "http_response.h"
#include "Data/stdafx.h"

namespace m2     {
namespace server {

    class Database;
    class Session;

    class ManagerController
    {
    public:
        ManagerController(Database* database, Session* session);
        ~ManagerController();

        responsePtr doProcess(requestPtr request);

    private:
        Database* db;
        Session* session_;

        uuids::uuid userUid;
    };


}} // m2::server
