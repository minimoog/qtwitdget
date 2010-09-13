import Qt 4.7

Item {
    id: tweetListViewItem

    property alias model: tweetListView.model

    signal replyClicked(string id, string screenname)
    signal retweetClicked(string text, string screenname)
    signal deleteClicked(string id)

	ListView {
        id: tweetListView
        //model: tweetListModel
        width: parent.width; height: parent.height
        clip: true
		delegate: Tweet {
            id: tweetDelegate
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
            tweetAvatar: avatarUrlRole
            tweetid:  statusIdRole
            isOwnTweet: ownTweetRole

            onReplyButtonClicked: replyClicked(tweetDelegate.tweetid, tweetDelegate.tweetScreenName)
            onRetweetButtonClicked: retweetClicked(tweetDelegate.tweetText, tweetDelegate.tweetScreenName)
            onDeleteButtonClicked: deleteClicked(tweetDelegate.tweetid)
		}
	}

    // taken from Flickable documentation
    Rectangle {
        id: scrollbar
        anchors.right: tweetListView.right
        y: tweetListView.visibleArea.yPosition * tweetListView.height
        width: 2
        height: tweetListView.visibleArea.heightRatio * tweetListView.height
        color: "black"
    }
}

