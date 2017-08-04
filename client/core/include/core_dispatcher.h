#pragma once

#include "error.h"
#include "guid.h"
#include <functional>
#include <string>
#include <vector>

namespace m2
{
    namespace core {
        struct RegisterNewUserHandler final
        {
            using CompletionHandler = std::function<void (Uuid&& userId)>;
            using ErrorHandler = std::function<void (Error&& error)>;

            CompletionHandler onComletion;
            ErrorHandler onError;
        };

        struct LoginHandler final
        {
            using CompletionHandler = std::function<void ()>;
            using ErrorHandler = std::function<void (Error&& error)>;

            CompletionHandler onComletion;
            ErrorHandler onError;
        };

        class MessageInfo {
        public:
            MessageInfo() {};
            MessageInfo(const std::string& participants,
                        const std::string& from,
                        //const TimeStamp& sendTime,
                        const std::string& message = "") {
                participants_ = participants;
                from_ = from;
                //sendTime_ = sendTime;
                message_ = message;
            }

            //using TimeStamp = std::chrono::system_clock::time_point;

            friend std::ofstream& operator << (std::ofstream& stream, const MessageInfo& mi);
            friend std::ifstream& operator >> (std::ifstream& stream, MessageInfo& mi);
        private:
            std::string participants_;
            std::string from_;
            //TimeStamp sendTime_;
            std::string message_;
        };

        class CoreDispatcher final
        {
        public:
            CoreDispatcher() = default;
            CoreDispatcher(CoreDispatcher&) = delete;
            CoreDispatcher& operator=(CoreDispatcher&) = delete;

            CoreDispatcher(CoreDispatcher&&) = delete;
            CoreDispatcher& operator=(CoreDispatcher&&) = delete;

            void registerNewUser(RegisterNewUserHandler handler);
            void login(const Uuid& uuid, LoginHandler handler);

            using ContactList = std::vector<std::string>;
            using MessageStory = std::vector<MessageInfo>;

            MessageStory GetMessageStory(const std::string& username) const;
            void SaveMessageStory(const MessageStory& ms, const std::string& contactName);

            void SaveContactList(const ContactList& contacts);
            ContactList GetContactList() const;
        };

    } // core
} // m2
