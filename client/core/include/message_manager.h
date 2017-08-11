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
        const MessageStory& GetMessageStory(const int chatId) const;
        void LoadMessageStory(int chatId);
        void SaveMessageStory();

        // chats
        using ChatsMap = std::unordered_map<int, Chat>;
        const ChatsMap& GetChats() const;
        bool ChatExists(int id) { return (chatsIds_.count(id) > 0); }

        void SendMessage(Message& message);

    private:
        // chatId and history
        MessageStory emptyStory_;
        std::unordered_map<int, MessageStory> storyByChat_;
        std::set<int> chatsIds_;  // список начатых чатов
    };

}  // core
}  // m2
