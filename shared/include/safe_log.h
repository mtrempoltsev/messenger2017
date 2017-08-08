#pragma once

#include <fstream>
#include <queue>
#include <mutex>

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

    SafeLog(const std::string & filePath);
    ~SafeLog();

    SafeLog & operator<<(const std::string & message);
    SafeLog & operator()(const MessageType & messageType);

    void reset(const std::string & filePath);

  private:

    class InnerSafeLog
    {
    public:
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

  };

}
}