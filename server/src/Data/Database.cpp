#include "stdafx.h"

#include "Database.h"
namespace m2
{
namespace server
{

Database::Database(const std::string rootDir)
{

}
bool Database::CreateUser(uuids::uuid Uid, const std::string &&PublicKey)
{
    return true;
}
bool Database::CreateUser(uuids::uuid Uid, const std::string &PublicKey)
{
    return true;
}
std::string Database::getPublicServerKey()
{
    return std::string();
}
std::string Database::getPrivateClient()
{
    return std::string();
}
std::string Database::getUserPublicKey(uuids::uuid Uid)
{
    return std::string();
}
}
}
