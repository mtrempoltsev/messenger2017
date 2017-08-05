#pragma once

#include <string>
#include <boost/uuid/uuid.hpp> //how to include it more correctly?


namespace m2 {

    struct NodeInfo
    {
        boost::uuids::uuid uuid;
        std::string ip;
        int port;
    };
}

