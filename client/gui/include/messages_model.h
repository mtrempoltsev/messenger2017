#ifndef INCLUDEMESSAGES_MODEL_H
#define INCLUDEMESSAGES_MODEL_H

#include <QAbstractListModel>
#include <include/models_elements.h>


class MessagesModel : public QAbstractListModel
{
public:
    MessagesModel(QObject* parent = 0);

    enum MessageDataRole {
        fromUuid = Qt::UserRole + 1,
        chatUuid,
        messText,
        messTime
    };

    Q_INVOKABLE void addMessage(MessageData mess);
    Q_INVOKABLE void loadMessageHistory();  //TODO: заглушка
    Q_INVOKABLE void setMessageState();     //TODO: заглушка

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QVector <MessageData> itemList;
};

#endif // INCLUDEMESSAGES_MODEL_H
