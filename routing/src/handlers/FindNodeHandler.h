#pragma once
#include "CommandHandler.h"
class FindNodeHandler : public CommandHandler
{

#pragma region __ Constructor and destructor __
public:
    FindNodeHandler(Node* node);
    ~FindNodeHandler();
#pragma endregion

};

