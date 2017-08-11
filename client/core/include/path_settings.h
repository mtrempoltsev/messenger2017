#pragma once

#include <string>
namespace m2 {
namespace core {

    std::string GetManagerPath(const std::string &managerName = "");

    const std::string contactListFileName = "contact-list.json";
    const std::string messageStoryFileName = "message-story.json";
}  // core
}  // m2
