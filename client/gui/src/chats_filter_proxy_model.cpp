#include "include/chats_filter_proxy_model.h"
#include <QDebug>

ChatsFilterProxyModel::ChatsFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
    model = new ChatsModel(this);
    this->setSourceModel(model);
}

void ChatsFilterProxyModel::addNewChat()
{
    //TODO: заглушка
    model->addNewChat("asd", "New Eba");
}


void ChatsFilterProxyModel::deleteChat(const QString& ID)
{
    model->deleteChat(ID);
}


void ChatsFilterProxyModel::updateChat(const QString& ID, const MessageData mess)
{
    model->updateChat(ID, mess);
}


void ChatsFilterProxyModel::clearChatUnread(const QString& ID)
{
    model->clearChatUnread(ID);
}


void ChatsFilterProxyModel::loadChatList()    //заглушка
{
    model->loadChatList();
}


void ChatsFilterProxyModel::setNameFilter(const QString &nameFilter)
{    
    currentFilter = nameFilter;
    filterChanged();
}


bool ChatsFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!currentFilter.isEmpty())
    {
        if (model->data(model->index(sourceRow, 0, sourceParent), ChatsModel::ChatDataRoles::name).toString().startsWith(currentFilter, Qt::CaseInsensitive))
            return true;
    }
    else
        return true;

    return false;
}


bool ChatsFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (model->data(left, ChatsModel::ChatDataRoles::lastMessageTime).toString() > model->data(right, ChatsModel::ChatDataRoles::lastMessageTime).toString())
        return true;

    return false;
}
