
#ifndef M2_SERVER_DATABASE_H
#define M2_SERVER_DATABASE_H
#include "string"
#include "stdafx.h"
namespace m2  {
namespace server
{

class Database
{
public:
    Database(const std::string rootDir);
    bool CreateUser(uuids::uuid Uid, const std::string&& PublicKey);
    bool CreateUser(uuids::uuid Uid, const std::string& PublicKey);



};
}}


#endif //M2_SERVER_DATABASE_H
