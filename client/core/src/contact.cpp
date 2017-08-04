#include "../include/contact.h"

namespace m2 {
    namespace core {

    std::ifstream& operator >> (std::ifstream& stream, Contact& contact) {
        stream >> contact.uuid_ >> contact.nickname_;
        return stream;
    }
    std::ofstream& operator << (std::ofstream& stream, const Contact& contact) {
        stream << contact.uuid_ << " " << contact.nickname_;
        return stream;
    }

    } // core
} // m2
