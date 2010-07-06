import Qt 4.7

// ### TODO: animation is wrong

Item {
    id: container

    property string buttonImageUrl
    property string hoverButtonImageUrl
    property string clickedButtonImageUrl

    signal clicked

    Image {
        id: buttonImage
        source: buttonImageUrl

        Behavior on opacity {
            NumberAnimation {
                duration: 300
                easing.type: Easing.Linear
            }
        }
    }

    Image {
        id: hoverImage
        opacity: 0
        source: hoverButtonImageUrl

        Behavior on opacity {
            NumberAnimation {
                duration: 300
                easing.type: Easing.Linear
            }
        }
    }

    Image {
        id: clickImage
        opacity: 0
        source: clickedButtonImageUrl

        Behavior on opacity {
            NumberAnimation {
                duration: 300
                easing.type: Easing.Linear
            }
        }
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: buttonImage
        hoverEnabled: true
        onClicked: { container.clicked(); }
    }

    states: [
        State {
            name: "Pressed"
            when: mouseRegion.pressed == true
            PropertyChanges {
                target: clickImage
                opacity: 1
            }
            PropertyChanges {
                target: buttonImage
                opacity: 0
            }

        },
        State {
            name: "Hovered"
            when: mouseRegion.containsMouse == true
            PropertyChanges {
                target: hoverImage
                opacity: 1
            }
            PropertyChanges {
                target: buttonImage
                opacity: 0
            }
        }
    ]
}
