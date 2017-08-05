#ifndef REGISTRATIONCONTROLER_H
#define REGISTRATIONCONTROLER_H

#include <QObject>
namespace m2 {
Q_NAMESPACE
namespace gui {
Q_NAMESPACE
namespace controler {
Q_NAMESPACE

class RegistrationControler : public QObject
{
    Q_OBJECT
public:
    enum class States {
        NORMAL,
        LOADING,
        ERROR,
        SUCCESS
    };
    Q_ENUMS(States)

    static void declareQML();

    RegistrationControler();

    void registerMe(const QString &server) const;

    void registrationSuccessed(const std::string &guid);

    void registrationFailed();

signals:
    void changeRegistartionState(const States &) const;


};
}
}
}


#endif // REGISTRATIONHANDLER_H
