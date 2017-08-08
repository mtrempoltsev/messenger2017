import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: window
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: LoginPage {}

        onCurrentItemChanged: {
            window.minimumHeight = stackView.currentItem.implicitHeight
            window.minimumWidth = stackView.currentItem.implicitWidth
            window.setHeight(window.minimumHeight)
            window.setWidth(window.minimumWidth)
        }
    }
}
