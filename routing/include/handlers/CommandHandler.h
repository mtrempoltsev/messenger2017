#pragma once
#include "core/Node.h"
#include "data_structures/Message.h"


/// Command handler is an abstract class for all handlers of incoming messages
class CommandHandler
{
public:
    CommandHandler(Node* node);
    ~CommandHandler();

public:
    virtual Message* handleMessage(Message message);


private:
    Node* node;

};

