import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    width: 300
    height: 75
    Image {
        id: avatar
        x: 8
        y: 8
        width: 50
        height: 50
        source: "/demo/ava.jpg"
    }

    Text {
        id: text_name
        x: 64
        y: 8
        width: 84
        height: 24
        text: qsTr("My name")
        font.pixelSize: 20
    }

    Text {
        id: text_const_guid
        x: 64
        y: 40
        width: 33
        height: 15
        color: "#a8a8a8"
        text: qsTr("GUID:")
        font.pixelSize: 12
    }

    Text {
        id: text_guid
        x: 103
        y: 40
        width: 64
        height: 15
        color: "#a8a8a8"
        text: qsTr("1111111111")
        font.pixelSize: 12
    }

    Button {
        id: button_settings
        x: 239
        y: 8
        width: 50
        height: 50
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
