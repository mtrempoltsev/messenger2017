import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

Rectangle {
    antialiasing: false

    //top rect
    Rectangle {
        z:1
        id: searchline
        height: 40
        width: parent.width

        //search rect
        TextField {
            id: searchText
            height: 30
            width: parent.parent.width - 45
            x: 5
            y: 5

            placeholderText: qsTr("Поиск")
        }

        //add dialog button
        Button {
            id: addDialogButton

            width: 30
            height: 30
            x: parent.width - 35
            y: 5

                background: Rectangle {
                    radius: 5
                    border.width: 0
                    color: parent.hovered ? "#a0dea0" : "white"
                }
            topPadding: 5
            bottomPadding: 5
            leftPadding: 5
            rightPadding: 5

            contentItem: Image {
                id: newimg
                width: 10
                height: 10
                source: "/img/new_message.png"
            }

            onClicked: {
                adding.state = "Visible"
            }
        }
    }

    // list delegate
    Component {
        id: contactDelegate
        Item {
            id: contactItem
            width: parent.width

            //seting start state
            state: "Normal"

            //for filtering
            visible: regExp(searchText.text, "i").test(name)
            height: visible ? 50 : 0

            // item view
            Column {
                Rectangle {
                    id: itemRect

                    width: contactItem.width
                    height: contactItem.height-1

                    Image {
                        id: itemIcon
                        width: 40
                        height: 40
                        x: 5
                        y: 5

                        fillMode: Image.PreserveAspectFit

                        source: icon
                    }

                    Text {
                        id: iconText
                        height: 40
                        x: 60
                        y: 5

                        verticalAlignment: Text.AlignVCenter
                        renderType: Text.NativeRendering
                        font.pixelSize: 14

                        text: name
                    }
                }
                Rectangle {
                    color: "lightGray"
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
                        color: listView.currentIndex == index ? "#a0dea0" : "white"
                    }
                },
                State {
                    name: "Hover"
                    PropertyChanges {
                        target: itemRect
                        color: listView.currentIndex == index ? "#63ce63" : "#eeeeee"
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
                    rightside.pop()
                    rightside.pushNoAnimation(["qrc:/qml/ChatPage.qml"])
                    listView.currentIndex = index
                }
            }
        }
    }

    //contacts view
    ListView {
        id: listView
        width: parent.width
        height: parent.height-40

        anchors.top: searchline.bottom
        model: ContactWidgetModel {}
        delegate: contactDelegate
        currentIndex: -1
    }
}
