#include "include/models/chats_model.h"

#include <QDebug>

using namespace ModelsElements;

ChatsModel::ChatsModel(QObject *parent) : QAbstractListModel(parent)
{

}


int ChatsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return itemList.size();
}


QVariant ChatsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        QHash<QString, ChatData>::const_iterator iter = itemList.constBegin() + index.row();

        switch (role) {
        case ChatID:
            return iter.value().chatID;
        case Name:
            return iter.value().name;
        case Avatar:
            return iter.value().avatar;
        case LastMessageGuid:
            return iter.value().lastMessage.fromUuid;
        case LastMessageText:
            return iter.value().lastMessage.messText;
        case LastMessageTime:
            return iter.value().lastMessage.messTime;
        case Unreadable:
            return iter.value().avatar;
        }
    }

    return QVariant();
}


QHash<int, QByteArray> ChatsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[ChatID] = "ChatID";
    roles[Name] = "Name";
    roles[Avatar] = "Avatar";
    roles[LastMessageGuid] = "LastMessageGuid";
    roles[LastMessageText] = "LastMessageText";
    roles[LastMessageTime] = "LastMessageTime";
    roles[Unreadable] = "Unreadable";

    return roles;
}


void ChatsModel::deleteChat(const QString& ID)
{
    if (itemList.contains(ID))
    {
        beginRemoveRows(index(0,0), 0, itemList.size() - 1);
        itemList.remove(ID);
        endRemoveRows();
    }
}


void ChatsModel::clearChatUnread(const QString& ID)
{
    if (itemList.contains(ID))
    {
        itemList[ID].unreadable = QVariant(0);
        emit dataChanged(index(0,0), index(itemList.size() - 1,0));
    }
}


QString ChatsModel::getChatName(const QString &ID)
{
    return getChatByID(ID)->name.toString();
}


QString ChatsModel::getChatAvatar(const QString &ID)
{
    return getChatByID(ID)->avatar.toString();
}


void ChatsModel::updateChatMessage(const QString& ID, const MessageData mess)
{
    if (itemList.contains(ID))
    {
        itemList[ID].lastMessage = mess;
        emit dataChanged(index(0,0), index(itemList.size() - 1,0));
    }
}


void ChatsModel::loadChatList(QHash<QString, ModelsElements::ChatData> &chatList)
{
    itemList = chatList;
    emit dataChanged(index(0,0), index(itemList.size() - 1,0));
}


ChatData *ChatsModel::getChatByID(const QString& ID)
{
    if (itemList.contains(ID))
        return &itemList[ID];

    return nullptr;
}


void ChatsModel::addNewChat(const QString& ID, const QString &name, QString avatar)
{
    beginInsertRows(QModelIndex(), itemList.size(), itemList.size());

    itemList.insert(ID ,
                    ChatData(
                        ID,
                        MessageData(),
                        name,
                        avatar,
                        0));

    endInsertRows();
}


void ChatsModel::addNewChat(const ChatData &chat)
{
    beginInsertRows(QModelIndex(), itemList.size(), itemList.size());

    itemList.insert(chat.chatID.toString(), chat);

    endInsertRows();
}
