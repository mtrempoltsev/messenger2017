#pragma once
#include "handlers/CommandHandler.h"
#include "Processor.h"

class FindProcessor : CommandHandler, Processor
{
    
public:
    FindProcessor();
    ~FindProcessor();



private:




public:

    Message* handleMessage(Message message);


};

