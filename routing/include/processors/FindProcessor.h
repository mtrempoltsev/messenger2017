#pragma once
#include <boost/uuid/uuid.hpp>
#include <list>
#include <vector>

#include "handlers/CommandHandler.h"
#include "data_structures/NodeInfo.h"
#include "Processor.h"

using boost::uuids::uuid;
using std::vector;
using std::list;

class FindProcessor : CommandHandler, Processor
{
    
public:
    FindProcessor();
    ~FindProcessor();


public:
    virtual Message* handleMessage(Message message);
    void process(uuid guid);

protected:
    list<> not_asked;
    list<NodeInfo>
};

