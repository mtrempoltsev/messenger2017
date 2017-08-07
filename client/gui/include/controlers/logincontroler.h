#ifndef LOGINCONTROLER_H
#define LOGINCONTROLER_H

#include <QObject>

class LoginControler : public QObject
{
    Q_OBJECT
public:
    explicit LoginControler(QObject *parent = nullptr);

signals:

public slots:
};

#endif // LOGINCONTROLER_H