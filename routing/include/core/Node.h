#pragma once
//#include "CommandHandler.h"
#include "Message.h"

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

