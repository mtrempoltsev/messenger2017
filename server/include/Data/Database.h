#ifndef M2_SERVER_DATABASE_H
#define M2_SERVER_DATABASE_H
#include "stdafx.h"
#include "Data/Users.h"

namespace m2  {
namespace server
{

class Database
{
public:
    Database(const std::string rootDir);
    bool CreateUser(uuids::uuid uid, const std::string& publicKey);
    std::string getPublicServerKey();
    std::string getPrivateClient();
    std::string getUserPublicKey(uuids::uuid Uid);
private:
    Users users;
};
}}
#endif //M2_SERVER_DATABASE_H
