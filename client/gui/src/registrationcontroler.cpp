#include "include/registrationcontroler.h"
#include "include/guiadapter.h"

namespace m2 {
namespace gui {


void RegistrationControler::declareQML()
{
    //qmlRegisterType<StyleClass>("RegistrationStates", 1, 0, "RegistartionControler");
}

RegistrationControler::RegistrationControler()
{

}

void RegistrationControler::registerMe(const QString &server) const
{
    //TODO get core
    //emit changeRegistartionState(States::LOADING);
}

void RegistrationControler::registrationSuccessed(const std::string &guid)
{
    //GuiAdapter::setGuid(guid);
    //emit changeRegistartionState(States::SUCCESS);
}

void RegistrationControler::registrationFailed()
{
    //emit changeRegistartionState(States::ERROR);
}


}
}
