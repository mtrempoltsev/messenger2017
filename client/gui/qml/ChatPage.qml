import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Controler.Message 1.0


Page {
    id: chatPage
    background: Rectangle {
        color: Style.mainBackground
    }


    MessagesControler{
        id: messagesControler

        onChatChanged: {
            chatName.text = name
            chatAvatar.source = avatar
        }

        Component.onCompleted: {
            messagesControler.getChatData()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Page {
            id: header_chat
            Layout.preferredHeight: 75
            Layout.fillWidth: true
            z: 2

            background: Rectangle {
                color: Style.mainBackground
            }

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
                    //source: "/demo/asd.jpg"
                }

                Text {
                    id: chatName

                    Layout.fillWidth: true
                    Layout.margins: 5
                    Layout.minimumHeight: 50
                    Layout.minimumWidth: 50
                    Layout.maximumWidth: -1
                    Layout.maximumHeight: 50

                    //заглушка
                    //text: "Eba"
                    color: Style.textColor
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
                        color: parent.hovered ? Style.hover : Style.mainBackground
                    }

                    contentItem: Text {
                        text: "⋮"
                        color: Style.buttonColor
                        font.pixelSize: 22
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Rectangle {
                id: split_right_chat
                color: Style.splitColor
                height: 1
                width: parent.width
                anchors.top: topLayout.bottom
            }
        }


        /////////////////////////////////////////////////////////////////////
        ChatListView {
            id: chatListView

            Layout.fillWidth: true
            Layout.fillHeight: true
            displayMarginBeginning: 40
            displayMarginEnd: 40

            model: messagesModel
        }


        //////////////////////////////////////////////////////////////////////
        Rectangle {
            id: split_footer_chat
            color: Style.splitColor
            height: 1
            Layout.fillWidth: true
            anchors.bottom: footer_chat.top
        }

        Page {
            id: footer_chat
            Layout.fillWidth: true

            background: Rectangle {
                color: Style.mainBackground
            }

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

                        color: parent.hovered ? Style.hover : Style.mainBackground
                    }

                    contentItem: Image {
                        id: affiximg
                        source: "/img/affix.png"
                    }

                    ColorOverlay {
                        anchors.fill: affiximg
                        source: affiximg
                        color: Style.buttonColor
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
                        placeholderText: qsTr("Напишите сообщение...")
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
                    Layout.alignment: Qt.AlignBottom

                    background: Rectangle {
                        radius: 10

                        color: parent.hovered ? Style.hover : Style.mainBackground
                    }

                    contentItem: Image {
                        id: sendimg
                        source: "/img/send.png"
                    }

                    ColorOverlay {
                        anchors.fill: sendimg
                        source: sendimg
                        color: Style.buttonColor
                    }

                    onClicked: {
                        messagesControler.sendNewMessage(chatMessageField.text)
                        chatListView.positionViewAtEnd()
                        chatMessageField.clear()
                    }
                }
            }
        }
    }
}
