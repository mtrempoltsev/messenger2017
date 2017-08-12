import QtQuick 2.9
import QtQuick.Controls 2.2
import "."

Rectangle {
    id: adding
    color: Qt.rgba(0.0, 0.0, 0.0, 0.5)
    anchors.fill: parent
    state: "Invisible"

    states: [
        State {
            name: "Visible"
            PropertyChanges {
                target: adding
                opacity: 1.0
            }
            PropertyChanges {
                target: adding
                visible: true
            }
        },
        State {
            name: "Invisible"
            PropertyChanges {
                target: adding
                opacity: 0.0
            }
            PropertyChanges {
                target: adding
                visible: false
            }
        }
    ]
    transitions: [
        Transition {
            from: "Visible"
            to: "Invisible"

            SequentialAnimation {
                NumberAnimation {
                    target: adding
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.InOutQuad
                }

                NumberAnimation {
                    target: adding
                    property: "visible"
                    duration: 0
                }
            }
        },
        Transition {
            from: "Invisible"
            to: "Visible"
            SequentialAnimation {
                NumberAnimation {
                    target: adding
                    property: "visible"
                    duration: 0
                }
                NumberAnimation {
                    target: adding
                    property: "opacity"
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
        }
    ]

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        onClicked: {
            adding.state = "Invisible"
        }
    }

    Rectangle {
        id: dialog
        width: 300
        height: 300
        color: Style.mainBackground
        anchors.centerIn: adding
        radius: 10

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        }

        Text {
            id: title
            color: Style.textColor
            font.pointSize: 20
            text: qsTr("Добавить пользователя")
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: split_title
            color: "lightGray"
            height: 1
            width: 300
            anchors.top: title.bottom
        }

        Button {
            id: cancel_button
            width: 130
            height: 40

            contentItem: Text {
                text: qsTr("Отмена")
                color: Style.buttonColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8

            background: Rectangle {
                radius: 5
                border.width: 0
                color: parent.hovered ? Style.hover : Style.mainBackground
            }

            onClicked: {
                adding.state = "Invisible"
            }
        }


        Button {
            id: add_button
            width: 130
            height: 40

            contentItem: Text {
                text: qsTr("Добавить")
                color: Style.buttonColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8

            background: Rectangle {
                radius: 5
                border.width: 0
                color: parent.hovered ? Style.hover : Style.mainBackground
            }

            onClicked: {
                console.log("adding user " + guid_textField.text)
            }
        }


        Text {
            anchors.left: guid_textField.left
            anchors.bottom: guid_textField.top
            anchors.bottomMargin: 5

            text: qsTr("Введите GUID пользователя:")
            color: Style.textColor
            font.pixelSize: 15
        }

        TextField {
            id: guid_textField
            anchors.centerIn: parent
            anchors.leftMargin: 12
            width: 276
            height: 39
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            placeholderText: qsTr("GUID")
            echoMode: TextInput.Normal
            font.pixelSize: 18
        }
    }
}
