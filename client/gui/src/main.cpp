#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "include/chats_filter_proxy_model.h"
#include "include/messages_model.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;    

    ChatsFilterProxyModel chatModel;
    chatModel.sort(0);
    MessagesModel messages;

    engine.rootContext()->setContextProperty("chatModel", &chatModel);
    engine.rootContext()->setContextProperty("messagesModel", &messages);

    return app.exec();
}
