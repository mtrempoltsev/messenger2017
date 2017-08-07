import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Rectangle {
    id: logo
    Layout.alignment: Qt.AlignTop
    Layout.fillWidth: true
//    height: 100
    implicitHeight: 100
    color: 'lightblue'
    implicitWidth: logoText.implicitWidth
    Text {
        id: logoText
        anchors.fill: logo
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Messenger 2017")
        font.bold: true
        font.pointSize: 40
    }
}
