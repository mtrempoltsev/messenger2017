#include "logincontroler.h"
#include "guiadapter.h"
#include <QtQml>

namespace m2 {
namespace gui {
namespace controler {


LoginControler::LoginControler(QObject *parent)
    :QObject(parent)
{

}

void LoginControler::declareQML()
{
    qmlRegisterType<LoginControler>("Controler.Login", 1, 0, "LoginControler");
}

void LoginControler::login() const
{
    emit startLogin();
    //TODO call core func
}

void LoginControler::loginSuccessed(const std::string &guid)
{
    GuiAdapter::getInstance().setGuid(guid);
    emit finishLoginSuccessed();
}

void LoginControler::loginFailed()
{
    //TODO add error msg handle
    emit finishLoginFailed("TODO THIS IS ERROR!!");
}

}
}
}
