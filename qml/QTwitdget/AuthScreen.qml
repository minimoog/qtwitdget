import QtQuick 1.0

Item {
    id: wrapper

    signal loginClicked(string username, string password)

    function clear() {
        usernameInput.text = ''
        passwordInput.text = ''
    }

    Rectangle {
        id: background
        gradient: Gradient {
            GradientStop { position: 0; color: "#ffffff" }
            GradientStop { position: 1; color: "#d9d9d9" }
        }
        anchors.fill: parent

        Image {
            id: logo
            anchors.top: parent.top; anchors.topMargin: 84
            anchors.horizontalCenter: parent.horizontalCenter
            source: "images/logo.png"
        }

        Image {
            id: username_background
            anchors.top: logo.bottom; anchors.topMargin: 53
            anchors.horizontalCenter: parent.horizontalCenter
            source: "images/text_input_background.png"
            // ### TODO: BorderImage

            TextInput {
                id: usernameInput
                color: "#a3a3a3"
                font.pixelSize: 13
                font.family: "Segoe UI"
                anchors.rightMargin: 13
                anchors.bottomMargin: 6
                anchors.topMargin: 6
                anchors.leftMargin: 13
                anchors.fill: parent
                focus: true
                KeyNavigation.tab: passwordInput
            }
        }

        Image {
            id: password_background
            anchors.top: username_background.bottom; anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            source: "images/text_input_background.png"

            TextInput {
                id: passwordInput
                color: "#a3a3a3"
                echoMode: TextInput.Password
                font.pixelSize: 13
                font.family: "Segoe UI"
                anchors.rightMargin: 13
                anchors.bottomMargin: 6
                anchors.topMargin: 6
                anchors.leftMargin: 13
                anchors.fill: parent
                KeyNavigation.backtab: usernameInput
            }
        }

        ButtonImage {
            id: signinButton
            width: 121
            height: 39
            anchors.top: password_background.bottom; anchors.topMargin: 52
            anchors.horizontalCenter: parent.horizontalCenter

            buttonImageUrl: "images/signin.png"
            pressedButtonImageUrl: "images/signin_pressed.png"

            onClicked: loginClicked(usernameInput.text, passwordInput.text)
        }

        Text {
            id: authorsText
            color: "#7c7c7c"
            text: 'Programmed by: Toni Jovanoski <br> UI design by: Daniel Janev'
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            font.family: "Segoe UI"
            font.pixelSize: 12
        }
    }
    Keys.onReturnPressed: loginClicked(usernameInput.text, passwordInput.text)
    Keys.onEnterPressed: loginClicked(usernameInput.text, passwordInput.text)
}
