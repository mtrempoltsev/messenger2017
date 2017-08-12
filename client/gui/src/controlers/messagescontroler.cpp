#include <include/controlers/messagescontroler.h>

#include <QtQml>
#include <QDate>

using namespace m2::gui::controler;
using namespace ModelsElements;

MessagesControler::MessagesControler(QObject *parent) : QObject(parent)
{
    adapter = GuiAdapter::getGuiAdapter();
    messagesModel = adapter->getMessagesModel();

    connect(adapter, SIGNAL(newMessage(ModelsElements::MessageData)), this, SLOT(receiveMessage(ModelsElements::MessageData)));
    connect(adapter, SIGNAL(messagesLoaded(QVector<ModelsElements::MessageData>)), this, SLOT(loadChat(QVector<ModelsElements::MessageData>)));

    loadHistory();
}


void MessagesControler::declareQML()
{
    qmlRegisterType<MessagesControler>("Controler.Message", 1, 0, "MessagesControler");
}


void MessagesControler::sendNewMessage(const QString &text)
{
    MessageData newMess(adapter->getMyUuid(), adapter->getCurrentChatID(), text, QDateTime::currentDateTime().toString("hh:mm"));
    messagesModel->addMessage(newMess);
    adapter->sendMessage(newMess);
}


void MessagesControler::loadHistory()
{
    adapter->loadChatHistory();
}


void MessagesControler::loadChat(QVector<ModelsElements::MessageData> messages)
{
    for (int i = 0; i < messages.size(); i++)
        messagesModel->addMessage(messages[i]);
}


void MessagesControler::receiveMessage(MessageData message)
{
    messagesModel->addMessage(message);
}
