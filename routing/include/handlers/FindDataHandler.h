#pragma once
#include "handlers/CommandHandler.h"

namespace m2 {
namespace routing {

class FindDataHandler : public CommandHandler
{


public:
    FindDataHandler(Node* node);
    ~FindDataHandler();


};

} // namespace routing
} // namespace m2
