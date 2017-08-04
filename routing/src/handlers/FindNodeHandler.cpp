#include "handlers/FindNodeHandler.h"



FindNodeHandler::FindNodeHandler(Node * node):CommandHandler(node)
{
}

FindNodeHandler::~FindNodeHandler()
{
}

Message* FindNodeHandler::handleMessage(Message message)
{
    Guid guid = message.getGuid();
    std::vector<NodeInfo> neighbours = node->kbucket_manager.getNeighbours(guid);
    Message* reply = new Message(/*parameters*/);
    //initialize reply with data
    return reply;
}
