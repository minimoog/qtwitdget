import Qt 4.7

//tweetListModel - external
//statusEdit - external

Item {
    id: tweetListViewItem

	ListView {
        id: tweetListView
        model: tweetListModel
        width: parent.width
        height: parent.height
        clip: true
		delegate: Tweet {
            id: tweetDelegate
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
            tweetAvatar: avatarUrlRole
            tweetid:  statusIdRole
            isOwnTweet: ownTweetRole

            Connections {
                target: tweetDelegate
                onReplyButtonClicked: {
                    statusEdit.setReply(tweetDelegate.tweetid, tweetDelegate.tweetScreenName);
                    statusEdit.setFocus();
                }
                onRetweetButtonClicked: {
                    statusEdit.setRetweet(tweetDelegate.tweetText, tweetDelegate.tweetScreenName);
                    statusEdit.setFocus();
                }
                onDeleteButtonClicked: {
                    tweetListModel.destroyTweet(tweetDelegate.tweetid);
                }
            }
		}
	}
}

