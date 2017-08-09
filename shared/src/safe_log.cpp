#include "../include/safe_log.h"

#include <thread>
#include <string>
#include <map>
#include <chrono>
#include <ctime>

using namespace m2::safelog;
using namespace std::chrono;

static std::map<SafeLog::MessageType, std::string> labelNameMap =
{
  { SafeLog::MessageType::ERROR, "[Error]: "},
  { SafeLog::MessageType::WARNING, "[Warning]: " },
  { SafeLog::MessageType::DEBUG, "[Debug]: " },
};

SafeLog::InnerSafeLog::InnerSafeLog() :
  isRunning_(false)
{
  
}

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

SafeLog::SafeLog()
{
  innerLog_ = new InnerSafeLog();
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
  innerLog_->pushMessage("[" + getTimeAsString() + "]" + labelNameMap[messageType]);
  return *this;
}

std::string SafeLog::getTimeAsString()
{
  system_clock::time_point p = system_clock::now();
  std::time_t t = system_clock::to_time_t(p);
  std::string timestr = std::ctime(&t);
  return std::string(timestr.begin(), timestr.end() - 1);
}
