#include "stdafx.h"

#include "../include/core_dispatcher.h"
#include "../include/contact.h"
#include "../include/message.h"
#include <exception>
#include <fstream>
#include <pwd.h>
#include <unistd.h>

namespace m2 {
namespace core {

const std::string contactListFileName = "contact-list";

static std::string GetUsername() {
    struct passwd *pwd = getpwuid(getuid());
    if (pwd) {
        return pwd->pw_name;
    }
    return "(?)";
}

// TODO: This should be done once,
// default-path should be red from config file or some setting storage
static std::string GetDefaultPath() {
    std::string loginedUser(GetUsername());
    std::string defaultPath = "/home/" + loginedUser + "/.messenger/";
    return defaultPath;
}

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

void CoreDispatcher::SaveContactList(const CoreDispatcher::ContactList& contacts) {
    std::string defaultPath = GetDefaultPath();
    std::ofstream stream(defaultPath + contactListFileName, std::ios_base::app);
    try {
        if (stream.is_open()) {
            for (auto user : contacts) {
                //if (!IsContactAlreadyExist) {
                    stream << user << std::endl;
                //}
            }
        }
    }
    catch(std::exception& ex) {
        // TODO: exception handling
    }

    stream.close();
}

CoreDispatcher::ContactList CoreDispatcher::GetContactList() const {
    std::string defaultPath = GetDefaultPath();
    std::ifstream stream(defaultPath + contactListFileName);
    ContactList contactList = {};
    try {
        if (stream.is_open()) {
            Contact contactName;
            while(stream >> contactName) {
                contactList.push_back(contactName);
            }
        }
    }
    catch (std::exception& ex) {
        // TODO: exception handling
    }

    stream.close();
    return contactList;
}

CoreDispatcher::MessageStory CoreDispatcher::GetMessageStory(const std::string& username) const {
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

void CoreDispatcher::SaveMessageStory(const MessageStory& ms, const std::string& contactName) {
    std::string defaultPath = GetDefaultPath();
    std::ofstream writer(defaultPath + contactName, std::ios_base::app);
    if (writer.is_open()) {
        for (auto i : ms) {
            writer << i << std::endl;
        }
    }
}

} // core
} // m2
