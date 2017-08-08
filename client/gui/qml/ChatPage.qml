import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Page {
    id: chatPage
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent
        Page {
            id: header_chat
            Layout.preferredHeight: 75
            Layout.fillWidth: true
            z: 2

            RowLayout {
                id: topLayout
                anchors.fill: parent

                Image {
                    id: chatAvatar

                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.minimumHeight: 50
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: 50
                    Layout.maximumHeight: 50

                    fillMode: Image.PreserveAspectFit

                    //заглушка
                    source: "/demo/asd.jpg"
                }

                Text {
                    id: chatName

                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.minimumHeight: 50
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: -1
                    Layout.maximumHeight: 50

                    text: "Eba"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    renderType: Text.NativeRendering
                    font.pixelSize: 14
                }

                Button {
                    id: chatToolsButton

                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.minimumHeight: 50
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: 50
                    Layout.maximumHeight: 50

                    background: Rectangle {
                        radius: 10
                        color: parent.hovered ? "#a0dea0" : "white"
                    }

                    text: "⋮"
                    font.pixelSize: 22
                    font.bold: true
                }
            }
            Rectangle {
                id: split_right_chat
                color: "lightGray"
                height: 1
                width: parent.width
                anchors.top: topLayout.bottom
            }
        }

        /////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////

        Rectangle {
            id: split_footer_chat
            color: "lightGray"
            height: 1
            Layout.fillWidth: true
            anchors.bottom: footer_chat.top
        }


        Rectangle{
            anchors.fill: parent
            ChatListView{id: listView; model: ChatMessagesModel{id: messages}}
        }

        Page {
            id: footer_chat
            Layout.fillWidth: true

            RowLayout {
                id: bottomLayout
                anchors.fill: parent

                Button {
                    id: chatAddContent

                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.minimumHeight: 40
                    Layout.minimumWidth: 40
                    Layout.maximumWidth: 40
                    Layout.maximumHeight: 40
                    Layout.alignment: Qt.AlignBottom

                    background: Rectangle {
                        radius: 10

                        color: parent.hovered ? "#a0dea0" : "white"
                    }

                    contentItem: Image {
                        id: affiximg
                        source: "/img/affix.png"
                    }
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.minimumHeight: 30
                    Layout.minimumWidth: 30
                    Layout.maximumWidth: -1
                    Layout.maximumHeight: (chatPage.height - header_chat.height) / 3




                TextArea {
                    id: chatMessageField
                    selectByMouse: true

                    wrapMode: TextEdit.Wrap
                    renderType: Text.NativeRendering
                    Layout.alignment: Qt.AlignBottom
                    placeholderText: qsTr("Напишите сообщение...")

                    background: Rectangle {
                        radius: 10

                        color: parent.hovered ? "#a0dea0" : "white"
                    }
                }
                }
                Button {
                    id: chatSend

                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.minimumHeight: 40
                    Layout.minimumWidth: 40
                    Layout.maximumWidth: 40
                    Layout.maximumHeight: 40

                    contentItem: Image {
                        id: sendimg
                        source: "/img/send.png"
                    }

                    text: "send"

                    onClicked: {
                        messages.append({"guid": 1, "messText":chatMessageField.text, "messTime": "18:00"})
                        listView.positionViewAtEnd()
                        chatMessageField.clear()
                    }
                }
            }
        }
    }
}

