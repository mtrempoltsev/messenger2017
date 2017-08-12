#include <include/controlers/messagescontroler.h>
#include <include/models/chats_filter_proxy_model.h>

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
    connect(adapter, SIGNAL(currentChatChanged(QString)), this, SLOT(changeChat()));

    changeChat();
}


void MessagesControler::declareQML()
{
    qmlRegisterType<MessagesControler>("Controler.Message", 1, 0, "MessagesControler");
}


void MessagesControler::sendNewMessage(const QString &text)
{
    MessageData newMess(adapter->getCurrentChatID(), adapter->getMyUuid(), text, QDateTime::currentDateTime().toString("hh:mm"));
    adapter->sendMessage(newMess);
}


void MessagesControler::getChatData()
{
    emit chatChanged(adapter->getCurrentChatName(), adapter->getCurrentChatAvatar());
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


void MessagesControler::receiveMessage(const MessageData &message)
{
    messagesModel->addMessage(message);
}


void MessagesControler::changeChat()
{
    messagesModel->cleanChat();
    loadHistory();

    getChatData();
}
