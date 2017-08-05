import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1


RowLayout {
    id: delegateItem

    readonly property bool sentByMe: guid == 1

    anchors.right: sentByMe ? parent.right : undefined
    spacing: 6
    Image{
        id: itemIcon

        fillMode: Image.PreserveAspectFit
        //            verticalAlignment: Image.AlignTop
        Layout.alignment: Qt.AlignTop
        //заглушка
        source: contacts.get(guid).avatar
    }


    Label {
        property real maxLen: root.width - itemIcon.width - delegateItem.spacing
        id:label
        text: model["messText"]
        wrapMode: Text.WrapAnywhere
        Layout.maximumWidth: {
            maxLen/* > paintedWidth? paintedWidth : maxLen*/
        }
        background: Rectangle {
            color: sentByMe ? "lightgrey" : "lightblue"
            width: label.maxLen > label.paintedWidth? label.paintedWidth : label.maxLen
        }

        color: sentByMe ? "black" : "white"
    }

    Item {
        Layout.fillWidth: true

    }

}
