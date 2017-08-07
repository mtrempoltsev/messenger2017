import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3


Page {
    id: root
    implicitHeight: 500
    implicitWidth: 500

    header: ToolBar {
        id: toolBar
        property int fontSize: 26
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("‹")
                font.bold: true
                font.pointSize: toolBar.fontSize
                onClicked: rightside.pop()
            }
            Label {
                text: qsTr("Settings")
                font.pointSize: toolBar.fontSize
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }
    }


    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Switch {

        }

        GroupBox {
            id: groupBox
            width: 200
            height: 200
            title: qsTr("Group Box")
            ColumnLayout {
                anchors.fill: parent
                RadioButton {
                    id: radioButton
                    text: qsTr("Radio Button")
                }

                RadioButton {
                    id: radioButton1
                    text: qsTr("Radio Button")
                }
            }
        }
    }

}
