#include "path_settings.h"

#include <boost/filesystem.hpp>
std::string m2::core::GetManagerPath(const std::string &managerName)
{
    std::string managerPath;
#ifdef _WIN32 || _WIN64
    managerPath = ".";
#else
    managerPath = getenv("HOME");
    managerPath.append("/.messenger/");
    if(!managerName.empty())
        managerPath.append(managerName).append("/");
#endif
    boost::filesystem::path p(managerPath);
    if(!boost::filesystem::exists(p))
    {
        boost::filesystem::create_directories(p);
    }
    return managerPath;
}
