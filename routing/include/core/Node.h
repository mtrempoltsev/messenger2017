#pragma once
#include <string>
#include <unordered_map>

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"

#include "handlers/CommandHandler.h"
#include "handlers/FindNodeHandler.h"
#include "handlers/FindDataHandler.h"

#include "processors/FindDataProcessor.h"
#include "processors/FindNodeProcessor.h"

#include "data_structures/Message.h"
#include "kbuckets/KBucketsManager.h"
#include "NetworkConnector/NetworkConnector.h"
#include "DHT/DHT.h"

using std::string;
using boost::uuids::uuid;

namespace m2 {
namespace routing {


class Node
{
public:
    Node(string port) {};
    ~Node();


    void start(uuid bootstrap_guid, string ip_address, string port);
        // void stop();
private:
    void onMessageReceive(char* buffer, size_t size);

    DHT dht;
    std::size_t void a();
    NodeInfo selfInfo;
    NetworkConnector networkConnector;
    KBucketsManager kbucketsManager;
    std::unordered_map<MessageType, CommandHandler, MessageTypeHash> handlers;

    friend class PingProcessor;
    friend class StoreProcessor;
    friend class FindNodeProcessor;
    friend class FindDataProcessor;

    friend class PingHandler;
    friend class StoreHandler;
    friend class FindNodeHandler;
    friend class FindDataHandler;
};

} // namespace routing
} // namespace m2
