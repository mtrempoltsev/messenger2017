#pragma once

#include "contact.h"
#include "core.h"
#include "error.h"
#include "guid.h"
#include "handlers.h"
#include "message.h"

#include <functional>
#include <memory>
#include <vector>

namespace m2 {

// !!!!!!!!!!! MOVED TO client/shared/handlers.h

//  struct LoginHandler final {
//  using CompletionHandler = std::function<void(std::string uuid)>;
//  using ErrorHandler = std::function<void(Error&& error)>;

//  CompletionHandler onComletion;
//  ErrorHandler onError;
//};

// struct RegisterHandler final {
//  using CompletionHandler = std::function<void()>;
//  using ErrorHandler = std::function<void(Error&& error)>;

//  CompletionHandler onCompletion;
//  ErrorHandler onError;
//};

namespace core {
class CoreDispatcher final {
public:
  CoreDispatcher() = default;
  CoreDispatcher(CoreDispatcher &) = delete;
  CoreDispatcher &operator=(CoreDispatcher &) = delete;

  CoreDispatcher(CoreDispatcher &&) = delete;
  CoreDispatcher &operator=(CoreDispatcher &&) = delete;

  void Login(LoginHandler handler);
  void RegisterUser(RegisterHandler handler);

  void stopCore();

  std::shared_ptr<Core> core_;
};

} // core
} // m2
