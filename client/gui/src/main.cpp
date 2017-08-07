#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <registrationcontroler.h>

using namespace m2::gui::controler;
#include <QQmlContext>
#include <include/chats_filter_proxy_model.h>
#include <include/contacts_model.h>
#include <include/messages_model.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    RegistrationControler::declareQML();
    QQmlApplicationEngine engine;

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    ChatsFilterProxyModel chats;
    MessagesModel messages;
    ContactsModel contacts;

    engine.rootContext()->setContextProperty("chatModel", &chats);
    engine.rootContext()->setContextProperty("messagesModel", &messages);
    engine.rootContext()->setContextProperty("contactsModel", &contacts);

    return app.exec();
}
