import QtQuick 2.9
import QtQuick.Controls 2.2
import "."

Page {
    id: mainpage
    //todod need remake
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
            color: Style.splitColor
            height: 1
            width: parent.width
            anchors.top: info.bottom
        }

        /*
          контакты
          */
        ContactWidget {
            id: contacts
            width: 300
            anchors.top: split_left.bottom
            height: parent.height - info.height - 1
        }
        SettingsPage {
            id: settings
            z: -1
            anchors.top: split_left.bottom
            height: parent.height - info.height - 1
            visible: false
        }
    }

    Rectangle {
        id: split_center
        color: Style.splitColor
        width: 1
        height: parent.height

        anchors.left: leftside.right
    }

    StackView {
        id: rightside
        z: -1
        height: parent.height
        implicitWidth: 300

        anchors.left: split_center.right
        anchors.right: parent.right

        initialItem: Page {
            Label {
                text: "Выберите чат"
                color: Style.textColor
                anchors.centerIn: parent
            }
            background: Rectangle {
                color: Style.mainBackground
            }
        }

        states: State {
            name: "noAnimation"
            PropertyChanges {
                target: animation
                duration: 0
            }
        }

        pushEnter: Transition {
            XAnimator {
                id: animation
                from: (rightside.mirrored ? 1 : -1) * -rightside.width
                to: 0
                duration: 400
                easing.type: Easing.OutCubic
            }
        }

        function pushNoAnimation(item) {
            state = "noAnimation"
            push(item)
            state = ""
        }
    }

    AddDialog {
        id: adding
    }
}
