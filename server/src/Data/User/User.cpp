#include "Data/User/User.h"

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

bool AUser::IsValid() const
{ return public_key.size() > 0; }

/****************************|  |****************************/

const std::string& AUser::PublicKey() const
{ return public_key; }

uuids::uuid AUser::Uid() const
{ return uid; }

/****************************|  |****************************/

AUser::operator bool() const
{ return IsValid(); }

AUser::operator uuids::uuid() const
{ return uid; }

AUser::operator std::string() const
{ return public_key; }

/****************************|  |****************************/


