#include "include/models/chats_filter_proxy_model.h"

using namespace ModelsElements;


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
    model->updateChatMessage(ID, mess);
}


void ChatsFilterProxyModel::clearChatUnread(const QString& ID)
{
    model->clearChatUnread(ID);
}


void ChatsFilterProxyModel::loadChatList(QHash<QString, ModelsElements::ChatData> &chatList)
{
    model->loadChatList(chatList);
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
        if (model->data(model->index(sourceRow, 0, sourceParent), ChatsModel::ChatDataRoles::Name).toString().startsWith(currentFilter, Qt::CaseInsensitive))
            return true;
    }
    else
        return true;

    return false;
}


bool ChatsFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (model->data(left, ChatsModel::ChatDataRoles::LastMessageTime).toString() > model->data(right, ChatsModel::ChatDataRoles::LastMessageTime).toString())
        return true;

    return false;
}
