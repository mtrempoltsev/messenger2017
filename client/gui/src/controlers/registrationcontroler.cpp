#include "include/controlers/registrationcontroler.h"

#include <QtQml>

#include "include/guiadapter.h"

namespace m2 {
namespace gui {
namespace controler {


RegistrationControler::RegistrationControler(QObject* parent)
    :QObject(parent)
{
    GuiAdapter* adapter = GuiAdapter::getGuiAdapter();

    connect(this, SIGNAL(startRegister(QString)), adapter, SLOT(registerToServer(QString)));
    connect(adapter, SIGNAL(registrationSuccessed()), this, SIGNAL(finishRegisterSuccessed()));
    connect(adapter, SIGNAL(registrationFailed(QString)), this, SIGNAL(finishRegisterFailed(QString)));
}


void RegistrationControler::declareQML()
{
    qmlRegisterType<RegistrationControler>("Controler.Registration", 1, 0, "RegistraionControler");
}


void RegistrationControler::registerMe(const QString &server) const
{
    emit startRegister(server);
}


QVariant RegistrationControler::getServerList()
{
    return QVariant(GuiAdapter::getGuiAdapter()->getServerList());
}


}
}
}
