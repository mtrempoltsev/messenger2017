#pragma once

#include <fstream>
#include <string>

namespace m2 {
    namespace core {

        class Contact {
        public:
            Contact() {};
            Contact(const std::string& uuid,
                    const std::string& nickname) {
                uuid_ = uuid;
                nickname_ = nickname;
            }

            friend std::ifstream& operator >> (std::ifstream& stream, Contact& contact);
            friend std::ofstream& operator << (std::ofstream& stream, const Contact& contact);
        private:
            std::string uuid_;
            std::string nickname_;
        };

    } //core
} //m2
