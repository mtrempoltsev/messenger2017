#include "include/messages_model.h"



MessagesModel::MessagesModel(QObject *parent) : QAbstractListModel(parent)
{
    //TODO: заглушка

    itemList.append(MessageData(
                        "1",
                        "Eba?",
                        "10:00"
                        ));

    itemList.append(MessageData(
                        "0",
                        "Eba",
                        "10:10"
                        ));

    itemList.append(MessageData(
                        "1",
                        "Eba?1",
                        "11:00"
                        ));

    itemList.append(MessageData(
                        "1",
                        "Eba?2",
                        "12:00"
                        ));

    itemList.append(MessageData(
                        "0",
                        "Eba?0",
                        "13:00"
                        ));

    itemList.append(MessageData(
                        "0",
                        "Eba?12",
                        "14:00"
                        ));

    itemList.append(MessageData(
                        "1",
                        "Eba?14",
                        "15:00"
                        ));
}


void MessagesModel::addMessage(MessageData mess)
{
    itemList.append(mess);
}


void MessagesModel::loadMessageHistory()
{

}


int MessagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return itemList.size();
}


QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        switch (role) {
        case MessageDataRole::fromUuid:
            return itemList[index.row()].fromUuid;
        case MessageDataRole::chatUuid:
            return itemList[index.row()].chatUuid;
        case MessageDataRole::messText:
            return itemList[index.row()].messText;
        case MessageDataRole::messTime:
            return itemList[index.row()].messTime;
        }
    }
    return QVariant();
}


bool MessagesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        switch (role)
        {
            case MessageDataRole::fromUuid:
            {
                itemList[index.row()].fromUuid = value;
                break;
            }
            case MessageDataRole::chatUuid:
            {
                itemList[index.row()].chatUuid = value;
                break;
            }
            case MessageDataRole::messText:
            {
                itemList[index.row()].messText = value;
                break;
            }
            case MessageDataRole::messTime:
            {
                itemList[index.row()].messTime = value;
                break;
            }

            default:
                return false;
        }
    }
    return true;
}


QHash<int, QByteArray> MessagesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[fromUuid] = "fromUuid";
    roles[chatUuid] = "chatUuid";
    roles[messText] = "messText";
    roles[messTime] = "messTime";

    return roles;
}
