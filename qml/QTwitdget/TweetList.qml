import Qt 4.7

Item {
    //id: tweetListView

    property alias model: tweetListView.model

    signal replyClicked(string id, string screenname)
    signal retweetClicked(string text, string screenname)
    //signal deleteClicked(string id)

    //since time is not updated

    //right arrow pressed or mention clicked
    signal moreClicked(string statusid, string text, string screenname, string sincetime)
    signal hashtagClicked(string hashtag)

    ListView {
        id: tweetListView
        anchors.fill: parent;
        clip: true
        delegate: TweetDelegate {
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
            tweetAvatar: avatarUrlRole
            tweetid:  statusIdRole
            isOwnTweet: ownTweetRole
            isNewTweet: newTweetRole
            tweetSinceTime: sinceTimeRole

            //onReplyButtonClicked: replyClicked(tweetDelegate.tweetid, tweetDelegate.tweetScreenName)
            //onRetweetButtonClicked: retweetClicked(tweetDelegate.tweetText, tweetDelegate.tweetScreenName)
            //onDeleteButtonClicked: deleteClicked(tweetDelegate.tweetid)

            onMoreButtonClicked: moreClicked(tweetid, tweetText, tweetScreenName, tweetSinceTime)
            onMentionLinkClicked: moreClicked('', '' /* temp */, screenname, '')
            onHashtagLinkClicked: hashtagClicked(hashtag)
            onReplyButtonClicked: replyClicked(tweetid, tweetScreenName)
            onRetweetButtonClicked: retweetClicked(tweetText, tweetScreenName)
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
