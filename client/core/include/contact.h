#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace m2 {
namespace core {

    class Contact {
    public:
        Contact() {};
        Contact(const std::string& uuid,
                const std::string& nickname) {
            uuid_ = uuid;
            nickname_ = nickname;
        }

        std::string GetId() const {
            return uuid_;
        }

        std::string GetNickname() const {
            return nickname_;
        }

        friend std::istream& operator >> (std::istream& stream, Contact& contact);
        friend std::ostream& operator << (std::ostream& stream, const Contact& contact);
    private:
        std::string uuid_;
        std::string nickname_;
    };

    class ContactManager {
    public:
        using ContactList = std::vector<Contact>;

        void SaveContactList(const ContactList& contacts);
        ContactList GetContactList(const std::string& contactId) const;
    };

} //core
} //m2
