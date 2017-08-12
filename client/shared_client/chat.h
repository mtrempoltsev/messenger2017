#pragma once

#include <unordered_map>

namespace m2 {
namespace core {

    class Chat {
    private:
        int id_;
        std::string name_;
        std::string avatar_;  // имя файла с аватаром генерируем пока сами внутри кора
        Message lastMessage_;
        int unreadable_;

    public:
        Chat() {}
        Chat(int chatId, std::string name, std::string avatar, Message message, int unreadable)
            : id_(chatId),
              name_(name),
              avatar_(avatar),
              lastMessage_(message),
              unreadable_(unreadable) {}

        std::string GetChatId() const { return std::to_string(id_); }
        std::string GetName() const { return name_; }
        std::string GetAvatar() const { return avatar_; }
        Message GetLastMessage() const { return lastMessage_; }
        int GetUnreadable() const { return unreadable_; }

        //   void SetAvatar(const std::string& avatar) { avatar_ = avatar; }
    };
}
}
