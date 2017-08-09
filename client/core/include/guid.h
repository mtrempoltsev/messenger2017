#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>

#include <boost/uuid/uuid.hpp>
#include <string>

namespace m2
{
    class Uuid final
    {
    public:
        Uuid() = default;
        Uuid(const std::string & uuid);
        std::string ToString() const;
        boost::uuids::uuid Get() const { return u_; }
        bool Set(const std::string& uuid);
        void Generate();
    private:
        boost::uuids::uuid u_;
    };
}
