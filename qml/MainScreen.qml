import Qt 4.7

//rootWindow - external
//tweetListModel - external
//mentionsListModel - external
//directMessagesListModel - external

Item {
    id: screen
    objectName: "screen"
    property bool authed: true

    Item {
        id: container
        height: parent.height; width: parent.width

        Item {
            id: toolbar

            width: parent.width; height: 37

            Row {
                ToggleButton {
                    id: showHomeTimelineButton
                    buttonImageUrl: "../images/hometimeline.png"
                    pressedButtonImageUrl: "../images/hometimeline_pressed.png"
                    showNotification: tweetListModel.numNewTweets
                    width: 35; height: toolbar.height - 2
                    toggled: true
                    onClicked: {
                        containerLists.x = 0;
                        tweetListModel.showNewTweets();
                        //other buttons untoggled
                        showMentionsButton.toggled = false;
                        showDirectMessagesButton.toggled = false;
                    }
                }

                ToggleButton {
                    id: showMentionsButton
                    buttonImageUrl: "../images/replies.png"
                    pressedButtonImageUrl: "../images/replies_pressed.png"
                    showNotification: mentionsListModel.numNewTweets
                    width: 35; height: toolbar.height - 2
                    onClicked: {
                        containerLists.x = - container.width;
                        mentionsListModel.showNewTweets();
                        showHomeTimelineButton.toggled = false;
                        showDirectMessagesButton.toggled = false;
                    }
                }

                ToggleButton {
                    id: showDirectMessagesButton
                    buttonImageUrl: "../images/messages.png"
                    pressedButtonImageUrl: "../images/messages_pressed.png"
                    showNotification: directMessagesListModel.numNewDirectMessages
                    width: 35; height: toolbar.height - 2
                    onClicked: {
                        containerLists.x = - 2 * container.width;
                        directMessagesListModel.showNewTweets();
                        showHomeTimelineButton.toggled = false;
                        showMentionsButton.toggled = false;
                    }
                }
            }
        }

        Item {
            id: containerLists
            x: 0; y: toolbar.height

            Row {
                TweetList {
                    id: homeTimelineList
                    width: container.width; height: container.height - toolbar.height
                    model:  tweetListModel

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

                TweetList {
                    id: mentionList
                    width: container.width; height: container.height - toolbar.height
                    model: mentionsListModel

                    onReplyClicked: {
                        tweetUpdateElement.setReply(id, screenname);
                        tweetUpdateElement.moved = true;
                    }

                    onRetweetClicked: {
                        tweetUpdateElement.setRetweet(text, screenname);
                        tweetUpdateElement.moved = true;
                    }

                    onDeleteClicked: {
                        mentionsListModel.destroyTweet(id);
                    }
                }

                DirectMessageList {
                    id: directMessageList
                    width: container.width; height: container.height - toolbar.height
                    model: directMessagesListModel

                    // TODO: Replying direct message
                    onReplyClicked: {
                        tweetUpdateElement.screenName = screenname;
                        tweetUpdateElement.moved = true;
                    }
                }
            }

            Behavior on x {
                NumberAnimation { easing.type: "InOutBack"; duration: 500 }
            }
        }

        TweetUpdate {
            property bool moved: false

            id: tweetUpdateElement
            x: 0; y: container.height //containerTweetList.height - height
            width: parent.width

            onUpdateButtonClicked: {
                rootWindow.updateButtonClicked(tweetUpdateElement.tweetid,
                                               tweetUpdateElement.updateText,
                                               tweetUpdateElement.screenName);
                tweetUpdateElement.updateText = "";
                tweetUpdateElement.tweetid = "";
                tweetUpdateElement.screenName = "";
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
                    PropertyChanges { target: tweetUpdateElement; y: container.height - tweetUpdateElement.height}
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
            PropertyChanges { target: container; x: parent.width }
            PropertyChanges { target: authPage; x: 0 }
        }
    ]

    transitions: [
        Transition { NumberAnimation { properties: "x"; duration: 500; easing.type: Easing.InOutQuad } }
    ]
}

