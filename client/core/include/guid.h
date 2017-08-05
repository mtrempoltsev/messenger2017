#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>

namespace m2
{
    class Uuid final
    {
    public:
        std::string toString() const;

    private:
        boost::uuids::uuid u;
    };
}
