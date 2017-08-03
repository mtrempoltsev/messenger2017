import QtQuick 2.0
import QtQuick.Layouts 1.1

ListView{
    //заглушка
    ContactsList{id: contacts}

    property bool myMessagesDirection: true

    anchors.fill: parent
    anchors.bottomMargin: 20

    spacing: 10

    delegate: Item{
        id: viewDelegate

        anchors.left: parent.left
        anchors.right: parent.right
        //костыль?
        height: textlayout.height > 60 ? textlayout.height : 60
        width: parent.width

        Rectangle{
            id: messageRect
            anchors.fill: parent

            RowLayout{
                anchors.fill: parent
                id: itemRect

                layoutDirection: (myMessagesDirection == true && guid == 1) ? Qt.RightToLeft : Qt.LeftToRight

                Image{
                    id: itemIcon

                    Layout.fillHeight: true
                    Layout.minimumHeight: 60
                    Layout.minimumWidth: 60
                    Layout.maximumWidth: 60
                    Layout.maximumHeight: -1

                    fillMode: Image.PreserveAspectFit
                    verticalAlignment: Image.AlignTop

                    //заглушка
                    source: contacts.get(guid).avatar
                }

                Rectangle{
                    id: textRect
                    anchors.top: parent.top
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Layout.minimumHeight: -1
                    Layout.minimumWidth: 100
                    Layout.maximumWidth: textlayout.width
                    Layout.maximumHeight: -1

                    color: guid == 1 ? "#e0e0f0" : "#f0f0f0"

                    ColumnLayout{
                        id: textlayout
                        anchors.top: parent.top
                        anchors.right: parent.right

                        Text{
                            id: itemSender

                            Layout.fillWidth: true

                            Layout.leftMargin: 10
                            Layout.rightMargin: 10

                            Layout.minimumHeight: 10
                            Layout.minimumWidth: 50
                            Layout.maximumWidth: -1
                            Layout.maximumHeight: -1

                            verticalAlignment: Text.AlignTop
                            horizontalAlignment: (myMessagesDirection == true && guid == 1) ? Text.AlignRight:Text.AlignLeft
                            renderType: Text.NativeRendering
                            //wrapMode: Text.WrapAnywhere

                            font.pixelSize: 14

                            text: contacts.get(guid).name

                            //заглушка
                            color: guid == 1? "red":"#9d81ba"
                        }


                        Text {
                            id: itemText

                            Layout.fillWidth: true

                            Layout.leftMargin: 10
                            Layout.rightMargin: 10
                            Layout.bottomMargin: 10

                            Layout.minimumHeight: 15
                            Layout.minimumWidth: 50
                            Layout.maximumWidth: -1
                            Layout.maximumHeight: -1

                            horizontalAlignment: Text.AlignLeft
                            renderType: Text.NativeRendering
                            //wrapMode: Text.WrapAnywhere

                            font.pixelSize: 14

                            text: messText
                        }
                    }
                }



                Text{
                    id: itemTime

                    Layout.fillHeight: true
                    //Layout.fillWidth: true

                    Layout.rightMargin: 25
                    Layout.leftMargin: 25

                    Layout.minimumHeight: 11
                    Layout.minimumWidth: 30
                    Layout.maximumWidth: -1
                    Layout.maximumHeight: 30

                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 11

                    text: messTime
                    color: "#aaaaaa"
                }

                Rectangle{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: -1
                    Layout.minimumWidth: -1
                    Layout.maximumWidth: -1
                    Layout.maximumHeight: -1
                }
            }
        }
    }
}


