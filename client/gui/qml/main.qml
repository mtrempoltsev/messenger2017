import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import QtQuick 2.6
import QtQuick.Controls 2.1

ApplicationWindow {
    id: window
    width: 540
    height: 600
    visible: true

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: RegisterPage {}
    }
}
