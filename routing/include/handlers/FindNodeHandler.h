#pragma once
#include "CommandHandler.h"
class FindNodeHandler : CommandHandler
{


public:
    FindNodeHandler(Node* node);
    ~FindNodeHandler();




private:



public:

    virtual Message* handleMessage(Message message);





};

