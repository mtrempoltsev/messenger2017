#include "core.h"
#include "core_dispatcher.h"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    {
        using namespace m2;
        using namespace core;

        Core core;
        /*auto contactDispatcher = core.getContactManager();
        contactDispatcher->LoadContactList();
        contactDispatcher->GetContactList();

        for (auto& i : contactDispatcher->GetContactList()) {
            std::cout << i << std::endl;
        }

        contactDispatcher->AddContact("uuid4", "qwerty");
        contactDispatcher->LoadContactList();

        for (auto& i : contactDispatcher->GetContactList()) {
            std::cout << i << std::endl;
        }*/

        auto messageDispatcher = core.GetMessageManager();
        messageDispatcher->LoadMessageStory();
        messageDispatcher->GetMessageStory(1);

        for (auto& i : messageDispatcher->GetMessageStory(1)) {
            std::cout << i << std::endl;
        }

    }

    return 0;
}
