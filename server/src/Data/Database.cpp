
#include "Data/Database.h"

using namespace m2::server;


Database1::Database1(const std::string rootDir)
    : Users(rootDir)
    , Dialogs(rootDir + "Dialogs/")
{}

bool
Database1::CreateUser(uuids::uuid Uid, const std::string &PublicKey)
{ return Users.CreateUser(Uid, PublicKey) ? true : false; }

bool
Database1::CreateUser(uuids::uuid Uid, std::string &&PublicKey)
{ return Users.CreateUser(Uid, std::move(PublicKey)); }


bool
Database1::IsClienExists(uuids::uuid Uid)
{ return Users[Uid]; }


std::string
Database1::getUserPublicKey(uuids::uuid Uid)
{ return Users(Uid).PublicKey(); }

std::string
Database1::getPublicServerKey()
{ return ""; }

std::string Database1::getPrivateServerKey()
{ return ""; }
