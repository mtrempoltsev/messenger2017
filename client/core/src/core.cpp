#include "core.h"

using namespace m2::core;

Core::Core() {
    contactManager_ = std::make_shared<ContactManager>();
    messageManager_ = std::make_shared<MessageManager>();
}

ContactManager::ContactList
Core::GetContactList(const std::string& contactId) {
    return contactManager_->GetContactList(contactId);
}

void Core::SaveContactList(const ContactManager::ContactList& contacts) {
    contactManager_->SaveContactList(contacts);
}

MessageManager::MessageStory
Core::GetMessageStory(const std::string& id) {
    return messageManager_->GetMessageStory(id);
}

void Core::SaveMessageStory(const MessageManager::MessageStory& ms,
                            const std::string& contactId) {
    messageManager_->SaveMessageStory(ms, contactId);
}

boost::optional<m2::Error>
Core::StartServerConnection(const m2::Uuid &serverGuid) {
    boost::optional<m2::Error> error;

    return error;
}

boost::optional<m2::Error> Core::RegisterNewUser() {}

boost::optional<m2::Error> Core::Login(const m2::Uuid &clientUuid) {
    boost::optional<m2::Error> error;
    // TODO: send user uuid to the server and try to login
    return error;
}
