#include "path_settings.h"

#include <boost/filesystem.hpp>
#include <iostream>

#ifdef _WIN32 || _WIN64
    const std::string delim = "\\";
    const std::string userEnv = "USERPROFILE";
#else
    const std::string userEnv = "HOME";
    const std::string delim = "/";
#endif
bool createDir(const std::string path)
{
    if(!boost::filesystem::exists(path))
    {
        boost::system::error_code er;
        return boost::filesystem::create_directory(path, er);
    }
    return true;
}
std::string m2::core::GetManagerPath(const std::string &managerName) {


    std::string managerPath = getenv(userEnv.c_str());
#ifdef _WIN32 || _WIN64 //In Windows needed documents folder. Temp crtuch maybe
    managerPath.append(delim).append("documents");
    if(!createDir(managerPath))
        return std::string();
    managerPath.append(delim);
#endif
    managerPath.append(delim).append("messenger");
    if(!createDir(managerPath))
        return std::string();
    if(!managerName.empty()) {
       managerPath.append(delim).append(managerName);
       if(!createDir(managerPath))
           return std::string();
    }
    return managerPath.append(delim);
}
