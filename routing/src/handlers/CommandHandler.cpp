#include "handlers/CommandHandler.h"

namespace m2 {
namespace routing {

CommandHandler::CommandHandler(Node* node)
{
    this->node = node;
}

CommandHandler::~CommandHandler()
{
}

    Message *CommandHandler::processMessage(Message message) {
        return nullptr;
    }

} // namespace routing
} // namespace m2
