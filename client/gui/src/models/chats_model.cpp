#include "include/models/chats_model.h"

#include <QByteArray>

using namespace ModelsElements;

ChatsModel::ChatsModel(QObject *parent) : QAbstractListModel(parent)
{
    itemList.insert("0",ChatData(
                        "0",

                        MessageData(
                            "1", "0",
                            "Eba?",
                            "18:00"),

                        "Eba",
                        "/demo/dsa.jpg",
                        1));
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
        case ChatDataRoles::ChatID:
            return iter.value().chatID;
        case ChatDataRoles::Name:
            return iter.value().name;
        case ChatDataRoles::Avatar:
            return iter.value().avatar;
        case ChatDataRoles::LastMessageGuid:
            return iter.value().lastMessage.fromUuid;
        case ChatDataRoles::LastMessageText:
            return iter.value().lastMessage.messText;
        case ChatDataRoles::LastMessageTime:
            return iter.value().lastMessage.messTime;
        case ChatDataRoles::Unreadable:
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
        itemList.remove(ID);
}


void ChatsModel::clearChatUnread(const QString& ID)
{
    if (itemList.contains(ID))
        itemList[ID].unreadable = QVariant(0);
}


void ChatsModel::updateChatMessage(const QString& ID, const MessageData mess)
{
    if (itemList.contains(ID))
        itemList[ID].lastMessage = mess;
}


void ChatsModel::loadChatList(QHash<QString, ModelsElements::ChatData> &chatList)
{
    itemList = chatList;
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

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}
