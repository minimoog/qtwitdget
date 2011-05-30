import QtQuick 1.0

Item {
    id: container

    property string entityText

    signal clicked

    width: 128; height: 40

    ButtonText {
        id: buttonText
        width: parent.width
        height: parent.height
        buttonImageUrl: "images/username.png"
        pressedButtonImageUrl: "images/username_pressed.png"
        text: entityText

        onClicked: container.clicked()
    }
}
