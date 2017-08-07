#pragma once

#include <functional>
namespace m2
{
   namespace core
   {
       class Core;
       using JobType = std::function<void (Core& core)>;
   }
}

