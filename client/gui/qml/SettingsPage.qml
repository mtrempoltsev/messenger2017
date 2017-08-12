import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Page {
    //    id: root
    implicitHeight: parent.height
    implicitWidth: parent.width

    background: Rectangle {
        color: Style.mainBackground
    }


    //    header: ToolBar {
    //        id: toolBar
    //        property int fontSize: 26
    //        RowLayout {
    //            anchors.fill: parent
    //            ToolButton {
    //                text: qsTr("‹")
    //                font.bold: true
    //                font.pointSize: toolBar.fontSize
    //                onClicked: rightside.pop()
    //            }
    //            Label {
    //                text: qsTr("Settings")
    //                font.pointSize: toolBar.fontSize
    //                elide: Label.ElideRight
    //                horizontalAlignment: Qt.AlignHCenter
    //                verticalAlignment: Qt.AlignVCenter
    //                Layout.fillWidth: true
    //            }
    //        }
    //    }
    ListModel {
        id: listmodelsettings
        ListElement {
            name: "Основное"
        }
        ListElement {
            name: "Дополнительно"
        }
    }

    ListView {
        anchors.leftMargin: 20
        anchors.fill: parent
        id: settingslist
        model: listmodelsettings
        delegate: SettingsListDelegateItem {
        }
    }

    RowLayout {
        anchors.bottom: parent.bottom
        Layout.fillWidth: true
        Text {
            Layout.leftMargin: 20
            text: qsTr("Белый стиль")
            color: Style.textColor
            anchors.verticalCenter: parent.verticalCenter
        }
        Switch {
            id: control
            checked: true

            onClicked: {
                Style.whitestyle = !Style.whitestyle
            }
        }
    }
}
