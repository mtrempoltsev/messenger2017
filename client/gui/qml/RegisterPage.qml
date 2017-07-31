import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3


Page {
    id: root

    RowLayout {
        id:mainLayout
        anchors.fill: parent
        Rectangle {
            id: logo
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            height: 100
            color: 'lightblue'
            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Messenger 2017")
                font.bold: true
                font.pointSize: 40

            }
        }

    }
}
