#include "Data2/Users.h"
namespace m2
{
namespace server
{
Users::Users()
{

}
bool Users::CreateUser(uuids::uuid Uid, const std::string &PublicKey)
{
    if(!users.count(Uid)) {
        users.insert({Uid, PublicKey});
        return  true;
    }
    return false;
}

}
}
