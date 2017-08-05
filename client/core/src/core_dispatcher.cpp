#include "stdafx.h"

#include "contact.h"
#include "core.h"
#include "core_dispatcher.h"
#include "jobtype.h"
#include "message.h"

namespace m2 {
namespace core {

void CoreDispatcher::stopCore() { core_->stopCore(); }
void CoreDispatcher::login(LoginHandler handler) {
  JobType job = [handler](Core &core) {
    std::string uuid = core.getLoginManager()->login();
    handler.onComletion(uuid);
  };
  std::cout << "        push job" << std::endl;
  core_->PushJob(job);
}

void CoreDispatcher::registerUser(RegisterHandler handler) {
  JobType job = [handler](Core &core) {
    int ret = core.getLoginManager()->registerUser();
    if (ret == 0) {
      handler.onCompletion();
    } else {
      ;
      //      hjandler.onError();
    }
  };
  std::cout << "        push job" << std::endl;
  core_->PushJob(job);
}

}  // core
}  // m2
