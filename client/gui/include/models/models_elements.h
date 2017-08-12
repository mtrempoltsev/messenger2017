#ifndef MODELS_ELEMENTS_H
#define MODELS_ELEMENTS_H

#include <QObject>
#include <QVariant>


namespace ModelsElements {
    struct MessageData
    {
        MessageData(){}
        MessageData(const QString &chatID, const QString &messGuid, const QString &messText, const QString &messTime);

        QVariant fromUuid;
        QVariant chatUuid;
        QVariant messText;
        QVariant messTime;
    };



    struct ChatData
    {
        ChatData(){}
        ChatData(const QString &ID, const MessageData &mess, const QString &chatName, QString chatAvatar, int chatUnreadable = 0);

        void setLastMessage(MessageData mess);

        QVariant chatID;
        QVariant name;
        QVariant avatar;
        MessageData lastMessage;
        QVariant unreadable;
    };



    struct ContactData
    {
        ContactData(){uuid = QVariant(QString("null"));}
        ContactData(const QString &userUuid, const QString &userName, const QString &userAvatar = QString());

        QVariant uuid;
        QVariant name;
        QVariant avatar;
    };
}



#endif // MODELS_ELEMENTS_H
