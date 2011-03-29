import QtQuick 1.0

Item {
    property alias model: tweetListView.model

    signal clickedDelegate(string delegateID, string delegateText, string delegateSinceTime)
    signal hashtagClicked(string hashtag)

    ListView {
        id: tweetListView
        anchors.fill: parent
        clip: true
        currentIndex: -1
        delegate: TweetSimpleDelegate {
            tweetid: statusIdRole
            tweetText: statusTextRole
            tweetSinceTime: sinceTimeRole

            onClicked: clickedDelegate(tweetid, tweetText, tweetSinceTime)
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
