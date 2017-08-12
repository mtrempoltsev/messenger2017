#include "include/models/contacts_model.h"

using namespace ModelsElements;


ContactsModel::ContactsModel(QObject *parent) : QAbstractListModel(parent)
{
    //TODO: заглушка
    contacts.insert("1", ContactData("1", "me", "/demo/dsa.jpg"));
    contacts.insert("0", ContactData("0", "Eba", "/demo/asd.jpg"));
}


void ContactsModel::loadContactList() //TODO: заглушка
{

}


ContactData* ContactsModel::getContactByID(const QString &ID)
{
    if (contacts.contains(ID))
        return &contacts[ID];

    return nullptr;
}


QVariant ContactsModel::getDataForID(const QString &ID, const int role)
{
    ContactData* elem = getContactByID(ID);
    if (elem != nullptr)
    {
        switch (role) {
        case ContactDataRoles::Uuid:
            return elem->uuid;
        case ContactDataRoles::Name:
            return elem->name;
        case ContactDataRoles::Avatar:
            return elem->avatar;
        }
    }

    return QVariant();
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
        case ContactDataRoles::Uuid:
            return iter.value().uuid;
        case ContactDataRoles::Name:
            return iter.value().name;
        case ContactDataRoles::Avatar:
            return iter.value().avatar;
        }
    }
    return QVariant();
}


QHash<int, QByteArray> ContactsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

    roles[Uuid] = "Uuid";
    roles[Name] = "Name";
    roles[Avatar] = "Avatar";

    return roles;
}

void ContactsModel::declareQML()
{
    qmlRegisterUncreatableType<ContactsModel>("ContactsModel", 1, 0, "ContactsModel", "none");
}
