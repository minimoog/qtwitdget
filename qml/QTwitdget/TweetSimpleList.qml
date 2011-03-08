import QtQuick 1.0

Item {
    property alias model: tweetListView.model

    ListView {
        id: tweetListView
        anchors.fill: parent
        clip: true
        delegate: TweetSimpleDelegate {
            tweetid: statusIdRole
            tweetText: statusTextRole
            tweetSinceTime: sinceTimeRole
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
