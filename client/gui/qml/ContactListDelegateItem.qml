import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import ContactsModel 1.0
import "."

// list delegate
Component {
    id: contactDelegate
    Item {
        id: contactItem
        width: parent.width

        //seting start state
        state: "Normal"

        //for filtering
        visible: RegExp(searchText.text, "i").test(Name)
        height: visible ? 50 : 0

        // item view
        Column {
            Rectangle {
                id: itemRect

                width: contactItem.width
                height: contactItem.height - 1

                RowLayout {
                    Image {
                        id: itemIcon
                        Layout.maximumWidth: 40
                        Layout.maximumHeight: 40
                        Layout.margins: 5

                        fillMode: Image.PreserveAspectFit

                        source: Avatar
                    }

                    ColumnLayout {
                        RowLayout {
                            id: rowcontact
                            Layout.preferredWidth: contactItem.width - itemIcon.width - itemIcon.Layout.margins * 2 - 5
                            Text {
                                id: iconText

                                renderType: Text.NativeRendering
                                Layout.maximumWidth: rowcontact.width - rowcontact.spacing - lastTime.width - lastTime.Layout.rightMargin
                                font.pixelSize: 14
                                font.bold: true
                                elide: "ElideRight"

                                text: Name
                                color: Style.contactTextColor
                            }
                            Text {
                                id: lastTime
                                Layout.fillWidth: true
                                Layout.rightMargin: 5
                                renderType: Text.NativeRendering
                                font.pixelSize: 14

                                horizontalAlignment: Text.AlignRight

                                text: LastMessageTime == undefined ? "" : LastMessageTime
                                color: Style.contactSubText
                            }
                        }
                        RowLayout {
                            Text {
                                id: lastName

                                renderType: Text.NativeRendering
                                font.pixelSize: 14
                                text: contactsModel.getDataForID(LastMessageGuid, ContactsModel.Name) == undefined ? "" : contactsModel.getDataForID(LastMessageGuid, ContactsModel.Name) + ":"
                                color: Style.contactTextColor

                                visible: contactsModel.getDataForID(LastMessageGuid, ContactsModel.Name)  == undefined ? false : true
                            }
                            Text {
                                id: lastText

                                renderType: Text.NativeRendering
                                font.pixelSize: 14
                                elide: Text.ElideRight

                                text: LastMessageText == undefined ? "Нет сообщений" : LastMessageText
                                color: Style.contactSubText
                            }
                        }
                    }
                }
            }
            Rectangle {
                color: Style.splitColor
                height: 1
                width: contactItem.width
            }
        }

        //item states
        states: [
            State {
                name: "Normal"
                PropertyChanges {
                    target: itemRect
                    color: listView.currentIndex == index ? Style.active : Style.mainBackground
                }
                PropertyChanges {
                    target: lastText
                    color: listView.currentIndex == index ? Style.contactSubTextHover : Style.contactSubText
                }
                PropertyChanges{
                    target: lastTime
                    color: listView.currentIndex == index ? Style.contactSubTextHover : Style.contactSubText
                }
            },
            State {
                name: "Hover"
                PropertyChanges {
                    target: itemRect
                    color: listView.currentIndex == index ? Style.hoverActive : Style.hover
                }
                PropertyChanges {
                    target: lastText
                    color: Style.contactSubTextHover
                }
                PropertyChanges{
                    target: lastTime
                    color: Style.contactSubTextHover
                }
            }
        ]

        //item states transitions
        transitions: [
            Transition {
                from: "*"
                to: "Normal"
                ColorAnimation {
                    duration: 100
                }
            },
            Transition {
                from: "*"
                to: "Hover"
                ColorAnimation {
                    duration: 100
                }
            }
        ]

        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                contactItem.state = 'Hover'
            }
            onExited: {
                contactItem.state = "Normal"
            }

            onClicked: {
                listView.currentIndex = index
            }
        }
    }
}
