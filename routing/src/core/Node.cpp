#include <handlers/FindDataHandler.h>
#include <handlers/FindNodeHandler.h>
#include "core/Node.h"

using namespace m2::routing;

Node::Node(string port)
    : networkConnector(std::stoi(port),
                       [this] (char* buffer, size_t size) {
                           this->onMessageReceive(buffer, size);
                       })
    , dht()
{
    // create own uuid
    uuid my_guid = boost::uuids::basic_random_generator()();

    // put int kbucket selfInfo
    selfInfo = {my_guid, networkConnector.getMyIpAddress(), std::stoi(port)};
    kbucketsManager.setNodeInfo(selfInfo);

    handlers = {std::make_pair(MessageType::PingRequest,     PingNodeHandler(this)),
                std::make_pair(MessageType::StoreRequest,    StoreHandler(this)),
                std::make_pair(MessageType::FindNodeRequest, FindNodeHandler(this)),
                std::make_pair(MessageType::FindDataRequest, FindDataHandler(this)),

                std::make_pair(MessageType::PingResponse,     PingNodeProcessor(this)),
                std::make_pair(MessageType::StoreResponse,    StoreProcessor(this)),
                std::make_pair(MessageType::FindNodeResponse, FindDataProcessor(this)),
                std::make_pair(MessageType::FindDataResponse, FindNodeProcessor(this)),
    };
};

Node::~Node()
{
}

} // namespace routing
} // namespace m2
