import QtQuick 1.0

Rectangle {
    id: container

    property alias model: tweetListView.model

    signal tweetClicked(string tweetid, string tweettext, string screenname)

    //signal reply(string id, string screenname, string tweettext)
    //signal retweet(string tweetid)
    //signal conversation(string id)
    //signal userinformation(string screenname)
    //signal deleteClicked(string id)

    //since time is not updated

    //right arrow pressed or mention clicked
   //signal hashtagClicked(string hashtag)

     color: "#ffffff"

    ListView {
        id: tweetListView
        anchors.fill: parent
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

            onTweetClicked: container.tweetClicked(tweetid, tweetText, tweetScreenName)

            //onReplyClicked: reply(tweetid, tweetScreenName, tweetText)
            //onRetweetClicked: retweet(tweetid)
            //onConversationClicked: conversation(tweetid)
            //onMentionLinkClicked: userinformation(screenname)
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
