/**********************************
Creation:
	SafeLog logger() - creation without running
	SafeLog logger("/logs/connection.log") - creation with running
	
Reset:
	logger.reset("/logs/otherlog.log") - use this method when you need to change a logfile or to running logger if you have used default constructor
	
Write a message without labels:
	logger << "some message"
	
Write an error message:
	logger(SL_ERROR) << "some error"
	
Write a warning message:
	logger(SL_WARNING) << "some warning message"
	
Write a debug message:
	logger(SL_DEBUG) << "some debug message"
	
***********************************/

#pragma once

#include <fstream>
#include <queue>
#include <mutex>
#include <condition_variable>
namespace m2 {
namespace safelog {

#ifndef SL_ERROR
#define SL_ERROR SafeLog::MessageType::ERROR
#endif //SL_ERROR

#ifndef SL_WARNING
#define SL_WARNING SafeLog::MessageType::WARNING
#endif //SL_WARNING

#ifndef SL_DEBUG
#define SL_DEBUG SafeLog::MessageType::DEBUG
#endif //SL_DEBUG

  class SafeLog
  {
  public:

    enum class MessageType
    {
      ERROR,
      WARNING,
      DEBUG
    };
    
    SafeLog();
    SafeLog(const std::string & filePath);
    ~SafeLog();

    SafeLog & operator<<(const std::string & message);
    SafeLog & operator()(const MessageType & messageType);

    void reset(const std::string & filePath);

  private:

    class InnerSafeLog
    {
    public:
      InnerSafeLog();
      InnerSafeLog(const std::string & filePath);

      void reset(const std::string & filePath);
      void pushMessage(const std::string & message);
      void stop() { isRunning_ = false; hasMessage_.notify_one(); }

    private:
      bool isRunning_;

      std::ofstream logFile_;
      std::queue<std::string> messageQueue_;
      std::mutex mutex_;
      std::condition_variable hasMessage_;

      void mainLoop();
    }  *innerLog_;
    std::string getTimeAsString();
  };

}
}
