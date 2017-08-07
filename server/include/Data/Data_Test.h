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

#define out std::cout
#define end std::endl

#define nof(n)  "block " << n << "/" << N
#define OK      "\t - OK \t: "

#define BLOCK(n, txt) out << nof(n) << OK << txt << end

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
        Users.CreateUser(0, "__PUBLIC_KEY" );
        Users.CreateUser(1, "__PUBLIC_KEY" );
        Users.CreateUser(2, "__PUBLIC_KEY" );

        // added 15. Check it
        //assert(Users.Users().size() == 3);

        // unknown user
        assert(!Users[156]);

    } BLOCK(1, "first creation");


    {   // create again
        AUsers Users(Root);

        // all, we added, must be here
        for (auto i = 0; i < 3; ++i)
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
        //                         ss.mm.hh dd.mm.yyyy
        const std::string data1 = "01.36.15 03.08.2017";
        const std::string data2 = "56.36.15 03.08.2017";

        // add 2 messages to Alice's chat
        dialog->AddMessage(data2, "__TEST_TEXT");
        dialog->AddMessage(data1, "__TEST_TEXT");

        // were they added?
        assert(fs::exists(dialog->Root() + data1 + "^2"));
        assert(fs::exists(dialog->Root() + data2 + "^2"));

        // remove first message
        dialog->DeleteMessage(data1);
        assert(!fs::exists(dialog->Root() + data1 + "^2"));

    } BLOCK(3, "static message test");

    { // alternative dialog getting
        AUsers   users(Root);
        ADialogs dialogs(Root + "Dialogs/");

        auto Alice = users(1);
        auto Bob   = users(2);

        ///***************

        auto dialog = dialogs.Get(Alice, Bob);
        assert(dialog);


    } BLOCK(4, "forward dialog taking");
}




/**/
#endif //M2_SERVER_DATA_TEST_H
