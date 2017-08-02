import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: mainpage
    width: parent.width
    height: parent.height

    Page {
        id: leftside
        x: 0
        width: 300
        height: parent.height

        Info {
            id: info
        }

        Rectangle {
            id: split_left
            color: "lightGray"
            height: 1
            width: 300
            anchors.top: info.bottom
        }

        ContactWidget{
            id: contacts
            width: 300
            anchors.top: split_left.bottom
            height: parent.height-info.height-1
        }
    }

    Rectangle {
        id: split_center
        color: "lightGray"
        width: 1
        height: parent.height

        anchors.left: leftside.right
    }

    StackView {
        id: rightside
        z:-1
        height: parent.height

        anchors.left: split_center.right
        anchors.right: parent.right

        initialItem: Page {
            Label {
                text: "Выберите чат"
                anchors.centerIn: parent
            }
        }
    }

    AddDialog {
        id: adding
    }
}
