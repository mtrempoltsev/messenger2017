#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace m2 {
namespace core {

class ContactBuilder {
public:
  std::string uuid;
  std::string nickname;
};

class Contact {
public:
  Contact(){};
  Contact(const ContactBuilder &cb) : uuid_(cb.uuid), nickname_(cb.nickname) {}

  std::string GetId() const { return uuid_; }

  std::string GetNickname() const { return nickname_; }

  friend std::istream &operator>>(std::istream &stream, Contact &contact);
  friend std::ostream &operator<<(std::ostream &stream, const Contact &contact);

private:
  std::string uuid_;
  std::string nickname_;
};

class ContactManager {
public:
  using ContactList = std::vector<Contact>;

  void LoadContactList();
  const ContactList &GetContactList() const;
  void AddContact(const std::string &uuid, const std::string &nickname);

private:
  void SaveContactList();
  ContactList contactList_;
};

} // core
} // m2
