#pragma once

namespace m2 {

struct LoginHandler final {
  using CompletionHandler = std::function<void(std::string uuid)>;
  using ErrorHandler = std::function<void(Error &&error)>;

  CompletionHandler onComletion;
  ErrorHandler onError;
};

struct RegisterHandler final {
  using CompletionHandler = std::function<void()>;
  using ErrorHandler = std::function<void(Error &&error)>;

  CompletionHandler onCompletion;
  ErrorHandler onError;
};
}
