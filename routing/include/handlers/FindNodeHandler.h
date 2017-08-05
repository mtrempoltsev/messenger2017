#pragma once
#include "handlers/CommandHandler.h"

namespace m2 {
namespace routing {

class FindNodeHandler : public CommandHandler
{

public:
    FindNodeHandler(Node* node);
    ~FindNodeHandler();


private:
    Message* handleMessage(Message message);

public:




};

} // namespace m2
} // namespace routing
