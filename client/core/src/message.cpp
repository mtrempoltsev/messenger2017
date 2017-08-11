#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>

#include "message.h"
#include "path_settings.h"

namespace m2 {
namespace core {

    using namespace boost::property_tree;

    std::istream& operator>>(std::istream& stream, Message& message) {
        stream >> message.chat_id_ >> message.from_uuid_ >> message.sendTime_ >> message.text_;
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, const Message& message) {
        stream << message.chat_id_ << " " << message.from_uuid_ << " " << message.sendTime_ << " "
               << message.text_;
        return stream;
    }

    const MessageManager::MessageStory& MessageManager::GetMessageStory(
        const size_t chat_id) const {
        return messageStory_;
    }

    void MessageManager::LoadMessageStory(/*size_t pos, size_t messageNumber*/) {
        try {
            std::string managerPath = GetManagerPath("messages");
            std::string filename(managerPath + messageStoryFileName);
            std::ifstream stream(filename);
            if (stream.is_open()) {
                ptree pt;
                read_json(stream, pt);
                MessageBuilder mb;
                for (auto& it : pt.get_child("messages")) {
                    mb.chat_id = it.second.get<size_t>("chat_id");
                    mb.from_uuid = it.second.get<std::string>("from_uuid");
                    // TODO: time (from string --> time?)
                    //                    mb.sendTime = it.second.get<std::time_t>("send_time");
                    mb.text = it.second.get<std::string>("text");
                    messageStory_.push_back(Message(mb));
                }
                stream.close();
            }
        } catch (std::exception& ex) {
        }
    }

    void MessageManager::SaveMessageStory() {
        std::string managerPath = GetManagerPath("messages");
        std::string filename(managerPath + messageStoryFileName);
        // std::cout << filename << std::endl;
        std::ofstream stream(filename);
        try {
            if (stream.is_open()) {
                ptree pt, jsonHistory, child;
                for (auto& message : messageStory_) {
                    child.put("chat_id", message.GetChatId());
                    child.put("from_uuid", message.GetFrom());
                    child.put("send_time", message.GetSendTime());
                    child.put("text", message.GetText());
                    jsonHistory.push_back(std::make_pair("", child));
                }
                pt.add_child("messages", jsonHistory);
                write_json(stream, pt);
                stream.close();
            }
        } catch (std::exception& ex) {
            // TODO: exception handling
            std::cout << "SAVE exception " << ex.what() << std::endl;
        }
    }

}  // core
}  // m2
