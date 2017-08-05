#pragma once
<<<<<<< HEAD
#include "core/Node.h"
#include "data_structures/Message.h"
=======
#include "Node.h"
#include "Message.h"
>>>>>>> upstream/master

namespace m2 {
namespace routing {

/// Command handler is an abstract class for all handlers of incoming messages
class CommandHandler
{
public:
    CommandHandler(Node* node);
    ~CommandHandler();

public:
    virtual Message* handleMessage(Message message);

<<<<<<< HEAD

private:
    Node* node;

=======
>>>>>>> upstream/master
};
        
} // namespace routing
} // namespace m2