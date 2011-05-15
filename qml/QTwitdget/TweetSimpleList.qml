import QtQuick 1.0

Rectangle {
    id: container

    property alias model: tweetListView.model

    signal conversationButtonClicked(string id)
    signal hashtagClicked(string hashtag)

    color: "#ffffff"

    ListView {
        id: tweetListView
        anchors.fill: parent
        clip: true
        delegate: TweetSimpleDelegate {
            tweetid: statusIdRole
            tweetText: statusTextRole
            tweetSinceTime: sinceTimeRole

            onConversationButtonClicked: container.conversationButtonClicked(tweetid)
            //onHashtagLinkClicked: hashtagClicked(hashtag)
        }
    }

    Rectangle {
        id: scrollbar
        anchors.right: tweetListView.right
        y: tweetListView.visibleArea.yPosition * tweetListView.height
        width: 2
        height: tweetListView.visibleArea.heightRatio * tweetListView.height
        color: "black"
    }
}
