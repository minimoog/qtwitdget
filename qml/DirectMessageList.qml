import Qt 4.7

Rectangle {

    property alias model: directMessageListView.model

    signal replyClicked(string id, string screenname) //??? we don't need id, fix

    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#ffffff"
        }

        GradientStop {
            position: 1
            color: "#d5ecfc"
        }
    }

    ListView {
        id: directMessageListView
        width: parent.width; height: parent.height
        clip: true
        delegate: DirectMessageTweet {
            id: directMessageDelegate
            senderScreenName: screenNameRole
            directMessageText: statusTextRole
            avatar: avatarUrlRole
            tweetid: statusIdRole
            isOwnTweet: ownTweetRole
            isNewTweet: newTweetRole
            sinceTime: sinceTimeRole

            onReplyButtonClicked: replyClicked(directMessageDelegate.tweetid, directMessageDelegate.senderScreenName)
        }
    }

    // taken from Flickable documentation
    Rectangle {
        id: scrollbar
        anchors.right: directMessageListView.right
        y: directMessageListView.visibleArea.yPosition * directMessageListView.height
        width: 2
        height: directMessageListView.visibleArea.heightRatio * directMessageListView.height
        color: "black"
    }
}
