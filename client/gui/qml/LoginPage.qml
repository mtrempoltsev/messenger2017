import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

PageWithLogo {
    id: loginPage


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

        }
    }

    Button {
        id: registerButton
        anchors.left: parent.left
        anchors.right: parent.right
        text: qsTr("Register")

        onClicked: {
            onFinishRegisterSuccessed: stackView.push("qrc:/qml/RegisterPage.qml")
        }
    }
}
