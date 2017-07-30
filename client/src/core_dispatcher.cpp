#include "../core_dispatcher.h"

m2::CoreDispatcher& m2::CoreDispatcher::instance()
{
    static CoreDispatcher instance;
    return instance;
}
