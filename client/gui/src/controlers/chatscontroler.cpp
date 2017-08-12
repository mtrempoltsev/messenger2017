#include <include/controlers/chatscontroler.h>

#include <QDebug>

using namespace m2::gui::controler;
using namespace ModelsElements;

ChatsControler::ChatsControler(QObject *parent) : QObject(parent)
{
    adapter = GuiAdapter::getGuiAdapter();

    chatsModel = adapter->getChatsModel();

    connect(adapter, SIGNAL(newMessage(ModelsElements::MessageData)), this, SLOT(newMessage(ModelsElements::MessageData)));
    connect(adapter, SIGNAL(newChat(ModelsElements::ChatData)), this, SLOT(newChat(ModelsElements::ChatData)));
    connect(adapter, SIGNAL(chatsLoaded(QHash<QString,ModelsElements::ChatData>)), this, SLOT(chatsLoaded(QHash<QString,ModelsElements::ChatData>)));

    chatsModel->setFilterKeyColumn(0);

    adapter->loadChats();
}


void ChatsControler::declareQML()
{
    qmlRegisterType<ChatsControler>("Controler.Chats", 1, 0, "ChatsControler");
}


void ChatsControler::changeCurrentChat(const int &index)
{
    QString nextChat = chatsModel->data(chatsModel->index(index, 0), ChatsModel::ChatDataRoles::ChatID).toString();

    qDebug()<<nextChat;
    if (adapter->getCurrentChatID() == nextChat)
        return;

    adapter->changeCurrentChat(nextChat);
    chatsModel->clearChatUnread(nextChat);
}


void ChatsControler::addNewChat(const QString &uuid)
{

}


void ChatsControler::newMessage(MessageData message)
{
    chatsModel->updateChat(message);
}


void ChatsControler::newChat(ChatData chat)
{

}


void ChatsControler::chatsLoaded(QHash<QString, ChatData> chats)
{
    foreach (auto chat, chats) {
        chatsModel->addNewChat(chat);
    }
}
