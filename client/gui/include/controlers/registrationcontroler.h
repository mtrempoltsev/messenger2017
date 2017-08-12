#ifndef REGISTRATIONCONTROLER_H
#define REGISTRATIONCONTROLER_H

#include <QObject>
#include <QVariant>


namespace m2 {
namespace gui {
namespace controler {

class RegistrationControler : public QObject
{
    Q_OBJECT
public:
    explicit RegistrationControler(QObject* parent = nullptr);

    static void declareQML();
    Q_INVOKABLE void registerMe(const int &serverIndex) const;
    Q_INVOKABLE QVariant getServerList();

signals:
    void startRegister(const int &serverIndex) const;
    void finishRegisterSuccessed() const;
    void finishRegisterFailed(const QString msg) const;

};
}
}
}


#endif // REGISTRATIONHANDLER_H
