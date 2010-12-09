import Qt 4.7

Item {
    id: toogleButton

    property string buttonImageUrl
    property string pressedButtonImageUrl
    property bool toggled: false

    signal clicked

    Image {
        id: buttonImage
        source: buttonImageUrl
        anchors.fill: parent
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent
        onClicked: {
            toogleButton.clicked();
            if (!toggled)
                toggled = true;
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
