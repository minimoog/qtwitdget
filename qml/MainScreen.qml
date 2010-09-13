import Qt 4.7

//rootWindow - external
//tweetListModel - external

Item {
    id: screen
    objectName: "screen"
    property bool authed: true

    Item {
        id: containerTweetList
        height: parent.height; width: parent.width

        TweetList {
            id: tweetsPage
            x: 0; y: 0
            height: parent.height; width: parent.width
            model: tweetListModel

            onReplyClicked: {
                tweetUpdateElement.setReply(id, screenname);
                tweetUpdateElement.moved = true;
            }

            onRetweetClicked: {
                tweetUpdateElement.setRetweet(text, screenname);
                tweetUpdateElement.moved = true;
            }

            onDeleteClicked: {
                tweetListModel.destroyTweet(id);
            }
        }

        TweetUpdate {
            property bool moved: false

            id: tweetUpdateElement
            x: 0; y: containerTweetList.height //containerTweetList.height - height
            width: parent.width

            onUpdateButtonClicked: {
                rootWindow.updateButtonClicked(tweetUpdateElement.tweetid, tweetUpdateElement.updateText);
                tweetUpdateElement.updateText = "";
                tweetUpdateElement.tweetid = "";
                moved = false;
            }

            // ### TODO: Toggle button
            TestButton {
                id: showUpdateElement
                height: 10; width: 50
                x: 0; y: -height
                text: "U"

                onClicked: parent.moved = !parent.moved;
            }

            states: [
                State {
                    name: "moved"
                    when: tweetUpdateElement.moved
                    PropertyChanges { target: tweetUpdateElement; y: containerTweetList.height - tweetUpdateElement.height}
                }
            ]

            transitions: Transition {
                PropertyAnimation { easing.type: "InOutQuad"; properties: "y"; duration: 400 }
            }
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

