#include "stdafx.h"

#include "contact.h"
#include "core.h"
#include "core_dispatcher.h"
#include "jobtype.h"
#include "message.h"
#include "error.h"

namespace m2 {
namespace core {

using namespace safelog;

void CoreDispatcher::stopCore() { core_->Stop(); }
void CoreDispatcher::Login(LoginHandler handler) {
    JobType job = [handler](Core &core) {
      if (core.GetHttpConnection() == nullptr) {
        if (core.InitHttpConnection()) {
          handler.onError(Error(Error::Code::NetworkError, "Connection error"));
          return;
        }
      }
      auto uuid = core.GetLoginManager()->Login(core.GetHttpConnection());
      if (!uuid.empty()) {
        handler.onComletion(uuid);
      }
      else {
        handler.onError(Error(Error::Code::LoginError, "Login error"));
      }
    };
    core_->PushJob(job, "Login");
}

void CoreDispatcher::RegisterUser(const std::string & serverDomain, RegisterHandler handler) {
    JobType job = [serverDomain, handler](Core &core) {
      if (!core.InitHttpConnection(serverDomain)) {
        handler.onError(Error(Error::Code::NetworkError, "Connection error"));
        return;
      }
      m2::Error ret = core.GetLoginManager()->RegisterUser(core.GetHttpConnection());
      if (ret.code == m2::Error::Code::NoError) {
        handler.onCompletion();
      }
      else {
        handler.onError(std::move(ret));
      }
    };
    std::cout << "        push job" << std::endl;
    core_->PushJob(job, "Registation");
}

bool CoreDispatcher::HasServer() {
  return core_->HasChosenServer();
}

std::list<std::string> CoreDispatcher::GetServerList() {
  return core_->GetLoginManager()->GetServerList();
}

} // core
} // m2
