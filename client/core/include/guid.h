#pragma once

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
