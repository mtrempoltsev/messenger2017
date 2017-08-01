#include "../stdafx.h"
#include "RegisterManager.h"

int main(int argc, char *argv[]) {
  m2::server::RegisterManager rg;
  std::string test = "{\"public_key\": \"ddd\" }";
  rg.doAction(test, m2::server::RegisterManager::createEncrypted);
  return 0;
}
