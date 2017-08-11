
#include "Data/User/Users.h"

using namespace m2::data::user;

AUsers::AUsers(const std::string& RootDir)
    : root (RootDir)
    , users(RootDir + "Users")
    , cash (RootDir + "Keys/", 256)
{}

/****************************|  |****************************/

AUser AUsers::GetUser(uuids::uuid Uid) {
    // exists?
    checkR(users[Uid]) AUser();

    // get public key
    auto file = cash.ReadFile(Uid.str());
    checkR(file) AUser();

    return AUser(Uid, file->second);
}

bool AUsers::IsExists(uuids::uuid Uid) const
{ return users[Uid]; }

AUser AUsers::CreateUser(uuids::uuid Uid, const std::string& PublicKey
) {
    auto ptr = cash.WriteFile(Uid.str(), PublicKey);
    checkR(ptr) AUser();
    users.Add(Uid);

    return AUser(Uid, PublicKey);
}

AUser AUsers::CreateUser(uuids::uuid Uid, const std::string&& PublicKey
) {
    auto ptr = cash.WriteFile(Uid.str(), PublicKey);
    checkR(ptr) AUser();
    users.Add(Uid);

    return AUser(Uid, std::move(PublicKey));
}

/****************************|  |****************************/

const std::string& AUsers::RootDir() const
{ return root; }

const AUsers::UserUids& AUsers::Users() const
{ return users.Uids(); }

size_t AUsers::Size() const
{ return users.Uids().size(); }

bool AUsers::operator[](const uuids::uuid& Uid)
{ return users[Uid]; }

AUser AUsers::operator()(const uuids::uuid& Uid)
{ return GetUser(Uid); }
