import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Page {
    id: loginPage
    function getMax(item1, item2) {
        return item1 < item2 ? item2 : item1
    }

    implicitWidth: getMax(logo.implicitWidth, mainLayout.implicitWidth) * 1.1 /*mainLayout.implicitWidth * 1.1*/
    implicitHeight: (logo.implicitHeight + mainLayout.implicitHeight) * 1.1

    header: RectangleLogo {
        id: logo
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent

        spacing: 4

//        RectangleLogo {
//            id: logo
//        }


        ColumnLayout {
            id: registerLayout
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            anchors.margins: 5
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

                }
            }
        }
    }

}
