#pragma once
#include "handlers/CommandHandler.h"

namespace m2 {
namespace routing {

class FindProcessor :
    public CommandHandler
{
public:
    FindProcessor();
    ~FindProcessor();
};
        
} // namespace routing
} // namespace m2