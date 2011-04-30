import Qt 4.7

Rectangle {
    id: container

    property alias model: tweetListView.model

    signal replyClicked(string id, string screenname, string tweettext)
    signal retweetClicked(string text, string screenname)
    //signal deleteClicked(string id)

    //since time is not updated

    //right arrow pressed or mention clicked
    signal moreClicked(string statusid, string text, string screenname, string sincetime)
    signal hashtagClicked(string hashtag)

    color: "#ffffff"

    ListView {
        id: tweetListView
        anchors.fill: parent;
        clip: true
        cacheBuffer: 120 * 20
        delegate: TweetDelegate {
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
            tweetAvatar: avatarUrlRole
            tweetid:  statusIdRole
            isOwnTweet: ownTweetRole
            isNewTweet: newTweetRole
            tweetSinceTime: sinceTimeRole

            //onMoreButtonClicked: moreClicked(tweetid, tweetText, tweetScreenName, tweetSinceTime)
            onMentionLinkClicked: moreClicked('', '' /* temp */, screenname, '')
            onHashtagLinkClicked: hashtagClicked(hashtag)
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
