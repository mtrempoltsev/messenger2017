#include "contact.h"
#include "path_settings.h"
#include "json_builder.h"
#include <exception>
#include <fstream>

namespace m2 {
namespace core {

std::istream& operator >> (std::istream& stream, Contact& contact) {
    stream >> contact.uuid_ >> contact.nickname_;
    return stream;
}

std::ostream& operator << (std::ostream& stream, const Contact& contact) {
    stream << contact.uuid_ << " " << contact.nickname_;
    return stream;
}

void
ContactManager::SaveContactList(const ContactManager::ContactList& contacts) {
    std::string defaultPath = GetDefaultPath();
    //std::ofstream stream(defaultPath + contactListFileName, std::ios_base::app);
    try {
        //if (stream.is_open()) {
            JsonBuilder jb;
            for (auto contact : contacts) {
                //if (!IsContactAlreadyExist) {
                    //stream << user << std::endl;
                //}
                jb.WriteContact(contact);
            }
        //}
    }
    catch(std::exception& ex) {
        // TODO: exception handling
    }
    //stream.close();
}

ContactManager::ContactList
ContactManager::GetContactList(const std::string& contactId) const {
    std::string defaultPath = GetDefaultPath();
    std::ifstream stream(defaultPath + contactListFileName);
    ContactList contactList = {};
    try {
        if (stream.is_open()) {
            Contact contactName;
            while(stream >> contactName) {
                contactList.push_back(contactName);
            }
        }
    }
    catch (std::exception& ex) {
        // TODO: exception handling
    }
    stream.close();
    return contactList;
}

} // core
} // m2
