import QtQuick 1.0

Rectangle {
    id: container

    property alias model: tweetListView.model

    signal conversationButtonClicked(string id)
    signal retweetButtonClicked(string text)
    signal replyButtonClicked(string id, string text)
    signal hashtagLinkClicked(string hashtag)

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
            onRetweetButtonClicked: container.retweetButtonClicked(tweetText)
            onReplyButtonClicked: container.replyButtonClicked(tweetid, tweetText)
            onHashtagLinkClicked: container.hashtagLinkClicked(hashtag)
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
