#ifndef CHATS_FILTER_PROXY_MODEL_H
#define CHATS_FILTER_PROXY_MODEL_H

#include <QSortFilterProxyModel>
#include <QObject>
#include "chats_model.h"


class ChatsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    ChatsFilterProxyModel(QObject* parent = 0);

    Q_INVOKABLE void addNewChat();  //TODO: заглушка
    Q_INVOKABLE void deleteChat(const QString& ID);
    Q_INVOKABLE void updateChat(const QString& ID, const MessageData mess);
    Q_INVOKABLE void clearChatUnread(const QString& ID);
    Q_INVOKABLE void loadChatList();    //TODO: заглушка


    Q_INVOKABLE void setNameFilter(const QString &nameFilter);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    ChatsModel* model;

    QString currentFilter;
};

#endif // CHAT_FILTER_PROXY_MODEL_H
