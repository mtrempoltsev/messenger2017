#pragma once

#include <pwd.h>
#include <unistd.h>

const std::string contactListFileName = "contact-list.json";
const std::string defaultPath = "/home/sam/.messenger/";

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
