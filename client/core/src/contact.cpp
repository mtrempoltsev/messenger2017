#include "contact.h"
#include "path_settings.h"
//#include "json_builder.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <exception>
#include <fstream>

namespace m2 {
namespace core {

using namespace boost::property_tree;

std::istream& operator >> (std::istream& stream, Contact& contact) {
    stream >> contact.uuid_ >> contact.nickname_;
    return stream;
}

std::ostream& operator << (std::ostream& stream, const Contact& contact) {
    stream << contact.uuid_ << " " << contact.nickname_;
    return stream;
}

void ContactManager::AddContact(const std::string& uuid, const std::string& nickname) {
    ContactBuilder cb;
    cb.uuid = uuid;
    cb.nickname = nickname;

    contactList_.push_back(cb);
    SaveContactList();
}

void
ContactManager::SaveContactList() {
    std::string defaultPath = GetDefaultPath();
    std::ofstream stream(defaultPath + contactListFileName);
	if (stream.is_open()) {
		try {
			ptree pt, jsonContacts, child;
			for (auto& contact : contactList_) {
				child.put("id", contact.GetId());
				child.put("nickname", contact.GetNickname());
				jsonContacts.push_back(std::make_pair("", child));
			}
			pt.add_child("contacts", jsonContacts);
			write_json(stream, pt);
		}
		catch (std::exception& ex) {
			// TODO: exception handling
			std::cout << "SAVE exception " << ex.what() << std::endl;
			//write a message to a log
		}
		stream.close();
	}
	else {
		//file was not created!!!
		//write a message to a log
	}
}

void ContactManager::LoadContactList() {
    std::string defaultPath = GetDefaultPath();
    std::string filename = defaultPath + contactListFileName;
    std::ifstream stream(filename);
	if (stream.is_open())
	{
		contactList_.clear(); //where should we clear the contact list?
		try {
			ptree pt;
			read_json(stream, pt);
			ContactBuilder cb;
			for (auto& it : pt.get_child("contacts")) {
				cb.uuid = it.second.get<std::string>("id");
				cb.nickname = it.second.get<std::string>("nickname");
				contactList_.push_back(Contact(cb));
			}
		}
		catch (std::exception& ex) {
			// TODO: exception handling
			std::cout << "LOAD exception " << ex.what() << std::endl;
			//write a message to a log
		}
		stream.close();
	}
	else {
		//file does not exist!!!
		//write a message to a log
	}
}

const ContactManager::ContactList&
ContactManager::GetContactList() const {
    return contactList_;
}

} // core
} // m2