#include "json_builder.h"
#include "path_settings.h"

#include <fstream>

namespace m2 {
namespace core {

    using boost::property_tree::ptree;
    using boost::property_tree::read_json;
    using boost::property_tree::write_json;

    Contact JsonBuilder::ReadContact() const {
        ptree pt2;
        std::ifstream stream(defaultPath + contactListFileName);
        read_json (stream, pt2);
        return Contact( pt2.get<std::string> ("id"),
                        pt2.get<std::string> ("nickname"));
    }

    void JsonBuilder::WriteContact(const Contact& contact) {
        ptree pt;
        pt.put ("id", contact.GetId());
        pt.put ("nickname", contact.GetNickname());

        std::ofstream stream(defaultPath + contactListFileName, std::ios_base::app); 
        write_json (stream, pt, false);
    }

} //core
} // m2
