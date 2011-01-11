import Qt 4.7

Item {
    id: toogleButton

    property string buttonImageUrl
    property string pressedButtonImageUrl
    property bool toggled: false
    property bool showNotification: false

    signal clicked

    Image {
        id: buttonImage
        source: buttonImageUrl
        smooth: true
        anchors.fill: parent

        Image {
            id: notificationMarkerImage
            opacity: 0
            anchors.top: parent.top
            anchors.right: parent.right
            source: "../images/red_marker.png"

            states: [
                State {
                    name: "showMarker"
                    when: showNotification
                    PropertyChanges { target: notificationMarkerImage; opacity: 1 }
                }
            ]

            transitions: [
                Transition {
                    NumberAnimation { property: "opacity"; duration: 500 }
                }
            ]
        }
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent
        onClicked: {
            toogleButton.clicked();
            toggled = !toggled;
        }
    }

    states: [
        State {
            name: "toggle_on"
            when: !toggled
            PropertyChanges { target: buttonImage; source: pressedButtonImageUrl }
        }
    ]
}
