#include "core.h"

#include <iostream>
//#include <thread>

using namespace m2::core;

Core::Core() {
  contactManager_ = std::make_shared<ContactManager>();
  messageManager_ = std::make_shared<MessageManager>();
  loginManager_ = std::make_shared<LoginManager>();
}

void Core::Start() {
  std::cout << "Start()" << std::endl;
  keepWorking_ = true;
  JobLoop();
}

void Core::stopCore() {
  std::cout << "Stop core!" << std::endl;
  std::unique_lock<std::mutex> lock(mutex_);
  keepWorking_ = false;
  hasJob_.notify_one();
}

LoginManager* Core::getLoginManager() { return loginManager_.get(); }

ContactManager::ContactList Core::GetContactList(const std::string& contactId) {
  return contactManager_->GetContactList(contactId);
}

void Core::SaveContactList(const ContactManager::ContactList& contacts) {
  contactManager_->SaveContactList(contacts);
}

MessageManager::MessageStory Core::GetMessageStory(const std::string& id) {
  return messageManager_->GetMessageStory(id);
}

void Core::SaveMessageStory(const MessageManager::MessageStory& ms,
                            const std::string& contactId) {
  messageManager_->SaveMessageStory(ms, contactId);
}

boost::optional<m2::Error> Core::StartServerConnection(
    const m2::Uuid& serverGuid) {
  boost::optional<m2::Error> error;

  return error;
}

boost::optional<m2::Error> Core::RegisterNewUser() {}

void Core::StartLoop() {
  keepWorking_ = true;
  // std::thread loopThread(&Core::JobLoop, this);
  // loopThread.detach();
}

boost::optional<m2::Error> Core::Login(const m2::Uuid& clientUuid) {
  boost::optional<m2::Error> error;
  // TODO: send user uuid to the server and try to login
  return error;
}

void Core::JobLoop() {
  while (keepWorking_) {
    std::cout << "wait loop" << std::endl;
    JobType job;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      while (jobQueue_.empty()) {
        std::cout << "jobs empty" << std::endl;
        hasJob_.wait(lock);
        if (!keepWorking_) return;
      }

      job = jobQueue_.front();
      jobQueue_.pop();
    }
    std::cout << "call job" << std::endl;
    job(*this);
  }
}

void Core::PushJob(JobType job) {
  std::cout << "      push job" << std::endl;
  std::unique_lock<std::mutex> lock(mutex_);
  jobQueue_.push(job);
  hasJob_.notify_one();
}
