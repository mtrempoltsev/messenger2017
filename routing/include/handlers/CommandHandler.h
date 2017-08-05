#pragma once
#include "Node.h"
#include "Message.h"

namespace m2 {
namespace routing {

/// Command handler is an abstract class for all handlers of incoming messages
class CommandHandler
{

#pragma region __ Fields __ 
private:
    Node* node;
#pragma endregion


#pragma region __ Constructor and destructor __
public:
    CommandHandler(Node* node);
    ~CommandHandler();
#pragma endregion


#pragma region __ Methods __

private:
#pragma region __ Private methods __
#pragma endregion

public:
#pragma region __ Public methods __
    virtual Message* processMessage(Message message);
#pragma endregion  
#pragma endregion

};
        
} // namespace routing
} // namespace m2