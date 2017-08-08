#include <include/models_elements.h>

MessageData::MessageData(const QString &messGuid, const QString messText, const QString messTime) :
    fromUuid(messGuid),
    messText(messText),
    messTime(messTime)
{}



ChatData::ChatData(const QString &ID, const MessageData &mess, const QString &chatName, QString chatAvatar, int chatUnreadable):
    chatID(ID),
    lastMessage(mess),
    name(chatName),
    avatar(chatAvatar),
    unreadable(chatUnreadable)
{}


ContactData::ContactData(const QString &userUuid, const QString &userName, const QString &userAvatar) :
    uuid(userUuid),
    name(userName),
    avatar(userAvatar)
{}
