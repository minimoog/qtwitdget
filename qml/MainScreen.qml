import Qt 4.7

//rootWindow - external

Item {
    id: screen
    objectName: "screen"
    property bool authed: true

    TweetList {
        id: tweetsPage
        x: 0
        y: 0
        height: parent.height
        width: parent.width
    }

    AuthPage {
        id: authPage
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height
        x: -screen.width
        onLoginClicked: {
            rootWindow.authorize(username, password);
        }

        Keys.onEscapePressed: authed = true
    }

    states: [
        State {
            name: "unauthed"
            when: !screen.authed
            PropertyChanges { target: tweetsPage; x: parent.width }
            PropertyChanges { target: authPage; x: 0 }
        }
    ]

    transitions: [
        Transition { NumberAnimation { properties: "x"; duration: 500; easing.type: Easing.InOutQuad } }
    ]
}

