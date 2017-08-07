#ifndef REGISTRATIONHANDLER_H
#define REGISTRATIONHANDLER_H

#include <QObject>
namespace m2 {
namespace gui {

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
    void changeRegistartionState(const States &);


};
}
}



#endif // REGISTRATIONHANDLER_H
