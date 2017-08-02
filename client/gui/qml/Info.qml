import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    width: 300
    height: 75
    padding: 8
    Image {
        id: avatar
        width: 50
        height: 50
        source: "/demo/ava.jpg"
    }

    Text {
        id: text_name
        width: 84
        height: 24
        text: qsTr("My name")
        font.pixelSize: 20

        anchors.left: avatar.right
        anchors.leftMargin: 10
    }

    Text {
        id: text_const_guid
        width: 33
        height: 15
        color: "#a8a8a8"
        text: qsTr("GUID:")
        font.pixelSize: 12

        anchors.top: text_name.bottom
        anchors.left: avatar.right
        anchors.leftMargin: 10
        anchors.topMargin: 5
    }

    Text {
        id: text_guid
        width: 64
        height: 15
        color: "#a8a8a8"
        text: qsTr("1111111111")
        font.pixelSize: 12

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
            border.width: 0
            color: parent.hovered ? "#a0dea0" : "white"
        }

        contentItem: Image {
            id: setimg
            width: 50
            height: 50
            source: "/img/settings.png"
        }

        onClicked: {
            console.log("loading settings page")
        }
    }
}
