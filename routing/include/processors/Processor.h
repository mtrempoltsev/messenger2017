#pragma once
#include "data_structures/NodeInfo.h"

namespace m2
{
namespace routing
{

class Processor
{
public:
    Processor();
    ~Processor();

    virtual void process(NodeInfo node_info, void* additional_data) = 0;

};

} //namespace routing
} //namespace m2
