import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "."


// list delegate
Component {
    id: settingsDelegate
    Item {
        id: settingsItem
        width: parent.width
        height: 35

        //seting start state
        state: "Normal"

        // item view
        Column {
            Rectangle {
                id: itemRect

                width: settingsItem.width
                height: settingsItem.height - 1
                color: Style.mainBackground

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: arrowimg.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: name
                    color: Style.textColor
                    font.pixelSize: Math.round(parent.height / 2)
                    elide: Text.ElideRight
                }

                Image {
                    id: arrowimg
                    source: "../img/arrow.png"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    height: parent.height / 2
                    width: height
                }

                ColorOverlay {
                    anchors.fill: arrowimg
                    source: arrowimg
                    color: Style.buttonColor
                }
            }
            Rectangle {
                color: Style.splitColor
                height: 1
                width: settingsItem.width
            }
        }

        MouseArea {
            id: listViewMouseArea
            anchors.fill: parent
            onClicked: {

                // Switch to details view
                //                listView.currentIndex = index
                //                detailViewImage.source = photo
                //                detailViewName.text = name
                //                detailViewNumber.text = number
                //                detailListView.switchState()
                //                Style.whitestyle = !Style.whitestyle
                rightside.push("qrc:/qml/SettingsPage.qml")
            }
        }
    }
}
