#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

#include <QAbstractListModel>
#include <QHash>

#include "include/models_elements.h"

enum ContactDataRoles
{
    uuid = Qt::UserRole + 1,
    name,
    avatar
};


class ContactsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ContactsModel();

    Q_INVOKABLE void loadContactList(); //TODO: заглушка
    Q_INVOKABLE const ContactData getContactByID(const QString &ID);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QHash <QString, ContactData> contacts;
};


#endif // CONTACTS_MODEL_H
