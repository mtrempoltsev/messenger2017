#pragma once
#include "handlers/CommandHandler.h"
#include "pr"

class FindProcessor : CommandHandler, 
{
    
public:
    FindProcessor();
    ~FindProcessor();

#pragma region __ Methods __

private:
#pragma region __ Private methods __

#pragma endregion

public:
#pragma region __ Public methods __
    Message* handleMessage(Message message);
#pragma endregion  
#pragma endregion
};

