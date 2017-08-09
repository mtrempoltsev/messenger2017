#include "stdafx.h"

#include "contact.h"
#include "core.h"
#include "core_dispatcher.h"
#include "jobtype.h"
#include "message.h"

namespace m2 {
namespace core {

using namespace safelog;

void CoreDispatcher::stopCore() { core_->Stop(); }
void CoreDispatcher::Login(LoginHandler handler) {
    JobType job = [handler](Core &core) {
      if (!core.GetHttpConnection() == nullptr) {
        if (core.InitHttpConnection()) {
          Error error;
          handler.onError(error);
          return;
        }
      }
      auto uuid = core.GetLoginManager()->Login(core.GetHttpConnection());
      if (!uuid.empty()) {
        handler.onComletion(uuid);
      }
      else {
        Error error;
        handler.onError(error);
      }
    };
    std::cout << "        push job" << std::endl;
    core.logger_(SL_DEBUG) << "push Login job";
    core_->PushJob(job);
}

void CoreDispatcher::RegisterUser(const std::string & serverDomain, RegisterHandler handler) {
    JobType job = [serverDomain, handler](Core &core) {
      if (core.InitHttpConnection(serverDomain)) {
        Error error;
        handler.onError(error);
        return;
      }
      int ret = core.GetLoginManager()->RegisterUser(core.GetHttpConnection());
      if (ret == 0) {
        handler.onCompletion();
      }
      else {
        handler.onError();
      }
    };
    std::cout << "        push job" << std::endl;
    core.logger_(SL_DEBUG) << "push Register job";
    core_->PushJob(job);
}

bool CoreDispatcher::HasServer() {
  return core_->HasChoosedServer();
}

std::list<std::string> CoreDispathcer::GetServerList() {
  return core_->GetLoginManager().GetServerList();
}

} // core
} // m2
