#pragma once
#include "CommandHandler.h"
class FindNodeHandler : public CommandHandler
{

public:
    FindNodeHandler(Node* node);
    ~FindNodeHandler();


private:
    Message* handleMessage(Message message);

public:




};

