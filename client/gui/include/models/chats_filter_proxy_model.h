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

    void addNewChat(const QString& ID, const QString &Name, QString Avatar = QString());
    void addNewChat(const ModelsElements::ChatData &chat);
    void deleteChat(const QString& ID);
    void updateChat(const ModelsElements::MessageData mess);
    void clearChatUnread(const QString& ID);
    void loadChatList(QHash<QString, ModelsElements::ChatData> &chatList);    //TODO: заглушка

    QString getChatName(const QString &ID);
    QString getChatAvatar(const QString &ID);

    void setNameFilter(const QString &nameFilter);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    ChatsModel* model;

    QString currentFilter;
};

#endif // CHAT_FILTER_PROXY_MODEL_H
