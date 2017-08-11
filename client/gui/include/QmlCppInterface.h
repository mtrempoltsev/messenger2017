#pragma once

#include <QDebug>
#include <QObject>

#include "core.h"
#include "core_dispatcher.h"
#include "handlers.h"

using m2::core::Core;
using m2::core::CoreDispatcher;
using m2::LoginHandler;
using m2::RegisterHandler;

class QmlCppInterface : public QObject {
  Q_OBJECT
  m2::core::CoreDispatcher *dispatcher_;

public:
  // QmlCppInterface(QObject *parent) : QObject(parent) {}
  QmlCppInterface(m2::core::CoreDispatcher *dispatcher) {
    dispatcher_ = dispatcher;
  }
  virtual ~QmlCppInterface() {}
  Q_INVOKABLE void loginButtonClicked() {

    LoginHandler lh;
    lh.onCompletion = [](std::string uid) {
      std::cout << "GUI GOT UID!!! " << uid << std::endl;
    };
    dispatcher_->Login(lh);

    qDebug() << "!!!!!!!!!!!!!!!!!!!!!! HI FROM QML LOGIN! ";
    //    std::string fileName = b[id];
    // Config::getInstance().setCurrentBook(id);
  }
  Q_INVOKABLE void registerButtonClicked() {
    RegisterHandler rh;
    rh.onCompletion = []() { std::cout << "REGISTERED OK!" << std::endl; };
    qDebug() << "!!!!!!!!!!!!!!!!!!!!!! HI FROM QML REGISTRAAAAAATION! ";
    dispatcher_->RegisterUser(rh);
  }
};
