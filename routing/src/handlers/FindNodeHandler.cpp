#include "FindNodeHandler.h"



FindNodeHandler::FindNodeHandler(Node * node):CommandHandler(node)
{
}

FindNodeHandler::~FindNodeHandler()
{
}

Message* FindNodeHandler::processMessage(Message message)
{
    Guid guid = message.getGuid();
    vector<NodeInfo> neighbours = node->kbucket_manager.getNeighbours(guid);
    Message* reply = new Message(/*parameters*/);
    //initialize reply with data
    return reply;
}
