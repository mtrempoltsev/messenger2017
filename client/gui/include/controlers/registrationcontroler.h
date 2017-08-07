#ifndef REGISTRATIONCONTROLER_H
#define REGISTRATIONCONTROLER_H

#include <QObject>
namespace m2 {
namespace gui {
namespace controler {

class RegistrationControler : public QObject
{
    Q_OBJECT
public:
    explicit RegistrationControler(QObject* parent = nullptr);

    static void declareQML();
    Q_INVOKABLE void registerMe(const QString &server) const;

    void registrationSuccessed(const std::string &guid);

    void registrationFailed();

signals:
    void startRegister() const;
    void finishRegisterSuccessed() const;
    void finishRegisterFailed(const QString msg) const;

};
}
}
}


#endif // REGISTRATIONHANDLER_H
