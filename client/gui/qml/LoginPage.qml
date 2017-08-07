import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

PageWithLogo {
    id: loginPage
    signal loginButtonClicked
    signal registerButtonClicked

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

    Button {

        id: loginButton
        anchors.left: parent.left
        anchors.right: parent.right
        text: qsTr("Login")
        onClicked: {
            QmlCppInterface.loginButtonClicked()
        }
    }

    footerButton: ToolButton {
        anchors.fill: parent
        text: qsTr("Register new user")
        font.pointSize: parent.fontSize
        onClicked: {
            stackView.push("qrc:/qml/RegisterPage.qml")
            QmlCppInterface.registerButtonClicked()
        }
    }

    states: [
        State {
            name: "loading"

            PropertyChanges {
                target: busyIndicator
                visible: true
                running: true
            }

            PropertyChanges {
                target: infoText
                text: qsTr("Loading... Please wait.")
            }

            PropertyChanges {
                target: loginButton
                enabled: false
            }

            PropertyChanges {
                target: registerButton
                enabled: false
            }
        },
        State {
            name: "Error"
            PropertyChanges {
                target: busyIndicator
                running: false
                visible: false
            }

            PropertyChanges {
                target: infoText
                color: "#8f001d"
                text: qsTr("Some error")
            }

            PropertyChanges {
                target: loginButton
                enabled: true
            }

            PropertyChanges {
                target: registerButton
                enabled: true
            }
        }
    ]
}
