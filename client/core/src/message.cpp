#include "../include/message.h"

namespace m2 {
    namespace core {

        std::ifstream& operator >> (std::ifstream& stream, MessageInfo& mi) {
            stream >> mi.from_uid_ >>
                      mi.to_uid_ >>
                      mi.sendTime_ >>
                      mi.message_;
            return stream;
        }

        std::ofstream& operator << (std::ofstream& stream, const MessageInfo& mi) {
            stream << mi.from_uid_ << " "
                   << mi.to_uid_ << " "
                   << mi.sendTime_ << " "
                   << mi.message_;
            return stream;
        }
    } // core
} // m2
