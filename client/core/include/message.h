#pragma once

#include <ctime>
#include <fstream>
#include <string>
#include <vector>

namespace m2 {
namespace core {

    class MessageBuilder {
    public:
        size_t chat_id;
        std::string from_uuid;
        std::time_t sendTime;
        std::string text;
    };

    class Message {
    public:
        Message() {}
        Message(const MessageBuilder& mb) :
            chat_id_(mb.chat_id), from_uuid_(mb.from_uuid),
            sendTime_(mb.sendTime), text_(mb.text) {}

        size_t GetChatId() const {
            return chat_id_;
        }

        std::string GetFrom() const {
            return from_uuid_;
        }

        std::time_t GetSendTime() const {
            return sendTime_;
        }

        std::string GetText() const {
            return text_;
        }

        friend std::ostream& operator << (std::ostream& stream, const Message& message);
        friend std::istream& operator >> (std::istream& stream, Message& message);
    private:
        size_t chat_id_;
        std::string from_uuid_;
        std::time_t sendTime_; // unix-time
        std::string text_;
    };

    class MessageManager {
    public:
        MessageManager(){};

        using MessageStory = std::vector<Message>;

        const MessageStory& GetMessageStory(const size_t chat_id) const;
        void LoadMessageStory();
        void SaveMessageStory();

    private:
        MessageStory messageStory_;
    };

}// core
} // m2
