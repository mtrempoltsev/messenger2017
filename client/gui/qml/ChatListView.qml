import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

ListView{
    //заглушка
    id: msgListView
    ContactsList{id: contacts}

    property bool myMessagesDirection: false

    anchors.fill: parent
    anchors.bottomMargin: 20

    spacing: 10

    delegate: ChatListDelegateItem {
        id: delegateItem
    }
}
