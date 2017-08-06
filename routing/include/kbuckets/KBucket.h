#pragma once
#include "data_structures/NodeInfo.h"
#include <list>

namespace m2 {
namespace routing {

class KBucket final
{
public:
    KBucket();
    
    bool insert(const m2::routing::NodeInfo& src);
    void replaceOldest(const m2::routing::NodeInfo& src);

    NodeInfo oldest() const;
    
    std::list<m2::routing::NodeInfo> known() const { return nodes; }
    size_t knownCnt() const { return filled; }

protected:
    std::list<m2::routing::NodeInfo> nodes;
    size_t filled; // NOTE: list::size O(capacity) implementation is allowed 
    size_t k;
};
    
} // namespace routing
} // namespace m2