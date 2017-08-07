#include "include/contacts_model.h"


ContactsModel::ContactsModel(QObject *parent) : QAbstractListModel(parent)
{

}


void ContactsModel::loadContactList() //TODO: заглушка
{

}


const ContactData ContactsModel::getContactByID(const QString &ID)
{
    if (contacts.contains(ID))
        return contacts[ID];

    return ContactData();
}


int ContactsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return contacts.size();
}


QVariant ContactsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    {
        QHash<QString, ContactData>::const_iterator iter = contacts.constBegin() + index.row();

        switch (role) {
        case ContactDataRoles::uuid:
            return iter.value().uuid;
        case ContactDataRoles::name:
            return iter.value().name;
        case ContactDataRoles::avatar:
            return iter.value().avatar;
        }
    }
    return QVariant();
}


QHash<int, QByteArray> ContactsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[uuid] = "uuid";
    roles[name] = "name";
    roles[avatar] = "avatar";

    return roles;
}
