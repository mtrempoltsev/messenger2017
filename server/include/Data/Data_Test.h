#ifndef M2_SERVER_DATA_TEST_H
#define M2_SERVER_DATA_TEST_H

#include <istream>

#include <boost/preprocessor/repetition/repeat.hpp>

#include "Data.hpp"




///*****************

int N = 0;

#define Set(n) N = n

///*****************

#define USR_CREATE(z, n, data) Users.CreateUser(n, "__PUBLIC_KEY" );

#define nof(n)  "block " << n << "/" << N
#define OK      "\t - OK \t: "

#define BLOCK(n, txt) std::cout << nof(n) << OK << txt << std::endl

namespace fs = boost::filesystem;
using namespace m2::data;


void DataTest()
{
    Set(4);

    // test folder
    std::string Root = "/home/kvilt/CppProjects/messenger2017/server/Test/";
    boost::filesystem::remove_all(Root);

    {   // create Users
        AUsers Users(Root);
        //BOOST_PP_REPEAT(15, USR_CREATE,);
        Users.CreateUser(1, "__PUBLIC_KEY 1" );
        Users.CreateUser(2, "__PUBLIC_KEY 2" );
        Users.CreateUser(3, "__PUBLIC_KEY 3" );

        // added 15. Check it
        assert(Users.Size() == 3);

        // unknown user
        assert(!Users[156]);

    } BLOCK(1, "first creation");


    {   // create again
        AUsers Users(Root);

        // all, we added, must be here
        for (auto i = 1; i < 4; ++i)
            assert(Users[i]);

        // must be 15. Check it
        assert(Users.Users().size() == 3);

    } BLOCK(2, "second creation");


    {   // messages test
        AUsers   users(Root);
        ADialogs dialogs(Root + "Dialogs/");

        auto Alice = users(1);
        auto Bob   = users(2);

        ///***************

        // create user_dialogs for new user
        assert(!dialogs[Alice]);
        auto user_dialog = dialogs(Alice);
        assert(user_dialog);

        // check, we have a right user
        assert(*user_dialog == Alice);

        ///***************

        // create dialog with a new talker
        assert(!(*user_dialog)[Bob]);
        auto dialog = (*user_dialog)(Bob);
        assert(dialog);

        // check, we have a right dialog
        assert(*dialog == Bob);

        ///***************

        // add 2 messages to Alice's chat
        auto Id1 = dialog->AddMessage("__TEST_TEXT 1");
        auto Id2 = dialog->AddMessage("__TEST_TEXT 2");

        // were they added?
        assert(fs::exists(dialog->Root() + Id1.str()));
        assert(fs::exists(dialog->Root() + Id2.str()));

        // remove first message
        dialog->DeleteMessage(Id1);
        assert(!fs::exists(dialog->Root() + Id1.str()));

    } BLOCK(3, "static message test");

    { // alternative dialog getting
        AUsers   users(Root);
        ADialogs dialogs(Root + "Dialogs/");

        auto Alice = users(1);
        auto Bob   = users(2);

        ///***************

        auto dialog = dialogs.Get(Alice, Bob);
        assert(dialog);


    } BLOCK(4, "existing dialog taking");
}




/**/
#endif //M2_SERVER_DATA_TEST_H
