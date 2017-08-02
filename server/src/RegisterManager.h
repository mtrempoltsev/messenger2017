#ifndef M2_SERVER_REGISTERMANAGER_H
#define M2_SERVER_REGISTERMANAGER_H
#include "Manager.h"
#include "tuple"

namespace m2 {
namespace server {
class RegisterManager : public Manager {
 public:
  enum kindAction{
    createEncrypted,
    registerUser
  };
  struct Decrypted{ // неудачное название, переименуйте на что-нибудь более понятное
    std::string publicKey;
    std::string serverString;
    std::string clientString;
  };
 public:
  std::string doAction(const std::string &data, kindAction kind);

 protected:
  std::string deserializePublicKey(const std::string &data);
  Decrypted deserializeDecrypted(const std::string &data);
 private:
  void save(const std::string &data);
};
}
}

#endif //M2_SERVER_REGISTERMANAGER_H
