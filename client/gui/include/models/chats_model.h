#ifndef CHATS_MODEL_H
#define CHATS_MODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QString>

#include <include/models/models_elements.h>


class ChatsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ChatDataRoles {
        ChatID = Qt::UserRole + 1,
        Name,
        Avatar,
        LastMessageGuid,
        LastMessageText,
        LastMessageTime,
        Unreadable
    };


    ChatsModel(QObject *parent = 0);


    void addNewChat(const QString& ID, const QString &Name, QString Avatar = QString());
    void addNewChat(const ModelsElements::ChatData &chat);
    void deleteChat(const QString& ID);
    void updateChatMessage(const QString& ID, const ModelsElements::MessageData mess);
    void clearChatUnread(const QString& ID);

    QString getChatName(const QString &ID);
    QString getChatAvatar(const QString &ID);

    void loadChatList(QHash<QString, ModelsElements::ChatData> &chatList);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    ModelsElements::ChatData* getChatByID(const QString& ID);

    QHash <QString, ModelsElements::ChatData> itemList;
};

#endif // CHAT_MESSAGES_MODEL_H
