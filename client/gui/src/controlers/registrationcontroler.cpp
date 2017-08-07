#include "registrationcontroler.h"
#include "guiadapter.h"
#include <QtQml>

namespace m2 {
namespace gui {
namespace controler {


RegistrationControler::RegistrationControler(QObject* parent)
    :QObject(parent)
{

}

void RegistrationControler::declareQML()
{
    qmlRegisterType<RegistrationControler>("Controler.Registration", 1, 0, "RegistraionControler");
}

void RegistrationControler::registerMe(const QString &server) const
{
    //TODO get core
    emit startRegister();
//    emit finishRegisterSuccessed();
}

void RegistrationControler::registrationSuccessed(const std::string &guid)
{
    GuiAdapter::getInstance().setGuid(guid);
    emit finishRegisterSuccessed();
}

void RegistrationControler::registrationFailed()
{

    emit finishRegisterFailed("Error");
}


}
}
}
