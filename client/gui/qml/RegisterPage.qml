import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Controler.Registration 1.0


PageWithLogo {
    id: root

    controler: RegistraionControler {
        id: registerControler
        onStartRegister: root.state="loading"
        onFinishRegisterFailed: {
            root.state="error"
        }
        onFinishRegisterSuccessed: stackView.push("qrc:/qml/MainPage.qml")
    }

    header: RectangleLogo {
        id: logo
    }

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
        running: false
        Layout.alignment: Qt.AlignHCenter
        visible: false
    }

    Text {
        id: helperText
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
            registerControler.registerMe(serverCombobox.currentText)
        }
    }


    states: [
        State {
            name: "loading"

            PropertyChanges {
                target: root
            }

            PropertyChanges {
                target: registerButton
                opacity: 0.5
                visible: true
                enabled: false
            }

            PropertyChanges {
                target: serverCombobox
                opacity: 0.5
                visible: true
                enabled: false
            }

            PropertyChanges {
                target: busyIndicator
                running: true
                visible: true
            }

            PropertyChanges {
                target: infoText
                text: qsTr("Loading to %1. Please wait ".arg(serverCombobox.currentText))
            }

            PropertyChanges {
                target: helperText
                opacity: 1
                visible: false
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