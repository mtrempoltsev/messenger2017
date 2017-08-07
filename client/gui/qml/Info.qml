import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: root
    implicitWidth: 300
    implicitHeight: 75
    padding: 8
    Image {
        id: avatar
        width: 50
        height: 50
        source: "/demo/ava.jpg"
    }

    Text {
        id: text_name
        text: qsTr("Владимир Пряхин")
        font.pixelSize: 20

        anchors.left: avatar.right
        anchors.leftMargin: 10

        width: 170
        elide: Text.ElideRight
    }

    Text {
        id: text_const_guid
        color: "#a8a8a8"
        text: qsTr("GUID:")

        anchors.top: text_name.bottom
        anchors.left: avatar.right
        anchors.leftMargin: 10
        anchors.topMargin: 5
    }

    Text {
        id: text_guid
        color: "#a8a8a8"
        text: qsTr("1111111111")

        anchors.top: text_name.bottom
        anchors.left: text_const_guid.right
        anchors.leftMargin: 5
        anchors.topMargin: 5
    }

    Button {
        id: button_settings
        width: 50
        height: 50

        anchors.right: parent.right
        padding: 3

        background: Rectangle {
            radius: 10
            color: parent.hovered ? "#a0dea0" : "white"
        }

        contentItem: Image {
            id: setimg
            source: "/img/settings.png"
        }

        onClicked: {
            //stackView.push("qrc:/qml/SettingsPage.qml")
            rightside.push("qrc:/qml/SettingsPage.qml")
        }
    }
}
