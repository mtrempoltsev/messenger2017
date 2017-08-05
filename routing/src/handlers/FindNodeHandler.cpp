#include "handlers/FindNodeHandler.h"

namespace m2 {
namespace routing {

FindNodeHandler::FindNodeHandler(Node * node):CommandHandler(node)
{
}

FindNodeHandler::~FindNodeHandler()
{
}

Message* FindNodeHandler::handleMessage(Message message)
{
//    Guid guid = message.getGuid(); //TODO
//    vector<NodeInfo> neighbours = node->kbucket_manager.getNeighbours(guid); //TODO
    Message* reply = new Message(/*parameters*/);
    //initialize reply with data
    return reply;
}
        
} // namespace routing
} // namespace m2