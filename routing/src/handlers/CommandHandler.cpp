#include "handlers/CommandHandler.h"
#include "core/Node.h"

namespace m2 {
namespace routing {

CommandHandler::CommandHandler(Node* node)
{
    this->node = node;
}

CommandHandler::~CommandHandler()
{
}

Message *CommandHandler::handleMessage(Message message) {
    return nullptr;
}

} // namespace routing
} // namespace m2
