#pragma once

#include "http_request.h"
#include "http_response.h"
#include "Data/stdafx.h"

namespace m2     {
namespace server {

    class Database;
    class Session;
    class Manager;

    class ManagerController
    {
    public:
        ManagerController(Database* database, Session* session);
        ~ManagerController();

        responsePtr doProcess(requestPtr request);

        Database* getDB() {return db;}
        uuids::uuid& getUuid() {return userUid;}

    private:
        Database* db;
        Session* session_;

        uuids::uuid userUid;

        std::map<std::string, Manager*> managerProcessor;
    };


}} // m2::server
