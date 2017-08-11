#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <logincontroler.h>
#include <registrationcontroler.h>

#include <include/chats_filter_proxy_model.h>
#include <include/contacts_model.h>
#include <include/messages_model.h>
#include <QQmlContext>

#include <iostream>
#include <thread>

#include "core.h"
#include "core_dispatcher.h"
#include "handlers.h"

using namespace m2::gui::controler;
using m2::core::Core;
using m2::core::CoreDispatcher;
using m2::LoginHandler;
using m2::RegisterHandler;

void runcore(Core &core) {
    // std::cout << "runcore" << std::endl;
    core.Start();
}

int main(int argc, char *argv[]) {

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  LoginControler::declareQML();
  RegistrationControler::declareQML();

  std::cout << "start core" << std::endl;
  m2::core::Core core;
  m2::core::CoreDispatcher dispatcher;
  dispatcher.core_ = std::shared_ptr<Core>(&core);

  engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  RegisterHandler regCallback;
  regCallback.onCompletion = nullptr;//[]() { std::cout << "Registration complete"; };
  regCallback.onError = nullptr;// [] (m2::Error && error) { std::cout << error.message; };

  std::thread coreThread(runcore, std::ref(core));
  try
  {
    dispatcher.RegisterUser("https://volt.trempoltsev.ru", regCallback);
  }
  catch (std::exception & ex) {
      std::cout << ex.what();
  }

  //  RegisterHandler rh;
  //  rh.onCompletion = []() { std::cout << "REGISTERED OK!" << std::endl; };

    ChatsFilterProxyModel chats;
    MessagesModel messages;
    ContactsModel contacts;

    engine.rootContext()->setContextProperty("chatModel", &chats);
    engine.rootContext()->setContextProperty("messagesModel", &messages);
    engine.rootContext()->setContextProperty("contactsModel", &contacts);

    /* START CORE */
    std::cout << "start core" << std::endl;

    /* GUI <--> CORE MESSAGE STORY TEST */
    const std::string chatId = "1";
    m2::MessageStoryHandler handler;
    using MessageStory = std::vector<m2::core::Message>;
    handler.onCompletion = [](const MessageStory &story) {
        std::cout << "Hurrray! We got messages story!" << std::endl;
        for (auto &i : story) {
            std::cout << i << std::endl;
        }
        // тут всякие emmit'ы
    };
    handler.onError = []() { std::cout << "You're a loser."; };

    // вызов кора выдать стори
    dispatcher.GetMessageStory(chatId, handler);
    return app.exec();

    // from merging-branch
    //const auto result = app.exec();
    //dispatcher.stopCore();
    //coreThread.join();
    //return result;
}
