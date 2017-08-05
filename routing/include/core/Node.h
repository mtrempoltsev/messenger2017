#pragma once

#include "data_structures/Message.h"
#include "kbuckets/KBucketsManager.h"

namespace m2 {
namespace routing {

class Node //friend class CommandHandler
{
  //Validator validator;
 public:
  Node();
  ~Node();

 private:
//  DHT dht; //TODO
  KBucketsManager kBucketsManager;


};

} // namespace routing
} // namespace m2
