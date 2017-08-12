#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>

#include "message.h"
#include "message_manager.h"
#include "path_settings.h"
#include "safe_log.h"

namespace m2 {
namespace core {

using namespace boost::property_tree;

std::istream &operator>>(std::istream &stream, Message &message) {
  stream >> message.chat_id_ >> message.from_uuid_ >> message.sendTime_ >>
      message.text_;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Message &message) {
  stream << message.chat_id_ << " " << message.from_uuid_ << " "
         << message.sendTime_ << " " << message.text_;
  return stream;
}

MessageManager::MessageManager() {
  // FIXME: chatsIds_ будут заполнены, когда (если) у нас заработает сервер
  chatsIds_.insert(1);
  chatsIds_.insert(2);
  chatsIds_.insert(3);

  // загружаем истории всех чатов
  for (auto &it : chatsIds_) {
    LoadMessageStory(it);
  }
}
const MessageManager::MessageStory &
MessageManager::GetMessageStory(const int chatId) const {
  if (storyByChat_.count(chatId) > 0) // если есть чат с заданным id
    return storyByChat_.at(chatId);
  else // чата нет
    /* FIXME: мб надо какой-нибудь optional<Error> ? */
    return emptyStory_;
}

// void MessageManager::LoadMessageStory(/*int pos, int messageNumber*/) {
void MessageManager::LoadMessageStory(int chatId) {
  //        m2::safelog::SafeLog log(GetManagerPath("log").append("mess.log"));
  try {
    std::string managerPath = GetManagerPath("messages");
    // std::string filename(managerPath + messageStoryFileName);
    // messages/1.json и т.д.
    std::string filename(managerPath + std::to_string(chatId) + ".json");
    std::ifstream stream(filename);
    if (stream.is_open()) {
      ptree pt;
      read_json(stream, pt);
      MessageBuilder mb;
      for (auto &it : pt.get_child("messages")) {
        mb.chat_id = it.second.get<int>("chat_id");
        mb.from_uuid = it.second.get<std::string>("from_uuid");
        // TODO: time (from string --> time?)
        //                    mb.sendTime =
        //                    it.second.get<std::time_t>("send_time");
        mb.text = it.second.get<std::string>("text");
        storyByChat_[mb.chat_id].push_back(Message(mb));
      }

      // после того, как вся история прочитана, формируем "чат" и берём
      // последнее
      // сообщение из диалога
      Chat bufChat(mb.chat_id, "NO CHAT NAME", "/demo/asd.jpg", Message(mb), 0);
      startedChats_[mb.chat_id] = bufChat;

      stream.close();
    }
  } catch (std::exception &ex) {
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
      for (auto &story : storyByChat_) {
        for (auto &message : story.second) {
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
    }
  } catch (std::exception &ex) {
    // TODO: exception handling
    std::cout << "SAVE exception " << ex.what() << std::endl;
  }
}

} // core
} // m2
