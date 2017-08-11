#ifndef M2_SERVER_USERS_H
#define M2_SERVER_USERS_H


#include "Data/index/IndexManager.hpp"
#include "Data/index/FileHandler.h"
#include "User.h"


namespace m2   {
namespace data {
namespace user {

    using namespace indices;

    /**
     *
     */
    class AUsers
    {
    public:

        typedef AIndexManager::LUids UserUids;

    public: /***************| Construction |***************/

        AUsers(const std::string& RootDir);

    public: /***************| Interface |***************/

        // get existing user, if not - AUser == false
        AUser GetUser (uuids::uuid Uid);
        bool  IsExists(uuids::uuid Uid) const;

        // add new user to database
        AUser CreateUser(uuids::uuid Uid, const std::string&  PublicKey);
        AUser CreateUser(uuids::uuid Uid, const std::string&& PublicKey);

        //migrate user

    protected: /************| Members |***************/

        std::string   root;
        AIndexManager users;
        AFileHandler  cash;

    public:

        const std::string& RootDir() const;
        const UserUids&    Users  () const;
        size_t             Size   () const;


    public: /***************| Operators |***************/

        AUser operator()(const uuids::uuid& Uid); // as GetUser
        bool  operator[](const uuids::uuid& Uid); // as IsExists

    private:
        AUsers(AUsers const&) = delete;
        AUsers& operator= (AUsers const&) = delete;
    };


}}}

#endif //M2_SERVER_USERS_H
