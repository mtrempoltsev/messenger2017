#ifndef M2_SERVER_DATABASE2_H
#define M2_SERVER_DATABASE2_H

#include "stdafx.h"

#include "Data2/Users.h"


namespace m2  {
namespace server
{


    class Database2
    {
    public:
        Database2(const std::string rootDir);
        bool CreateUser(uuids::uuid uid, const std::string& publicKey);
        std::string getPublicServerKey();
        std::string getPrivateClient();
        std::string getUserPublicKey(uuids::uuid Uid);
    private:
        Users users;
    };


}}
#endif //M2_SERVER_DATABASE2_H
