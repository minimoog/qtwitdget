import Qt 4.7

Item {
    height: tweetListView.count * 100
    width: 500

	ListView {
        id: tweetListView
        model: tweetListModel
        width: parent.width
        height: parent.height
		delegate: Tweet {
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
		}
	}
}

