#include "include/chats_model.h"

#include <QByteArray>





ChatsModel::ChatsModel(QObject *parent) : QAbstractListModel(parent)
{
    itemList.append(ChatData(
                        "0",

                        MessageData(
                            "1",
                            "Eba?",
                            "18:00"),

                        "Eba",
                        "/demo/dsa.jpg",
                        1));


    itemList.append(ChatData(
                        "1",
                        MessageData(
                            "1",
                            "Ili ne Eba?",
                            "17:00"),
                        "Ne Eba",
                        "/demo/dsa.jpg",
                        2));
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
        switch (role) {
        case ChatDataRoles::chatID:
            return itemList[index.row()].chatID;
        case ChatDataRoles::name:
            return itemList[index.row()].name;
        case ChatDataRoles::avatar:
            return itemList[index.row()].avatar;
        case ChatDataRoles::lastMessageGuid:
            return itemList[index.row()].lastMessage.fromUuid;
        case ChatDataRoles::lastMessageText:
            return itemList[index.row()].lastMessage.messText;
        case ChatDataRoles::lastMessageTime:
            return itemList[index.row()].lastMessage.messTime;
        case ChatDataRoles::unreadable:
            return itemList[index.row()].avatar;
        }
    }
    return QVariant();
}


bool ChatsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        switch (role)
        {
            case ChatDataRoles::chatID:
            {
                itemList[index.row()].chatID = value;
                break;
            }
            case ChatDataRoles::name:
            {
                itemList[index.row()].name = value;
                break;
            }
            case ChatDataRoles::avatar:
            {
                itemList[index.row()].avatar = value;
                break;
            }
            case ChatDataRoles::lastMessageGuid:
            {
                itemList[index.row()].lastMessage.fromUuid = value;
                break;
            }
            case ChatDataRoles::lastMessageText:
            {
                itemList[index.row()].lastMessage.messText = value;
                break;
            }
            case ChatDataRoles::lastMessageTime:
            {
                itemList[index.row()].lastMessage.messTime = value;
                break;
            }
            case ChatDataRoles::unreadable:
            {
                itemList[index.row()].avatar = value;
                break;
            }

            default:
                return false;
        }
    }
    return true;
}


QHash<int, QByteArray> ChatsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[chatID] = "chatID";
    roles[name] = "name";
    roles[avatar] = "avatar";
    roles[lastMessageGuid] = "lastMessageGuid";
    roles[lastMessageText] = "lastMessageText";
    roles[lastMessageTime] = "lastMessageTime";
    roles[unreadable] = "unreadable";

    return roles;
}


void ChatsModel::deleteChat(const QString& ID)
{
    int res = getChatByID(ID);
    if (res != -1)
        itemList.removeAt(res);
}


void ChatsModel::clearChatUnread(const QString& ID)
{
    int res = getChatByID(ID);
    if (res != -1)
        itemList[res].unreadable = QVariant(0);
}


void ChatsModel::updateChat(const QString& ID, const MessageData mess)
{
    int res = getChatByID(ID);
    if (res != -1)
        itemList[res].lastMessage = mess;
}


void ChatsModel::loadChatList()
{
    //TODO: заглушка
}


int ChatsModel::getChatByID(const QString& ID)
{
    for (int i = 0; i < itemList.size(); i++)
        if (itemList[i].chatID == ID)
            return i;

    return -1;
}


void ChatsModel::addNewChat(const QString& ID, const QString &name, QString avatar)
{
    beginInsertRows(QModelIndex(), itemList.size(), itemList.size());

    //TODO: заглушка
    itemList.append(ChatData(
                        ID,
                        MessageData(
                            "1",
                            "Eba?",
                            "18:"+QString::number(itemList.size())),

                        name,
                        "/demo/dsa.jpg",
                        0));


    endInsertRows();

    QModelIndex index = createIndex(0, 0, static_cast<void *>(0));
    emit dataChanged(index, index);
}
