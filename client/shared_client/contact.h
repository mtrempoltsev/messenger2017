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
        std::string avatar;
    };

    class Contact {
    public:
        Contact() {}
        Contact(const ContactBuilder &cb)
            : uuid_(cb.uuid), nickname_(cb.nickname), avatar_(cb.avatar) {}

        std::string GetId() const { return uuid_; }
        std::string GetNickname() const { return nickname_; }
        std::string GetAvatar() const { return avatar_; }

        friend std::istream &operator>>(std::istream &stream, Contact &contact);
        friend std::ostream &operator<<(std::ostream &stream, const Contact &contact);

    private:
        std::string uuid_;
        std::string nickname_;
        std::string avatar_;
    };
}  // core
}  // m2
