import QtQuick 1.1
import com.nokia.meego 1.0

Page {
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

        TextField {
            id: usernameInput
            placeholderText: "username"

            anchors.top: logo.bottom
            anchors.topMargin: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextField {
            id: passwordInput
            placeholderText: "password"

            anchors.top: usernameInput.bottom
            anchors.topMargin: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: signinButton

            text: 'Signin to Twitter'
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: passwordInput.bottom
            anchors.topMargin: 16

            onClicked: rootWindow.authorize(usernameInput.text, passwordInput.text)
        }

        Text {
            id: authorsText
            color: "#7c7c7c"
            text: 'Programmed by: Toni Jovanoski <br> UI design by: Daniel Janev'
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
        }
    }
}
