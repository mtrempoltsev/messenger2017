#include <iostream>
#include "../include/core_dispatcher.h"

int main() {
    {
        using namespace m2;
        using namespace core;

        CoreDispatcher cd;
        CoreDispatcher::ContactList contacts = { "Ivan", "Sergey", "Vadim" };
        cd.SaveContactList(contacts);

        CoreDispatcher::ContactList contacts1 = cd.GetContactList();
        for (auto i : contacts1) {
            std::cout << i << " ";
        }

        CoreDispatcher::MessageStory ms =
            { MessageInfo("user1:user2", "user2", "Trololololo"),
              MessageInfo("user1:user2", "user1", "olololo"),
              MessageInfo("user1:user2", "user1", "Trololololo"),
              MessageInfo("user1:user2", "user2", "olololo" )
            };

        cd.SaveMessageStory(ms, "user1");

        CoreDispatcher::MessageStory ms1 = cd.GetMessageStory("user1");
        cd.SaveMessageStory(ms1, "user2");
    }

    return 0;
}
