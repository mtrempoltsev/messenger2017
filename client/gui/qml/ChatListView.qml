import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

ListView{
    //заглушка
    id: msgListView

    property bool myMessagesDirection: true

    spacing: 10

    ScrollBar.vertical: ScrollBar{
        id: scroll
    }

    delegate: ChatListDelegateItem {
        id: delegateItem
    }
}
