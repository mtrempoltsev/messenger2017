#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <registrationcontroler.h>

using namespace m2::gui::controler;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;



    qmlRegisterType<RegistrationControler>("controler.registration", 1, 0, "RegistraionControler");

    return app.exec();
}
