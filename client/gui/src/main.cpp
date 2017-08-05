#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>
#include <thread>
#include "../../core/include/core.h"
#include "../../core/include/core_dispatcher.h"

#include <registrationcontroler.h>

using namespace m2::gui::controler;

using m2::core::Core;
using m2::core::CoreDispatcher;
using m2::core::LoginHandler;
using m2::core::RegisterHandler;

void runcore(Core &core) {
  // std::cout << "runcore" << std::endl;
  core.Start();
}

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  RegistrationControler::declareQML();

  engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
  if (engine.rootObjects().isEmpty()) return -1;

  std::cout << "start core" << std::endl;
  m2::core::Core core;
  m2::core::CoreDispatcher dispatcher;
  dispatcher.core_ = std::shared_ptr<Core>(&core);

  std::thread coreThread(runcore, std::ref(core));
  RegisterHandler rh;
  rh.onCompletion = []() { std::cout << "REGISTERED OK!" << std::endl; };
  LoginHandler lh;
  lh.onComletion = [](std::string uid) {
    std::cout << "GUI GOT UID!!! " << uid << std::endl;
  };

  dispatcher.registerUser(rh);
  dispatcher.login(lh);

  // coreThread.join();

  int res = app.exec();
  dispatcher.stopCore();
  return res;
}
