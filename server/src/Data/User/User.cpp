#include "User.h"

using namespace m2::data::user;


AUser::AUser()
    : uid(0)
{}

AUser::AUser(uuids::uuid Uid, const std::string& PubKey)
    : public_key(PubKey)
    , uid(Uid)
{}

AUser::AUser(uuids::uuid Uid, std::string&& PubKey)
        : public_key(std::move(PubKey))
        , uid(Uid)
{}

/****************************|  |****************************/

const std::string& AUser::PublicKey() const
{ return public_key; }

uuids::uuid AUser::Uid() const
{ return uid; }

bool AUser::IsValid()
{ return uid && public_key.size(); }

AUser::operator bool()
{ return IsValid(); }

/****************************|  |****************************/


