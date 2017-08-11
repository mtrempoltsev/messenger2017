#pragma once

#include <string>

namespace m2 {
struct Error final {

  enum class Code {
      NoError,
      NetworkError,
      LoginError,
      RegistationError
  };

  Error(Code code, std::string &&message) : code{code}, message{message} {}
  Error(Error &) = delete;
  Error &operator=(Error &) = delete;


    Error(Error &&) = default;
    Error &operator=(Error &&) = default;

    Code code;
    std::string message;
};
}
