
#include "Database.h"
namespace  m2
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
}
}