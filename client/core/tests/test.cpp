#include <iostream>
#include "../include/core_dispatcher.h"

int main() {
    {
        using namespace m2;
        using namespace core;

        CoreDispatcher cd;
        CoreDispatcher::ContactList contacts =
            { Contact("uuid1", "Ivan"),
              Contact("uuid2", "Sergey"),
              Contact("uuid3", "Vadim")
          };

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
        cd.SaveMessageStory(ms1, "user2");
    }

    return 0;
}
