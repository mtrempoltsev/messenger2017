#pragma once

#include "error.h"
#include "guid.h"
#include "jobtype.h"
#include "login_manager.h"

#include "contact.h"
#include "contact_manager.h"
#include "message.h"
#include "message_manager.h"

#include "http_client.h"
#include "http_connection.h"
#include "safe_log.h"

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

  // core <--> server
  boost::optional<m2::Error> StartServerConnection(const m2::Uuid &serverGuid);
  boost::optional<m2::Error> Login(const m2::Uuid &clientUuid);
  boost::optional<m2::Error> RegisterNewUser();

  std::map<m2::Uuid, std::string> GetServersMap();

  std::shared_ptr<ContactManager> GetContactManager();
  std::shared_ptr<LoginManager> GetLoginManager();
  std::shared_ptr<MessageManager> GetMessageManager();

  //http interface
  HttpClient & GetHttpClient() { return httpClient; }
  HttpConnectionPtr GetHttpConnection() { return httpConnection; }
  void SetHttpConnetion(HttpConnectionPtr httpCon) { httpConnection = httpCon; }
  bool InitHttpConnection(const std::string & serverDomain = std::string());

  // uber-threads
  void Start();
  void Stop();
  void JobLoop();
  void PushJob(JobType job, std::string && jobname);


  // managers
  std::shared_ptr<ContactManager> contactManager_;
  std::shared_ptr<LoginManager> loginManager_;
  std::shared_ptr<MessageManager> messageManager_;

  // map of availible servers
  std::map<m2::Uuid, std::string> serversMap_;

  // threads
  bool keepWorking_; // working flag
  std::mutex mutex_;
  std::condition_variable hasJob_;
  std::queue<JobType> jobQueue_;

  //http
  HttpClient httpClient;
  HttpConnectionPtr httpConnection;

  //logger
  safelog::SafeLog logger_;
};
} // core
} // m2