#include "include/controlers/logincontroler.h"
#include "include/guiadapter.h"
#include <QtQml>
#include "include/guiadapter.h"

namespace m2 {
namespace gui {
namespace controler {


LoginControler::LoginControler(QObject *parent)
    :QObject(parent)
{
    GuiAdapter* adapter = GuiAdapter::getGuiAdapter();

    connect(this, SIGNAL(startLogin()), adapter, SLOT(loginToServer()));
    connect(adapter, SIGNAL(loginSuccessed()), this, SIGNAL(finishLoginSuccessed()));
    connect(adapter, SIGNAL(loginFailed(QString)), this, SIGNAL(finishLoginFailed(QString)));
}


void LoginControler::declareQML()
{
    qmlRegisterType<LoginControler>("Controler.Login", 1, 0, "LoginControler");
}


void LoginControler::login() const
{
    emit startLogin();
}


}
}
}
