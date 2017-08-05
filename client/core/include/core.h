#pragma once

#include "config.h"
#include "contact.h"
#include "error.h"
#include "guid.h"
#include "jobtype.h"
#include "loginmanager.h"
#include "message.h"

#include <boost/optional.hpp>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <queue>

namespace m2 {
namespace core {

class Core {
 public:
  Core();
  std::map<m2::Uuid, std::string> GetServersMap();

  // core <--> server
  boost::optional<m2::Error> StartServerConnection(const m2::Uuid& serverGuid);
  boost::optional<m2::Error> Login(const m2::Uuid& clientUuid);
  boost::optional<m2::Error> RegisterNewUser();
  void StartLoop();
  // core <-> GUI
  ContactManager::ContactList GetContactList(const std::string& contactId);
  void SaveContactList(const ContactManager::ContactList& contacts);

  MessageManager::MessageStory GetMessageStory(const std::string& id);
  void SaveMessageStory(const MessageManager::MessageStory& ms,
                        const std::string& contactId);

  LoginManager* getLoginManager();
  void Start();

  // private:
  // std::shared_ptr<CoreDispatcher> dispatcher_;

  // make list (actually, map) of availible servers
  std::map<m2::Uuid, std::string> ReadServersFile();
  std::shared_ptr<ContactManager> contactManager_;
  std::shared_ptr<LoginManager> loginManager_;
  std::shared_ptr<MessageManager> messageManager_;

  std::map<m2::Uuid, std::string> serversMap_;
  m2::core::Config config_;
  void JobLoop();

  std::shared_ptr<ContactManager> GetContactManager() {
    return contactManager_;
  }
  void PushJob(JobType job);

  bool keepWorking_;  // working flag
  std::mutex mutex_;
  std::condition_variable hasJob_;
  std::queue<JobType> jobQueue_;
};
}  // core
}  // m2
