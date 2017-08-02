#include "../stdafx.h"

#include <boost/preprocessor/repetition/repeat.hpp>

#include "Data/Data.hpp"

using namespace m2::data;



#define USR_CREATE(z, n, data) Users.CreateUser(n, "__PUBLIC_KEY");


int main(int argc, char* argv[])
{
    std::string Root = "/home/kvilt/CLionProjects/messenger2017/server/Test/";

    // create Users
    AUsers Users(Root);
    BOOST_PP_REPEAT(15, USR_CREATE,);

    // print all users
    for (auto& i : Users.Users())
        std::cout << i << "\t" << Users.GetUser(i).PublicKey() << std::endl;

    // check, if a user exists
    std::cout << "\nWat's about user 156?" << std::endl;
    if (Users[156]) std::cout << "Something was wrong"   << std::endl;
    else            std::cout << "The user does't exist" << std::endl;
}
