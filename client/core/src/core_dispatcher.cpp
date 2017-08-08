#include "stdafx.h"

#include "contact.h"
#include "core.h"
#include "core_dispatcher.h"
#include "jobtype.h"
#include "message.h"

namespace m2 {
namespace core {

void CoreDispatcher::stopCore() { core_->Stop(); }
void CoreDispatcher::Login(LoginHandler handler) {
  JobType job = [handler](Core &core) {
    std::string uuid = core.GetLoginManager()->Login();
    handler.onCompletion(uuid);
  };
  std::cout << "        push job" << std::endl;
  core_->PushJob(job);
}

void CoreDispatcher::RegisterUser(RegisterHandler handler) {
  JobType job = [handler](Core &core) {
    int ret = core.GetLoginManager()->RegisterUser();
    if (ret == 0) {
      handler.onCompletion();
    } else {
      ;
      //      handler.onError();
    }
  };
  std::cout << "        push job" << std::endl;
  core_->PushJob(job);
}

void CoreDispatcher::GetServerSet(ServerSetHandler handler) {
  JobType job = [handler](Core &core) {
    std::set<std::string> servers = core.GetLoginManager()->GetServerSet();
    if (servers.empty()) {
      handler.onCompletion(servers);
    } else {
      ;
      //      handler.onError();
    }
  };
  std::cout << "        push job get server list" << std::endl;
  core_->PushJob(job);
}
}  // core
}  // m2
