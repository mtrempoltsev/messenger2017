#include "Data2/Database.h"

namespace m2
{
namespace server
{

Database2::Database2(const std::string rootDir)
{}

bool Database2::CreateUser(uuids::uuid uid, const std::string &publicKey)
{
    bool result = users.CreateUser(uid, publicKey);
    return result;
}
std::string Database2::getPublicServerKey()
{
    return std::string();
}
std::string Database2::getPrivateClient()
{
    return std::string();
}
std::string Database2::getUserPublicKey(uuids::uuid Uid)
{
    return std::string();
}

}
}
