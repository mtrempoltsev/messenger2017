#ifndef CHATS_MODEL_H
#define CHATS_MODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QString>

#include <include/models_elements.h>


class ChatsModel : public QAbstractListModel
{
    Q_OBJECT

public:    
    enum ChatDataRoles {
        chatID = Qt::UserRole + 1,
        name,
        avatar,
        lastMessageGuid,
        lastMessageText,
        lastMessageTime,
        unreadable
    };

    ChatsModel(QObject *parent = 0);


    Q_INVOKABLE void addNewChat(const QString& ID, const QString &name, QString avatar = QString());
    Q_INVOKABLE void deleteChat(const QString& ID);
    Q_INVOKABLE void updateChat(const QString& ID, const MessageData mess);
    Q_INVOKABLE void clearChatUnread(const QString& ID);
    Q_INVOKABLE void loadChatList();    //TODO: заглушка

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    int getChatByID(const QString& ID);

    QVector <ChatData> itemList;

};

#endif // CHAT_MESSAGES_MODEL_H
