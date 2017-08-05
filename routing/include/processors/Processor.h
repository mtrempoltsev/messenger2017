#pragma once
#include "data_structures/NodeInfo.h"

class Processor
{
public:
    Processor();
    ~Processor();

public:
    virtual void process(NodeInfo node_info, void* additional_data);

};

