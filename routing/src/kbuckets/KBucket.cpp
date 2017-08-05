#include "KBucket.h"
#include <stdexcept>

namespace m2 {
namespace routing {
    
KBucket::KBucket():
    k(16) // load from config instead
{}

bool KBucket::insert(const m2::routing::NodeInfo& src)
{
    if (filled < k) {
       nodes.push_back(src);
       ++filled;
       return true;
    }
    
    return false;
}

void KBucket::replaceOldest(const m2::routing::NodeInfo& src)
{
    if (!filled) {
        throw std::runtime_error("KBucket::replaceOldest: empty bucket");
    }

    nodes.back() = src;
}

NodeInfo KBucket::oldest() const
{
    if (!filled) {
        throw std::runtime_error("KBucket::Oldest: empty bucket");
    }

    return nodes.back();
}

} // namespace routing
} // namespace m2
