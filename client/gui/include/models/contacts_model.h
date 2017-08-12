#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QtQml>


#include "include/models/models_elements.h"


class ContactsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ContactDataRoles
    {
        Uuid = Qt::UserRole + 1,
        Name,
        Avatar
    };
    Q_ENUMS(ContactDataRoles)

    ContactsModel(QObject* parent = 0);

    void loadContactList(); //TODO: заглушка
    ModelsElements::ContactData *getContactByID(const QString &ID);

    Q_INVOKABLE QVariant getDataForID(const QString &ID, const int role);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    static void declareQML();

private:
    QHash <QString, ModelsElements::ContactData> contacts;
};


#endif // CONTACTS_MODEL_H
