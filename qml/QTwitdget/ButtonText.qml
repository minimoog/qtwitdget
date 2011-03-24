import QtQuick 1.0

Item {
    id: container

    property string text
    property string buttonImageUrl
    property string pressedButtonImageUrl

    signal clicked

    Image {
        id: btnImage
        anchors.fill: parent
        source: buttonImageUrl

        MouseArea {
            id: mouseRegion
            anchors.fill: parent
            onClicked: container.clicked()
        }

        Text {
            id: btnText
            anchors.centerIn: parent
            text: container.text
            color: "#F0F0F0"
            font.pixelSize: 12 + (osMac?2:0)
        }
    }

    states: [
        State {
            name: "Pressed"
            when: mouseRegion.pressed
            PropertyChanges { target: btnImage; source: pressedButtonImageUrl }
        }
    ]
}
