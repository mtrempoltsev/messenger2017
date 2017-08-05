#pragma once
//#include "CommandHandler.h"
#include "Message.h"

namespace m2 {
namespace routing {

class Node //friend class CommandHandler
{
  //Validator validator;
 public:
  Node();
  ~Node();

 private:
  DHT dht;
  KBucketManager kbucketManager;


};

} // namespace routing
} // namespace m2
