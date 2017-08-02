import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import QtQuick 2.6
import QtQuick.Controls 2.1

ApplicationWindow {
    id: window
    visible: true

    minimumHeight: stackView.currentItem.implicitHeight
    minimumWidth: stackView.currentItem.implicitWidth


    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: RegisterPage {}
    }
}
