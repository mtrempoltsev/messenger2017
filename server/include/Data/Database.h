
#ifndef M2_SERVER_DATABASE_H
#define M2_SERVER_DATABASE_H

#include "Data.hpp"

#include "Data2/Database.h"

#include "stdafx.h"



#define DATA_BASE 1



namespace m2     {
namespace server {

    class Database1
    {
    public:

        Database1(const std::string rootDir);

    public:

        bool CreateUser(uuids::uuid Uid, const std::string&  PublicKey);
        bool CreateUser(uuids::uuid Uid,       std::string&& PublicKey);

        bool IsClienExists(uuids::uuid Uid);

    public:

        std::string getUserPublicKey(uuids::uuid Uid);

        std::string getPublicServerKey();
        std::string getPrivateServerKey();

    protected:

        data::AUsers   Users;
        data::ADialogs Dialogs;

    };


#if DATA_BASE == 2
        typedef server::Database2   Database;
#else
        typedef Database1           Database;
#endif

}}


#endif //M2_SERVER_DATABASE_H
