#pragma once

#include <list>

// client/shared_client/
#include "chat.h"
#include "message.h"

/*
  Привет из кора!
  Где-то в main() будет что-то типа такого:

  m2::core::Core core;                    // стартуем ядро
  m2::core::CoreDispatcher dispatcher;    // стартуем диспетчер (к - кэп)

  Ну а потом вы будете вызывать MessageStoryнас через этот самый диспетчер примерно так:

  void zaregaiMenyaKnopka(){
    RegisterHandler handler;
    handler.onCompletion = []() { emit registrationOk(); };
    handler.onError = []() { emit vsePropalo(); };
    dispatcher.RegisterUser(handler);
  }

  Тут будет общая струткура того, что мы вам возвращаем.

  Ещё есть core/include/core_dispatcher.h, где описаны эти самые функции,
  которые вы вызываете через диспатчер. Пока они принимают только хэндлеры, но
  вангую что будут ситуации, когда вы захотите нам что-то передать (или я не
  права?)

  Пока у нас не предусмотрено ничего, кроме следующего:

  void Login(LoginHandler handler);
  void RegisterUser(RegisterHandler handler);
  void GetServerSet(ServerSetHandler handler);

  В принципе, уже сейчас вы можете нас вызвыать, и мы вам вернём, что у нас всё
  хорошо :D
 */

namespace m2 {

struct LoginHandler final {
    using CompletionHandler = std::function<void(std::string uuid)>;
    using ErrorHandler = std::function<void(Error &&error)>;

    CompletionHandler onCompletion;
    ErrorHandler onError;
};

struct RegisterHandler final {
    using CompletionHandler = std::function<void()>;
    using ErrorHandler = std::function<void(Error &&error)>;

    CompletionHandler onCompletion;
    ErrorHandler onError;
};

/********* MESSAGES *********
Если вдруг очень хочется потестить, то мессенджы хрянятся:
  на юниксе в ~/.messenger/message-story.json
  на винде в %USERPROFILE/documents/.messenger/message-story.json
В core/tests есть пример. Правда там время пока не оч работает :D

1. Вы нас вызываете (GUI --> Core)
    dispatcher.GetMessageStory(userId, handler)

2. Параметры, которые мы хотим чтобы вы нам передали при вызове
см. core/include/core_dispatcher.h
    void GetMessageStory(const std::string &id, MessageStoryHandler handler);

3. Струткура того, что мы вам возвращаем (через аргументы CompletionHandler и ErrorHandler) и как
вам это получать
см. client/shared_client/message.h
*/
struct MessageStoryHandler final {
    using MessageStory = std::vector<m2::core::Message>;

    using CompletionHandler = std::function<void(const MessageStory &)>;
    using ErrorHandler = std::function<void(/*Error &&error*/)>;

    CompletionHandler onCompletion;
    ErrorHandler onError;
};

/********* CHATS *********
1. Вы нас вызываете (GUI --> Core)
    dispatcher.GetChats(userId, handler)

2. Параметры, которые мы хотим чтобы вы нам передали при вызове
(см. core/include/core_dispatcher.h)
    void GetChats(ChatsHandler handler);

3. Струткура того, что мы вам возвращаем (через аргументы CompletionHandler и ErrorHandler) и как
вам это получать
см. client/shared_client/chat.h
*/
struct ChatsHandler final {
    using ChatsMap = std::unordered_map<size_t, m2::core::Chat>;

    using CompletionHandler = std::function<void(const ChatsMap &)>;
    using ErrorHandler = std::function<void(/*Error &&error*/)>;

    CompletionHandler onCompletion;
    ErrorHandler onError;
};

/********* CONTACTS *********
Если вдруг очень хочется потестить, то мессенджы хрянятся:
  на юниксе в ~/.messenger/message-story.json
  на винде в %USERPROFILE/documents/.messenger/contact-list.json
В core/tests есть пример.

// GUI --> Core
dispatcher.GetContact(userId, handler)

// core_dispatcher.h
void GetContact(const std::string &id, MessageStoryHandler handler);

// client/shared_client/contact.h
*/

// ...

/********* SERVER LIST *********/
struct ServerSetHandler final {
    using CompletionHandler = std::function<void(const std::list<std::string> &servers)>;
    using ErrorHandler = std::function<void(/*Error &&error*/)>;

    CompletionHandler onCompletion;
    ErrorHandler onError;
};
}
