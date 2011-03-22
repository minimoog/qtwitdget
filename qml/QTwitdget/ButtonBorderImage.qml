import QtQuick 1.0

Item {
    id: buttonImage

    property string buttonImageUrl
    property string pressedButtonImageUrl
    property int borderBottom
    property int borderTop
    property int borderLeft
    property int borderRight

    signal clicked

    BorderImage {
        id: brdImage
        source: buttonImageUrl
        anchors.fill: parent
        border.bottom: borderBottom
        border.top: borderTop
        border.left: borderLeft
        border.right: borderRight
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent

        onClicked: buttonImage.clicked()
    }

    states: [
        State {
            name: 'pressed'
            when: mouseRegion.pressed
            PropertyChanges { target: brdImage; source: pressedButtonImageUrl }
        }
    ]
}
