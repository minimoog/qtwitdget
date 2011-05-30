import QtQuick 1.0

Rectangle {
    property alias model: tweetListView.model

    signal replyButtonClicked(string statusid, string statustext)
    signal retweet(string tweettext)
    signal conversation(string id)
    signal userinformation(string screenname)
    signal hashtagClicked(string hashtag)

    color: "#ffffff"

    ListView {
        id: tweetListView
        anchors.fill: parent
        clip: true
        currentIndex: -1
        delegate: TweetSimpleDelegate {
            tweetid: statusIdRole
            tweetText: statusTextRole
            tweetSinceTime: sinceTimeRole

//            onReplyClicked: replyButtonClicked(tweetid, tweetText)
//            onRetweetClicked: retweet(tweetText)
//            onConversationClicked: conversation(tweetid)
//            onMentionLinkClicked: userinformation(screenname)
//            onHashtagLinkClicked: hashtagClicked(hashtag)
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
