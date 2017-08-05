import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1


RowLayout {
    function getMsgAligment() {
        if (msgListView.myMessagesDirection)
            return sentByMe ? Qt.AlignRight : Qt.AlignLeft
        return Qt.AlignLeft
    }

    function getLayoutDirection() {
        if (msgListView.myMessagesDirection)
            return sentByMe ? Qt.RightToLeft : Qt.LeftToRight
        return Qt.LeftToRight
    }

    id: delegateItem
    readonly property bool sentByMe: guid == 1
    layoutDirection: getLayoutDirection()
    Layout.alignment: getMsgAligment()
    width: parent.width

    spacing: 6

    Image{
        id: itemIcon

        fillMode: Image.PreserveAspectFit
        Layout.alignment: Qt.AlignTop
        //заглушка
        source: contacts.get(guid).avatar
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignTop
        RowLayout {
            Layout.alignment: Qt.AlignTop | getMsgAligment()
            Text {
                id: itemUserName
                text: contacts.get(model.guid).name
            }

            Text {
                id: itemTime
                text: model.messTime
            }
        }



        Label {
            property real maxLen: delegateItem.width - itemIcon.width - delegateItem.spacing -2
            id:msgLabel
            text: model.messText
            wrapMode: Text.WrapAnywhere
            Layout.maximumWidth: {
                maxLen
            }
            background: Rectangle {
                id: msgLabelBackground
                color: sentByMe ? "lightgrey" : "lightblue"
                width: msgLabel.maxLen > msgLabel.paintedWidth? msgLabel.paintedWidth : msgLabel.maxLen
            }

            color: sentByMe ? "black" : "white"
        }
    }




    Item {
        Layout.fillWidth: true
    }
}
