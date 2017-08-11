#pragma once

#include <ctime>
#include <fstream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "chat.h"
#include "message.h"

namespace m2 {
namespace core {
    class MessageManager {
    public:
        MessageManager();

        // message story
        using MessageStory = std::vector<Message>;
        const MessageStory& GetMessageStory(const size_t chatId) const;
        void LoadMessageStory(size_t chatId);
        void SaveMessageStory();

        // chats
        using ChatsMap = std::unordered_map<size_t, Chat>;
        const ChatsMap& GetChats() const;
        bool ChatExists(size_t id) { return (chatsIds_.count(id) > 0); }

    private:
        // chatId and history
        std::unordered_map<size_t, MessageStory> storyByChat_;
        std::set<size_t> chatsIds_;  // список начатых чатов
    };

}  // core
}  // m2
