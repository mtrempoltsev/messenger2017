#ifndef M2_SERVER_USERS_H
#define M2_SERVER_USERS_H

#include <unordered_set>

#include "../Index/IndexManager.hpp"
#include "../Index/FileHandler.h"
#include "User.h"


namespace m2   {
namespace data {
namespace user {

    using namespace indices;

    /**
     * .check user: if (AUsers[Uid]) //AUser have "operator bool()"
     *
     */
    class AUsers
    {
    public:

        typedef AIndexManager::LUids LUsers;

    public: /***************| Construction |***************/
      static AUsers& instance()
      {
        // согласно стандарту, этот код ленивый и потокобезопасный
        static AUsers s("./");
        return s;
      }


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
        const LUsers&      Users  () const;


    public: /***************| Operators |***************/
      bool  operator[](const uuids::uuid& Uid);
     private:
      AUsers(const std::string& RootDir);
      AUsers(AUsers const&) = delete;
      AUsers& operator= (AUsers const&) = delete;
      AUser operator()(const uuids::uuid& Uid);
    };


}}}

#endif //M2_SERVER_USERS_H
