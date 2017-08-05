#pragma once

#include "config.h"
#include "error.h"
#include "guid.h"
#include "contact.h"
#include "message.h"

#include <boost/optional.hpp>
#include <map>
#include <memory>

namespace m2 {
namespace core {

    class Core {
    public:
        Core();
        std::map<m2::Uuid, std::string> GetServersMap();

        // core <--> server
        boost::optional<m2::Error> StartServerConnection(const m2::Uuid &serverGuid);
        boost::optional<m2::Error> Login(const m2::Uuid &clientUuid);
        boost::optional<m2::Error> RegisterNewUser();

        // core <-> GUI
        ContactManager::ContactList GetContactList(const std::string& contactId);
        void SaveContactList(const ContactManager::ContactList& contacts);

        MessageManager::MessageStory GetMessageStory(const std::string& id);
        void SaveMessageStory(const MessageManager::MessageStory& ms,
                              const std::string& contactId);

    private:
        std::map<m2::Uuid, std::string>
        ReadServersFile(); // make list (actually, map) of availible servers
        std::shared_ptr<ContactManager> contactManager_;
        std::shared_ptr<MessageManager> messageManager_;

        std::map<m2::Uuid, std::string> serversMap_;
        m2::core::Config config_;
    };

} // core
} // m2
