import Qt 4.7

//rootWindow - external
//tweetListModel - external
//mentionsListModel - external

Item {
    id: screen
    objectName: "screen"
    property bool authed: true

    Item {
        id: container
        height: parent.height; width: parent.width

        Item {
            id: toolbar //or menubar

            width: parent.width; height: 30

            Row {
                TestButton {
                    id: showHomeTimelineButton
                    width: 40; height: toolbar.height - 2
                    text: "H" + ' ' + tweetListModel.numNewTweets
                    onClicked: {
                        containerLists.x = 0;
                        tweetListModel.showNewTweets();
                    }
                }

                TestButton {
                    id: showMentionsButton
                    width:  40; height: toolbar.height - 2
                    text: "M" + ' ' + mentionsListModel.numNewTweets
                    onClicked: {
                        containerLists.x = - container.width;
                        mentionsListModel.showNewTweets();
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
            }

            Behavior on x {
                // ### BUG: Mentions selected, user clicks authorize, happen overlap
                NumberAnimation { easing.type: "InOutBack"; duration: 500 }
            }
        }

        TweetUpdate {
            property bool moved: false

            id: tweetUpdateElement
            x: 0; y: container.height //containerTweetList.height - height
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

