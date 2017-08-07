#include "guiadapter.h"

namespace m2 {
namespace gui {
namespace controler {

GuiAdapter GuiAdapter::instance;

GuiAdapter::GuiAdapter()
{

}

QVariant GuiAdapter::getGuid() const
{
    return guid;
}

void GuiAdapter::setGuid(const std::string &value)
{
    guid = QVariant::fromValue(QString::fromStdString(value));
}

GuiAdapter GuiAdapter::getInstance()
{
    return instance;
}

}
}
}
