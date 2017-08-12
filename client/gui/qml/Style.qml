pragma Singleton
import QtQuick 2.0

QtObject {
    property bool whitestyle: true

    /*  general */
    property color mainBackground: whitestyle ? "white" : "#292b35"
    property color splitColor: "lightGray"
    property color textColor: whitestyle ? "black" : "white"
    property color buttonColor: whitestyle ? "black" : "lightgray"

    /*  ContactPage */
    property color hover: whitestyle ? "#a0dea0" : "#408698"
    property color hoverActive: whitestyle ? "limegreen" : "#285560"
    property color active: whitestyle ? "#63ce63" : "#336c7a"
    property color contactTextColor: whitestyle ? "black" : "white"
    property color contactSubText: "gray"
    property color contactSubTextHover: "white"

    /*  chatPage  */
    property color chatTime: "lightGray"

    property color myNameColor: "#9d81ba"
    property color myMessageBackground: whitestyle ? "#a0dea0" : "#408698"
    property color myMessageColor: whitestyle ? "black" : "white"

    property color otherNameColor: "red"
    property color otherMessageBackground: whitestyle ? "whitesmoke" : "lightgray"
    property color otherMessageColor: "black"
}
