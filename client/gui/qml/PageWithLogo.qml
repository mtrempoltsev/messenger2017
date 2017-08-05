import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3


Page {
    function getMax(item1, item2) {
        return item1 < item2 ? item2 : item1
    }

    property var controler
    default property alias contents: placeholder.children

    implicitWidth: getMax(logo.implicitWidth, mainLayout.implicitWidth) * 1.1
    implicitHeight: (logo.implicitHeight + mainLayout.implicitHeight) * 1.1

    header: RectangleLogo {
        id: logo
    }

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent

        ColumnLayout {
            id: placeholder
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            anchors.margins: 5
            spacing: 4
        }
    }

}
