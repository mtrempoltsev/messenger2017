#pragma once

namespace m2 {
// namespace client_shared {
namespace core {
class MessageBuilder {
public:
  int chat_id;
  std::string from_uuid;
  std::time_t sendTime;
  std::string text;
};

class Message {
public:
  Message() {}
  Message(const MessageBuilder &mb)
      : chat_id_(mb.chat_id), from_uuid_(mb.from_uuid), sendTime_(mb.sendTime),
        text_(mb.text) {}

  int GetChatId() const { return chat_id_; }
  std::string GetFrom() const { return from_uuid_; }
  // std::time_t GetSendTime() const { return sendTime_; }
  std::string GetSendTime() const { return "12:34"; }
  std::string GetText() const { return text_; }

  friend std::ostream &operator<<(std::ostream &stream, const Message &message);
  friend std::istream &operator>>(std::istream &stream, Message &message);

private:
  int chat_id_;
  std::string from_uuid_;
  std::time_t sendTime_; // unix-time
  std::string text_;
};
}
}
