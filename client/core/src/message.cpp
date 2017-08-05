#include "message.h"
#include <exception>
#include "path_settings.h"

namespace m2 {
namespace core {

std::ifstream& operator>>(std::ifstream& stream, MessageInfo& mi) {
  stream >> mi.from_uid_ >> mi.to_uid_ >> mi.sendTime_ >> mi.message_;
  return stream;
}

std::ofstream& operator<<(std::ofstream& stream, const MessageInfo& mi) {
  stream << mi.from_uid_ << " " << mi.to_uid_ << " " << mi.sendTime_ << " "
         << mi.message_;
  return stream;
}

MessageManager::MessageStory MessageManager::GetMessageStory(
    const std::string& username) const {
  std::string defaultPath = GetDefaultPath();
  std::ifstream reader(defaultPath + username);
  MessageStory ms;
  if (reader.is_open()) {
    MessageInfo mi;
    while (reader >> mi) {
      ms.push_back(mi);
    }
  }
  reader.close();
  return ms;
}

void MessageManager::SaveMessageStory(const MessageStory& ms,
                                      const std::string& contactName) {
  std::string defaultPath = GetDefaultPath();
  std::ofstream writer(defaultPath + contactName, std::ios_base::app);
  if (writer.is_open()) {
    for (auto i : ms) {
      writer << i << std::endl;
    }
  }
}

}  // core
}  // m2
