#include "Data/Database.h"
namespace m2
{
namespace server
{

Database::Database(const std::string rootDir)
{

}

bool Database::CreateUser(uuids::uuid uid, const std::string &publicKey)
{
    bool result = users.CreateUser(uid, publicKey);
    return result;
}
std::string Database::getPublicServerKey()
{
    return std::__cxx11::string();
}
std::string Database::getPrivateClient()
{
    return std::__cxx11::string();
}
std::string Database::getUserPublicKey(uuids::uuid Uid)
{
    return std::__cxx11::string();
}
}
}