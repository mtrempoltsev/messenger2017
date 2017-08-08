#ifndef M2_SERVER_DATABASE_H
#define M2_SERVER_DATABASE_H

#include "Data.hpp"


namespace m2     {
namespace server {


    class Database
    {
    public:

        Database(const std::string& rootDir);

    public: /****************| Users |****************/

        bool CreateUser(uuids::uuid Uid, const std::string&  PublicKey);
        bool CreateUser(uuids::uuid Uid,       std::string&& PublicKey);

        bool IsClienExists(uuids::uuid Uid);

    public: /****************| Keys |****************/

        std::string getUserPublicKey(uuids::uuid Uid);

        std::string getPublicServerKey();
        std::string getPrivateServerKey();

    public: /****************| Sessions |****************/

        Session* GetSession(const uuids::uuid& Uid);
        bool     AddSession(const uuids::uuid& Uid, Session* ptr);
        void  DeleteSession(const uuids::uuid& Uid);

    public: /****************| Messages |*****************/

        data::DialogPtr
        GetDialog(const uuids::uuid& User
                , const uuids::uuid& Addressee);

        data::UserDialogsPtr
        GetUserDialog(const uuids::uuid& User);

        const uuids::uuid&
        StoreMessage(const uuids::uuid& Sender
                   , const uuids::uuid& Addressee
                   , const std::string& Text);

    protected:

        data::AUsers    Users;
        data::ADialogs  Dialogs;
        data::ASessions Sessions;

    };


}}


#endif //M2_SERVER_DATABASE_H
