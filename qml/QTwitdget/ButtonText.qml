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
            width: parent.width
            elide: Text.ElideRight
            anchors.centerIn: parent
            text: container.text
            horizontalAlignment: Text.AlignHCenter
            color: "#F0F0F0"
            font.pixelSize: 12
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
