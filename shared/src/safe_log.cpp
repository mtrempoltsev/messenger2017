#include "../include/safe_log.h"

#include <thread>
#include <string>
#include <map>

using namespace m2::safelog;

static std::map<SafeLog::MessageType, std::string> labelNameMap =
{
  { SafeLog::MessageType::ERROR, "[Error]: "},
  { SafeLog::MessageType::WARNING, "[Warning]: " },
  { SafeLog::MessageType::DEBUG, "[Debug]: " },
};

SafeLog::InnerSafeLog::InnerSafeLog(const std::string & filePath) :
  isRunning_(false)
{
  reset(filePath);
}

void SafeLog::InnerSafeLog::reset(const std::string & filePath)
{
  if (logFile_.is_open())
    logFile_.close();
  logFile_.open(filePath);
  if (logFile_.is_open())
  {
    isRunning_ = true;
    std::thread thread(std::bind(&InnerSafeLog::mainLoop, this));
    thread.detach();
  }
}

void SafeLog::InnerSafeLog::pushMessage(const std::string & message)
{
  std::unique_lock<std::mutex> lock(mutex_);
  messageQueue_.push(message);
  hasMessage_.notify_one();
}

void SafeLog::InnerSafeLog::mainLoop()
{
  while (isRunning_)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (messageQueue_.empty()) {
      hasMessage_.wait(lock);
      if (!isRunning_) break;
    }
    logFile_ << messageQueue_.front();
    messageQueue_.pop();
  }
  logFile_.close();
  delete this;
}

SafeLog::SafeLog(const std::string & filePath)
{
  innerLog_ = new InnerSafeLog(filePath);
}

SafeLog::~SafeLog()
{
  innerLog_->stop();
}

void SafeLog::reset(const std::string & filePath)
{
  innerLog_->reset(filePath);
}

SafeLog & SafeLog::operator<<(const std::string & message)
{
  innerLog_->pushMessage(message + "\n\n");
  return *this;
}

SafeLog & SafeLog::operator()(const MessageType & messageType)
{
  innerLog_->pushMessage(labelNameMap[messageType]);
  return *this;
}