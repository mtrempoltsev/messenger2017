#ifndef GUIADAPTER_H
#define GUIADAPTER_H

#include <QVariant>

namespace m2 {
namespace gui {


class GuiAdapter
{
    GuiAdapter();

    QVariant guid;

    static GuiAdapter instance;
public:
    static GuiAdapter getInstance();


    QVariant getGuid() const;
    void setGuid(const std::string &value);
};

#endif // GUIADAPTER_H


}
}
