
#include "Data/Database.h"

using namespace m2::server;


Database::Database(const std::string& rootDir)
    : Users(rootDir)
    , Dialogs(rootDir + "Dialogs/")
{}

bool
Database::CreateUser(uuids::uuid Uid, const std::string &PublicKey)
{ return Users.CreateUser(Uid, PublicKey) ? true : false; }

bool
Database::CreateUser(uuids::uuid Uid, std::string &&PublicKey)
{ return Users.CreateUser(Uid, std::move(PublicKey)); }


bool
Database::IsClienExists(uuids::uuid Uid)
{ return Users[Uid]; }


std::string
Database::getUserPublicKey(uuids::uuid Uid)
{ return Users(Uid).PublicKey(); }

std::string
Database::getPublicServerKey()
{ return ""; }

std::string Database::getPrivateClient()
{ return ""; }
