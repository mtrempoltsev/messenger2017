import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: mainpage
    //todod need remake
    anchors.fill: parent
    implicitHeight: 500
    implicitWidth: leftside.implicitWidth + rightside.implicitWidth + 2
    Page {
        id: leftside
        height: parent.height
        implicitWidth: info.implicitWidth
        Info {
            id: info

        }

        Rectangle {
            id: split_left
            color: "lightGray"
            height: 1
            width: parent.width
            anchors.top: info.bottom
        }

        /*
          контакты
          */
    }

    Rectangle {
        id: split_center
        color: "lightGray"
        width: 1
        height: parent.height

        anchors.left: leftside.right
    }

    ChatPage {
        id: rightside
        height: parent.height
        implicitWidth: 300
        anchors.left: split_center.right
        anchors.right: parent.right
    }

    AddDialog {
        id: adding
    }
}
