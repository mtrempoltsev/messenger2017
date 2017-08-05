
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "Data/stdafx.h"


void indices::MakeDir(const std::string& Path) {
    using namespace boost::filesystem;
    if (!exists(Path))
    create_directories(Path);
}
