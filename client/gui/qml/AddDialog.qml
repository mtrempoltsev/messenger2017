import QtQuick 2.9
import QtQuick.Controls 2.2

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
        color: "#ffffff"
        anchors.centerIn: adding
        radius: 10

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
        }

        Text {
            id: title
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
            x: 12
            y: 252
            width: 122
            height: 40
            text: qsTr("Отмена")

            background: Rectangle {
                radius: 5
                border.width: 0
                color: parent.hovered ? "#a0dea0" : "white"
            }

            onClicked: {
                adding.state = "Invisible"
            }
        }

        Button {
            id: add_button
            x: 166
            y: 252
            width: 122
            height: 40
            text: qsTr("Добавить")

            background: Rectangle {
                radius: 5
                border.width: 0
                color: parent.hovered ? "#a0dea0" : "white"
            }

            onClicked: {
                console.log("adding user " + guid_textField.text)
            }
        }

        Text {
            x: 12
            y: 88
            text: qsTr("Введите GUID пользователя:")
            font.pixelSize: 15
        }

        TextField {
            id: guid_textField
            x: 12
            y: 112
            width: 276
            height: 39
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            placeholderText: qsTr("GUID")
            echoMode: TextInput.Normal
            font.pixelSize: 18
        }
    }
}
