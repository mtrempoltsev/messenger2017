#include "core.h"
#include "core_dispatcher.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    {
        using namespace m2;
        using namespace core;

        ContactManager::ContactList contacts = { Contact("uuid1", "Ivan"),
                                                 Contact("uuid2", "Alex"),
                                                 Contact("uuid3", "Vadim") };

        Core core;
        core.SaveContactList(contacts);
        ContactManager::ContactList contacts2 = core.GetContactList("user1");

        for (auto& i : contacts2) {
            std::cout << i << std::endl;
        }
/*
        cd.SaveContactList(contacts);

        CoreDispatcher::ContactList contacts1 = cd.GetContactList();
        //for (auto i : contacts1) {
        //    std::cout << i << " ";
        //}

        CoreDispatcher::MessageStory ms =
            { MessageInfo("user1", "user2", "14:35", "Trololololo"),
              MessageInfo("user2", "user1", "14:37", "olololo"),
              MessageInfo("user2", "user1", "14:45", "Trololololo"),
              MessageInfo("user1", "user2", "14:55", "olololo" )
            };

        cd.SaveMessageStory(ms, "user1");

        CoreDispatcher::MessageStory ms1 = cd.GetMessageStory("user1");
        cd.SaveMessageStory(ms1, "user2");*/
    }

    return 0;
}
