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
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            anchors.margins: 5
            Layout.minimumWidth: 300
            spacing: 4

            Text {
                id: infoText
                text: qsTr("Wellcome to messenger!")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                font.pointSize: 15
                font.bold: true
                Layout.preferredHeight: 40
            }

            BusyIndicator {
                id: busyIndicator
                Layout.alignment: Qt.AlignHCenter
                visible: false
            }

            Text {
                id: text2
                text: qsTr("Please choose your web server:")
                verticalAlignment: Text.AlignBottom
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

                onClicked: {
                    console.log(serverCombobox.currentText)
                    root.state="loading"
                    root.state="error"
                }
            }
        }
    }


    states: [
        State {
            name: "loading"

            PropertyChanges {
            }

            PropertyChanges {
                target: registerButton
                enabled: false
            }

            PropertyChanges {
                target: serverCombobox
                enabled: false
            }

            PropertyChanges {
                target: busyIndicator
                visible: true
            }

            PropertyChanges {
                target: infoText
                text: qsTr("Loading, please wait...")
            }
        },
        State {
            name: "error"

            PropertyChanges {
                target: infoText
                color: "#990020"
                text: qsTr("Can not connect to server! Please try again.")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
            }
        }
    ]
}
