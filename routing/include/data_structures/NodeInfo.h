#pragma once
#include <string>
#include <boost/uuid/uuid.hpp> //how to include it more correctly?

namespace m2 {
namespace routing {
struct NodeInfo
{
    boost::uuids::uuid uuid;
    std::string ip;
    int port;
};

} // namespace routing
} // namespace m2
