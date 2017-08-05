#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <registrationcontroler.h>

using namespace m2::gui::controler;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    RegistrationControler::declareQML();
    QQmlApplicationEngine engine;

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;





    return app.exec();
}
