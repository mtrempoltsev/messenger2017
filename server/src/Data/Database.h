
#ifndef M2_SERVER_DATABASE_H
#define M2_SERVER_DATABASE_H
#include "string"
namespace m2  {
namespace server
{

class Database
{
public:
    Database(const std::string rootDir);
    bool CreateUser(uuids::uuid Uid, const std::string&& PublicKey);
    bool CreateUser(uuids::uuid Uid, const std::string& PublicKey);
    std::string getPublicServerKey();
    std::string getPrivateClient();
    std::string getUserPublicKey(uuids::uuid Uid);
};
}}


#endif //M2_SERVER_DATABASE_H
