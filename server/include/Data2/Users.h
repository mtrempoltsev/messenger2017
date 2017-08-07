#ifndef M2_SERVER_USERS_H_H
#define M2_SERVER_USERS_H_H
#include "stdafx.h"
#include "unordered_map"

namespace m2
{
namespace server
{
class Users
{
public:
    Users();

    bool CreateUser(uuids::uuid Uid, const std::string &PublicKey);
private:
    std::unordered_map<uuids::uuid, std::string, uuids::KeyHasher> users;
};
}
}

#endif //M2_SERVER_USERS_H_H
