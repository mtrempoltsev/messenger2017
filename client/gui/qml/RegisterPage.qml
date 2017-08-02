import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3


Page {
    id: root
    implicitWidth: 450
    implicitHeight: 350
    ColumnLayout {
        id:mainLayout
        anchors.fill: parent
        Layout.minimumWidth: 450
        Layout.minimumHeight: 350

        Rectangle {
            id: logo
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            height: 100
            color: 'lightblue'
            Text {
                anchors.fill: logo
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Messenger 2017")
                font.bold: true
                font.pointSize: 40
            }
        }

        ColumnLayout {
            id: registerLayout
            anchors.margins: 5
            Layout.alignment: Qt.AlignHCenter
            Layout.minimumWidth: 300
            spacing: 4

            Text {
                text: qsTr("Wellcome to messenger!")
                font.bold: true
                Layout.preferredHeight: 40
            }

            Text {
                text: qsTr("Please choose your web server:")
                font.italic: true
                Layout.preferredHeight: 40
            }

            ComboBox {
                id: serverCombobox
                Layout.preferredHeight: 40
                anchors.left: parent.left
                anchors.right: parent.right
                editable: true
                model: ListModel {
                    id: model
                    ListElement { text: "test-server.io"}
                    ListElement { text: "dev-server.io" }
                    ListElement { text: "mail.ru"}
                }
                onAccepted: {
                    if (find(currentText) === -1) {
                        model.append({text: editText})
                        currentIndex = find(editText)
                    }
                }
            }

            Button {
                id: registerButton
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Register")

                onClicked: console.log(serverCombobox.currentText)
            }
        }
    }
}
