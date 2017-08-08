#pragma once

#include <set>
#include <string>

namespace m2 {
namespace core {
class LoginManager {
 public:
  std::string Login() { return "123e4567-e89b-12d3-a456-426655440000"; }

  bool RegisterUser() { return 0; }

  std::set<std::string> GetServerSet();
};
}  // core
}  // m2
