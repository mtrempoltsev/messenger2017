#pragma once

//#include <list>

namespace m2 {

struct LoginHandler final {
  using CompletionHandler = std::function<void(std::string uuid)>;
  using ErrorHandler = std::function<void(Error &&error)>;

  CompletionHandler onCompletion;
  ErrorHandler onError;
};

struct RegisterHandler final {
  using CompletionHandler = std::function<void()>;
  using ErrorHandler = std::function<void(Error &&error)>;

  CompletionHandler onCompletion;
  ErrorHandler onError;
};

struct ServerSetHandler final {
  using CompletionHandler = std::function<void(std::set<std::string> servers)>;
  using ErrorHandler = std::function<void(Error &&error)>;

  CompletionHandler onCompletion;
  ErrorHandler onError;
};
}
