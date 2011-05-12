import QtQuick 1.0

Item {
    id: btnImage

    property string buttonImageUrl
    property string pressedButtonImageUrl
    property alias sourceSizeWidth : buttonImage.sourceSize.width
    property alias sourceSizeHeight : buttonImage.sourceSize.height

    signal clicked

    Image {
        id: buttonImage
        source: buttonImageUrl
        anchors.fill: parent
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent

        onClicked: btnImage.clicked();
    }

    states: [
        State {
            name: "pressed"
            when: mouseRegion.pressed
            PropertyChanges { target: buttonImage; source: pressedButtonImageUrl }
        }
    ]
}
