#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "contact.h"

namespace m2 {
namespace core {

class ContactManager {
 public:
  using ContactList = std::vector<Contact>;

  ContactManager() { /*LoadContactList();*/
  }
  void LoadContactList();
  const ContactList &GetContactList() const;
  void AddContact(const std::string &uuid, const std::string &nickname);

 private:
  void SaveContactList();
  ContactList contactList_;
};

}  // core
}  // m2
