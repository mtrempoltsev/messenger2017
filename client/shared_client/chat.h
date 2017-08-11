#pragma once

#include <unordered_map>

namespace m2 {
namespace core {

    class Chat {
    private:
        size_t chatId_;
        std::string name_;
        std::string avatar_;  // имя файла с аватаром генерируем пока сами внутри кора
        Message lastMessage_;
        int unreadable_;

    public:
        Chat(size_t chatId, std::string name, /* std::string avatar,*/ Message message,
             int unreadable)
            : chatId_(chatId), name_(name), lastMessage_(message), unreadable_(unreadable) {}

        std::string GetChatId() const { return std::to_string(chatId_); }
        std::string GetName() const { return name_; }
        std::string GetAvatar() const { return avatar_; }
        Message GetLastMessage() const { return lastMessage_; }
        int GetUnreadable() const { return unreadable_; }
    };
}
}
