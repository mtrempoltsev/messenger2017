#pragma once

#include <list>

#include "message.h"

/*
  Привет из кора!
  Где-то в main() будет что-то типа такого:

  m2::core::Core core;                    // стартуем ядро
  m2::core::CoreDispatcher dispatcher;    // стартуем диспетчер (к - кэп)

  Ну а потом вы будете вызывать нас через этот самый диспетчер примерно так:

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

// GUI --> Core
dispatcher.GetMessageStory(userId, handler)

// core_dispatcher.h
void GetMessageStory(const std::string &id, MessageStoryHandler handler);

// message.h
class Message {
private:
  size_t chat_id;
  std::string from_uuid;
  std::time_t sendTime;
  std::string text;

public:
  size_t GetChatId() const { return chat_id_; }
  std::string GetFrom() const { return from_uuid_; }
  std::time_t GetSendTime() const { return sendTime_; }
  std::string GetText() const { return text_; }
}
*/
struct MessageStoryHandler final {
    using MessageStory = std::vector<m2::core::Message>;

    using CompletionHandler = std::function<void(const MessageStory &)>;
    using ErrorHandler = std::function<void(/*Error &&error*/)>;

    CompletionHandler onCompletion;
    ErrorHandler onError;
};

// В процессе...
struct ServerSetHandler final {
    using CompletionHandler = std::function<void(std::set<std::string> servers)>;
    using ErrorHandler = std::function<void(Error &&error)>;

    CompletionHandler onCompletion;
    ErrorHandler onError;
};
}
