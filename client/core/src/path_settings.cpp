#include "path_settings.h"

#include <boost/filesystem.hpp>
std::string m2::core::GetManagerPath(const std::string &managerName) {
    std::string delim;
    std::string userEnv;
#ifdef _WIN32 || _WIN64
    delim = "\\";
    userEnv = "USERPROFILE";
#else
    userEnv = "HOME";
    delim = "/";
#endif
    std::string managerPath = getenv(userEnv.c_str());
#ifdef _WIN32 || _WIN64 //In Windows needed documents folder. Temp crtuch maybe
    managerPath.append(delim).append("documents").append(delim);
#endif
    managerPath.append(delim).append(".messenger").append(delim);
    if(!managerName.empty()) {
        managerPath.append(managerName).append(delim);
    }

    boost::filesystem::path p(managerPath);
    if(!boost::filesystem::exists(p)) {
        boost::filesystem::create_directories(p);
    }
    return managerPath;
}
