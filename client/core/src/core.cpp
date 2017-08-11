#include "core.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
//#include <thread>
#include "path_settings.h"

using namespace m2::core;

const std::string chosenFilePath = "/home/vova/chosen_server.txt";

Core::Core() : logger_(GetManagerPath("logs").append("core.log")) {
  contactManager_ = std::make_shared<ContactManager>();
  messageManager_ = std::make_shared<MessageManager>();
  loginManager_ = std::make_shared<LoginManager>();
}

/********* MANAGERS *******/
std::shared_ptr<ContactManager> Core::GetContactManager() { return contactManager_; }

std::shared_ptr<LoginManager> Core::GetLoginManager() { return loginManager_; }

std::shared_ptr<MessageManager> Core::GetMessageManager() { return messageManager_; }

boost::optional<m2::Error> Core::StartServerConnection(const m2::Uuid &serverGuid) {
    boost::optional<m2::Error> error;

    return error;
}

/********* CORE <--> SERVER *********/
boost::optional<m2::Error> Core::RegisterNewUser() {}

boost::optional<m2::Error> Core::Login(const m2::Uuid &clientUuid) {
    boost::optional<m2::Error> error;
    // TODO: send user uuid to the server and try to login
    return error;
}

/********* THREADS *********/
void Core::Start() {
  std::cout << "Start()" << std::endl;
  logger_(SL_INFO) << "Start()";
  keepWorking_ = true;
  JobLoop();
}

void Core::Stop() {
  std::cout << "Stop core!" << std::endl;
  logger_(SL_INFO) << "Stop core!";
  std::unique_lock<std::mutex> lock(mutex_);
  keepWorking_ = false;
  hasJob_.notify_one();
}

void Core::JobLoop() {
  while (keepWorking_) {
    std::cout << "wait loop" << std::endl;
    logger_(SL_INFO) << "wait loop";
    JobType job;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      while (jobQueue_.empty()) {
        std::cout << "jobs empty" << std::endl;
        logger_(SL_INFO) << "jobs empty";
        hasJob_.wait(lock);
        if (!keepWorking_)
          return;
      }
      std::cout << "call job" << std::endl;
      job = jobQueue_.front();
      jobQueue_.pop();
      (*job)(*this);
    }
  }
}

void Core::PushJob(JobType job, std::string && jobname) {
  std::cout << "      push job" << std::endl;
  logger_(SL_INFO) << "push job with name " + jobname;
  std::unique_lock<std::mutex> lock(mutex_);
  jobQueue_.push(job);
  hasJob_.notify_one();
}

/**********HTTP INTERFACE*****************/
bool Core::InitHttpConnection(const std::string &serverDomain) {
  std::string validServerDomain = serverDomain.empty() ? loginManager_->GetServerDomain() : serverDomain;
  if (httpConnection == nullptr)
    httpConnection = httpClient.connect(validServerDomain);
  if (httpConnection != nullptr) {
    loginManager_->SetServerDomain(serverDomain);
  }
  else {
    logger_(SL_ERROR) << "The Connection does not exist!";
  }
  return httpConnection != nullptr;
}
