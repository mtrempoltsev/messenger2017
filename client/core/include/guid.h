#pragma once

#include "../stdafx.h"
namespace m2
{
    class Uuid final
    {
    public:
        std::string toString() const;
        boost::uuids::uuid get() const { return u_; }
        void set( const std::string & uuid );
    private:
        boost::uuids::uuid u_;
    };
}
