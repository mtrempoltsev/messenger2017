#include "Data/stdafx.h"



void indices::MakeDir(const std::string& Path) {
    using namespace boost::filesystem;
    if (!exists(Path))
    create_directories(Path);
}
