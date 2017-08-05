#pragma once
#include "core/Node.h"
#include "utils/Config.h"
#include "data_structures/Message.h"

#include <list>

/// Command handler is an abstract class for all handlers of incoming messages
class CommandHandler
{


protected:
    Node* node;




public:
    CommandHandler(Node* node);
    ~CommandHandler();





private:



public:

    virtual Message* handleMessage(Message message);





};

