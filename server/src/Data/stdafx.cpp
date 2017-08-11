
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "Data/stdafx.h"


void indices::MakeDir(const std::string &Path)
{
    using namespace boost::filesystem;
    if (!exists(Path))
        create_directories(Path);
}

uuids::uuid uuids::to_uuid(const boost::uuids::uuid &a)
{
    uuids::uuid b;
    for (size_t i = 0; i < a.size(); ++i)
        b |= (uuids::uuid(*(a.begin() + i)) << 8 * (a.size()-1-i));
    return b;
}

