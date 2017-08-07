#ifndef LOGINCONTROLER_H
#define LOGINCONTROLER_H

#include <QObject>

namespace m2 {
namespace gui {
namespace controler {


class LoginControler : public QObject
{
    Q_OBJECT
public:
    explicit LoginControler(QObject *parent = nullptr);

    static void declareQML();

    Q_INVOKABLE void login() const;

    void loginSuccessed(const std::string &guid);
    void loginFailed();

signals:
    void startLogin() const;
    void finishLoginSuccessed() const;
    void finishLoginFailed(const QString &msg) const;

};

}
}
}

#endif // LOGINCONTROLER_H
