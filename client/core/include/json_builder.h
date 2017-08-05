#pragma once

#include "contact.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace m2 {
namespace core {

    class JsonBuilder {
    public:

        Contact ReadContact() const;
        void WriteContact(const Contact& contact);

    };
} // core
} // m2
