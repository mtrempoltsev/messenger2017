#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <include/controlers/chatscontroler.h>
#include <include/controlers/logincontroler.h>
#include <include/controlers/messagescontroler.h>
#include <include/controlers/registrationcontroler.h>

#include <include/models/chats_filter_proxy_model.h>
#include <include/models/contacts_model.h>

#include <include/guiadapter.h>

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

  LoginControler::declareQML();
  RegistrationControler::declareQML();
  MessagesControler::declareQML();
  ContactsModel::declareQML();
  ChatsControler::declareQML();

  /* START CORE */
  std::cout << "start core" << std::endl;
  m2::core::Core core;
  std::shared_ptr<m2::core::CoreDispatcher> dispatcher =
      std::make_shared<m2::core::CoreDispatcher>();
  dispatcher.get()->core_ = std::shared_ptr<Core>(&core);

  //  engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
  //  if (engine.rootObjects().isEmpty())
  //    return -1;

  //  GuiAdapter *adapter = GuiAdapter::getGuiAdapter();
  // GuiAdapter::getGuiAdapter()->addModelsToEngineRoot(&engine);

  QQmlApplicationEngine engine;

  GuiAdapter::getGuiAdapter()->addModelsToEngineRoot(&engine);

  engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  GuiAdapter::getGuiAdapter()->setDispatcher(dispatcher);

  RegisterHandler regCallback;
  regCallback.onCompletion = []() { std::cout << "Registration complete"; };
  regCallback.onError = [](m2::Error &&error) { std::cout << error.message; };

  std::thread coreThread(runcore, std::ref(core));
  try {
    dispatcher.get()->RegisterUser("https://volt.trempoltsev.ru", regCallback);
  } catch (std::exception &ex) {
    std::cout << ex.what();
  }

  //  RegisterHandler rh;
  //  rh.onCompletion = []() { std::cout << "REGISTERED OK!" << std::endl; };

  //  ChatsFilterProxyModel chats;
  //  MessagesModel messages;
  //  ContactsModel contacts;

  //  engine.rootContext()->setContextProperty("chatModel", &chats);
  //  engine.rootContext()->setContextProperty("messagesModel", &messages);
  //  engine.rootContext()->setContextProperty("contactsModel", &contacts);

  //  /* START CORE */
  //  std::cout << "start core" << std::endl;

  //  /* GUI <--> CORE MESSAGE STORY TEST */
  //  const std::string chatId = "1";
  //  m2::MessageStoryHandler handler;
  //  using MessageStory = std::vector<m2::core::Message>;
  //  handler.onCompletion = [](const MessageStory &story) {
  //    std::cout << "Hurrray! We got messages story!" << std::endl;
  //    for (auto &i : story) {
  //      std::cout << i << std::endl;
  //    }
  //    // тут всякие emmit'ы
  //  };
  //  handler.onError = []() { std::cout << "You're a loser."; };

  //  // вызов кора выдать стори
  //  dispatcher.GetMessageStory(chatId, handler);
  //  // return app.exec();

  // from merging-branch
  const auto result = app.exec();
  coreThread.join();
  dispatcher.get()->stopCore();
  return result;
}
