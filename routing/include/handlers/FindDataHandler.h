#pragma once
#include "CommandHandler.h"

namespace m2 {
namespace routing {

class FindDataHandler : public CommandHandler
{

#pragma region __ Constructor and destructor __
public:
    FindDataHandler(Node* node);
    ~FindDataHandler();
#pragma endregion

};
        
} // namespace routing
} // namespace m2