#include "../stdafx.h"
#include "../include/server.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

#include "Data/Data_Test.h"

int main(int argc, char* argv[])
{
    using namespace m2::server;
    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    uint16_t port = 8282;
    if (argc > 1) {
        try {
            port = lexical_cast<uint16_t>(argv[1]);
        }
        catch(bad_lexical_cast &) {
            std::cout << "Error port argument! Use default - 8282" << std::endl;
        }
    }

    std::cout << "Connected to port = " << port << std::endl;

    Server server;
    server.start(port);

    return 0;
}

