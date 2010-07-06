import Qt 4.7

Rectangle {
    id: container
    width: 30
    height: 11

    signal clicked

    BorderImage {
        id: buttonImage
        width: container.width; height: container.height
        source: "../images/button_reply.png"
    }

    BorderImage {
        id: hoverImage
        opacity: 0
        source: "../images/button_reply_hover.png"
        width: container.width; height: container.height

        Behavior on opacity {
            NumberAnimation {
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }
    }

    BorderImage {
        id: clickImage
        opacity: 0
        source: "../images/button_reply_click.png"
        width: container.width; height: container.height

        Behavior on opacity {
            NumberAnimation {
                duration: 300
                easing.type: Easing.InOutQuad
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
        },
        State {
            name: "Hovered"
            when: mouseRegion.containsMouse == true
            PropertyChanges {
                target: hoverImage
                opacity: 1
            }
        }
    ]
}
