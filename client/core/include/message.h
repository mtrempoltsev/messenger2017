#pragma once

#include <fstream>
#include <string>

namespace m2 {
    namespace core {

        class MessageInfo {
        public:
            MessageInfo() {};
            MessageInfo(//const std::string& participants,
                        const std::string& from_uid,
                        const std::string& to_uid,
                        const std::string& sendTime,
                        const std::string& message = "") {
                //participants_ = participants;
                from_uid_ = from_uid;
                to_uid_ = to_uid;
                sendTime_ = sendTime;
                message_ = message;
            }

            friend std::ofstream& operator << (std::ofstream& stream, const MessageInfo& mi);
            friend std::ifstream& operator >> (std::ifstream& stream, MessageInfo& mi);
        private:
            //std::string participants_;
            std::string from_uid_;
            std::string to_uid_;
            std::string sendTime_;
            std::string message_;
        };

    }// core
} // m2
