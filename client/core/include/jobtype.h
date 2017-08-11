#pragma once

#include <functional>
#include <memory>
namespace m2
{
   namespace core
   {
       class Core;
       using JobFunc = std::function<void(Core & core)>;
       using JobType = std::shared_ptr<JobFunc>;
   }
}

