#ifndef INCLUDEMESSAGES_MODEL_H
#define INCLUDEMESSAGES_MODEL_H

#include <QAbstractListModel>
#include <include/models/models_elements.h>


class MessagesModel : public QAbstractListModel
{
public:
    MessagesModel(QObject* parent = 0);
    ~MessagesModel();

    enum MessageDataRole {
        FromUuid = Qt::UserRole + 1,
        ChatUuid,
        MessText,
        MessTime
    };

    void addMessage(const ModelsElements::MessageData &mess);
    void loadMessageHistory();  //TODO: заглушка
    void cleanChat();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QVector <ModelsElements::MessageData> itemList;
};

#endif // INCLUDEMESSAGES_MODEL_H
