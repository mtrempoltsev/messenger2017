#ifndef M2_SERVER_DATA_TEST_H
#define M2_SERVER_DATA_TEST_H


#include <boost/preprocessor/repetition/repeat.hpp>

#include "Data.hpp"

using namespace m2::data;

/*****************/

int N = 0;

#define Set(n) N = n

/*****************/

#define USR_CREATE(z, n, data) Users.CreateUser(n, "__PUBLIC_KEY" );

#define out std::cout
#define end std::endl

#define nof(n)  "block " << n << "/" << N
#define OK      "\t - OK \t: "

#define BLOCK(n, txt) out << nof(n) << OK << txt << end


void DataTest()
{
    Set(3);

    // test folder
    std::string Root = "/home/kvilt/CLionProjects/messenger2017/server/Test/";

    {   // create Users
        AUsers Users(Root);
        BOOST_PP_REPEAT(15, USR_CREATE,);

        // added 15. Check it
        assert(Users.Users().size() == 15);

        // unknown user
        assert(!Users[156]);

    } BLOCK(1, "first creation");


    {   // create again
        AUsers Users(Root);

        // all, we added, must be here
        for (auto i = 0; i < 15; ++i)
            assert(Users[i]);

        // must be 15. Check it
        assert(Users.Users().size() == 15);

    } BLOCK(2, "second creation");


    {   // messages test
        AUsers   users(Root);
        ADialogs dialogs(Root + "Dialogs/");

        auto Alice = users[1];
        auto Bob   = users[2];

        /***************/

        // create user_dialogs for new user
        assert(!dialogs(Alice));
        auto user_dialog = dialogs[Alice];
        assert(user_dialog);

        // check, we took right user
        assert(user_dialog->Uid() == Alice.Uid());

        /***************/

        // create dialog with new talker
        assert(!(*user_dialog)(Bob));
        auto dialog = (*user_dialog)[Bob];
        assert(dialog);

        // check, we have right dialog
        assert(dialog->Uid() == Bob.Uid());

    } BLOCK(3, "static message test");


}





/**/
#endif //M2_SERVER_DATA_TEST_H
