#include "stdafx.h"

#include "core_dispatcher.h"
#include "contact.h"
#include "message.h"

namespace m2 {
namespace core {
/*
static bool IsContactAlreadyExist(std::string& contact) {
    std::string defaultPath = GetDefaultPath();
    std::ifstream reader(defaultPath + contactListFileName);
    if (reader.is_open()) {
        std::string username;
        while (getline(reader, username)) {
            if (username == contact) {
                reader.close();
                return true;
            }
        }
    }
    reader.close();
    return false;
}


*/

} // core
} // m2
