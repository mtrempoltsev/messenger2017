#include "registrationcontroler.h"
#include "guiadapter.h"
#include <QtQml>


namespace m2 {
namespace gui {
namespace controler {

void RegistrationControler::declareQML()
{
    qmlRegisterUncreatableMetaObject(staticMetaObject,
                "RegistrationStates", 1, 0, "RegistartionControler", "Get back to work, slacker!");
}

RegistrationControler::RegistrationControler()
{

}

void RegistrationControler::registerMe(const QString &server) const
{
    //TODO get core
    emit changeRegistartionState(RegistrationControler::States::LOADING);
}

void RegistrationControler::registrationSuccessed(const std::string &guid)
{
    GuiAdapter::getInstance().setGuid(guid);
    emit changeRegistartionState(States::SUCCESS);
}

void RegistrationControler::registrationFailed()
{
    emit changeRegistartionState(States::ERROR);
}


}
}
}
