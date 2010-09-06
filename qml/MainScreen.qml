import Qt 4.7

//rootWindow - external

Item {
    id: screen
    objectName: "screen"
    property bool authed: true

    Item {
        id: containerTweetList
        height: parent.height
        width: parent.width

        TweetList {
            id: tweetsPage
            x: 0
            y: 0
            height: parent.height
            width: parent.width
        }

        TweetUpdate {
            x: 0
            y: containerTweetList.height - height
            width: parent.width
        }
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
            PropertyChanges { target: containerTweetList; x: parent.width }
            PropertyChanges { target: authPage; x: 0 }
        }
    ]

    transitions: [
        Transition { NumberAnimation { properties: "x"; duration: 500; easing.type: Easing.InOutQuad } }
    ]
}

