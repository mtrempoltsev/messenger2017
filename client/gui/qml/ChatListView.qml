import QtQuick 2.0
import QtQuick.Layouts 1.1

ListView{
    id: listView

    //заглушка
    ContactsList{id: contacts}

    anchors.fill: parent

    model: ChatMessagesModel{}
    spacing: 30

    delegate: Item{
        id: viewDelegate

        width: listView.width
        //костыль?
        height: itemSender.height + itemText.height

        Rectangle{
            anchors.fill: parent

            RowLayout{
                anchors.fill: parent
                id: itemRect

                layoutDirection: guid == 1? Qt.RightToLeft: Qt.LeftToRight

                Image{
                    id: itemIcon

                    Layout.fillHeight: true
                    Layout.margins: 5
                    Layout.minimumHeight: 50
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: 50
                    Layout.maximumHeight: -1

                    fillMode: Image.PreserveAspectFit
                    verticalAlignment: Image.AlignTop

                    //заглушка
                    source: contacts.get(guid).avatar
                }

                ColumnLayout{
                    id: textlayout

                    Layout.minimumHeight: -1
                    Layout.minimumWidth: -1
                    Layout.maximumWidth: -1
                    Layout.maximumHeight: -1

                    Text{
                        id: itemSender

                        Layout.fillWidth: true

                        verticalAlignment: Text.AlignTop
                        renderType: Text.NativeRendering
                        font.pixelSize: 14

                        text: contacts.get(guid).name

                        //заглушка
                        color: guid == 1? "red":"black"
                    }

                    Text {
                        id: itemText

                        Layout.fillWidth: true

                        Layout.minimumHeight: 14
                        Layout.minimumWidth: 50
                        Layout.maximumWidth: -1
                        Layout.maximumHeight: -1

                        renderType: Text.NativeRendering
                        font.pixelSize: 14

                        text: messText
                    }
                }


                Text{
                    id: itemTime

                    Layout.fillHeight: true
                    Layout.rightMargin: 5
                    Layout.minimumHeight: 11
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: -1
                    Layout.maximumHeight: -1

                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignTop
                    font.pixelSize: 11

                    text: messTime
                    color: "#aaaaaa"
                }
            }
        }
    }
}


