#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <logincontroler.h>
#include <registrationcontroler.h>

#include <QQmlContext>
#include <include/chats_filter_proxy_model.h>
#include <include/contacts_model.h>
#include <include/messages_model.h>

#include <iostream>
#include <thread>

/* our uber-headers */
// gui <--> core
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

  std::cout << "start core" << std::endl;
  m2::core::Core core;
  m2::core::CoreDispatcher dispatcher;
  dispatcher.core_ = std::shared_ptr<Core>(&core);

  //   QmlCppInterface obj(&dispatcher);
  //   engine.rootContext()->setContextProperty("QmlCppInterface", &obj);

  std::thread coreThread(runcore, std::ref(core));
  //  RegisterHandler rh;
  //  rh.onCompletion = []() { std::cout << "REGISTERED OK!" << std::endl; };

  //  LoginHandler lh;
  //  lh.onComletion = [](std::string uid) {
  //    std::cout << "GUI GOT UID!!! " << uid << std::endl;
  //  };

  // dispatcher.registerUser(rh);
  // dispatcher.Login(lh);

  // coreThread.join();

  // int res = app.exec();
  // dispatcher.stopCore();

  QQmlApplicationEngine engine;

  engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  ChatsFilterProxyModel chats;
  MessagesModel messages;
  ContactsModel contacts;

  engine.rootContext()->setContextProperty("chatModel", &chats);
  engine.rootContext()->setContextProperty("messagesModel", &messages);
  engine.rootContext()->setContextProperty("contactsModel", &contacts);

  return app.exec();
}
