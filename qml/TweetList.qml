import Qt 4.7

Item {
    id: tweetListViewItem

	ListView {
        id: tweetListView
        model: tweetListModel
        width: parent.width
        height: parent.height
		delegate: Tweet {
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
            tweetAvatar: avatarUrlRole
		}
	}
}

