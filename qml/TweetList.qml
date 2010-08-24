import Qt 4.7

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

            Connections {
                target: tweetDelegate
                onReplyDeleteButtonClicked: {
                    statusEdit.setReply(tweetDelegate.tweetid, tweetDelegate.tweetScreenName);
                    statusEdit.setFocus();
                }
                onRetweetButtonClicked: {
                    statusEdit.setRetweet(tweetDelegate.tweetText, tweetDelegate.tweetScreenName);
                    statusEdit.setFocus();
                }
            }
		}
	}
}

