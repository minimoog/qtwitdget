import Qt 4.7

//tweetListModel - external

Item {
    id: tweetListViewItem

    signal replyClicked(string id, string screenname)
    signal retweetClicked(string text, string screenname)

	ListView {
        id: tweetListView
        model: tweetListModel
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

            //move/refactor to MainScreen.qml
            onDeleteButtonClicked: tweetListModel.destroyTweet(tweetDelegate.tweetid)
		}
	}
}

