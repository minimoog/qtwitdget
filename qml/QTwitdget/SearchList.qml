import QtQuick 1.0

Item {

    property alias model: searchListView.model

    //signal searchClicked

    //add search box
    //width: 360
    //height: 462

    signal moreClicked(string text, string screenname, string sincetime)
    signal replyClicked(string id, string screenname)
    signal retweetClicked(string text, string screenname)
    signal hashtagClicked(string hashtag)

    function doSearch(query) {
        queryInput.text = query
        model.startSearch(query)
    }

    TextInput {
        id: queryInput

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20
        focus: true

        onAccepted: model.startSearch(queryInput.text)
    }

    ListView {
        id: searchListView
        //anchors.fill: parent
        anchors.top: queryInput.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        delegate: TweetDelegate {
            tweetSinceTime: sinceTimeRole
            tweetScreenName: screenNameRole
            tweetAvatar: avatarUrlRole
            tweetText: statusTextRole

            onMoreButtonClicked: moreClicked(tweetText, tweetScreenName, tweetSinceTime)
            onMentionLinkClicked: moreClicked('' /* temp */, screenname, '' /* temp */)
            onHashtagLinkClicked: doSearch(hashtag)
            onReplyButtonClicked: replyClicked(tweetid, tweetScreenName)
            onRetweetButtonClicked: retweetClicked(tweetText, tweetScreenName)
        }
    }

    Rectangle {
        id: scrollbar
        anchors.right: searchListView.right
        y: searchListView.visibleArea.yPosition * searchListView.height
        width: 2
        height: searchListView.visibleArea.heightRatio * searchListView.height
        color: "black"
    }
}
