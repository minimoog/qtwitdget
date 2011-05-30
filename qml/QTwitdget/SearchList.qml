import QtQuick 1.0

Item {
    id: container

    property alias model: searchListView.model

    signal tweetClicked(string tweetid, string tweettext, string screenname)

    //signal searchClicked

    //signal reply(string id, string screenname, string tweettext)
    //signal retweet(string text, string screenname)
    //signal conversation(string id)
    //signal userinformation(string screenname)
    //signal hashtagClicked(string hashtag)

    function doSearch(query) {
        queryInput.text = query
        model.startSearch(query)
    }

    Rectangle {
        id: background
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#ffffff"
            }
            GradientStop {
                position: 1
                color: "#D1D1D1"
            }
        }
        anchors.fill: parent
    }

    BorderImage {
        id: queryInputBackground
        height: 33
        anchors.right: parent.right
        anchors.rightMargin: 54
        anchors.left: parent.left
        anchors.leftMargin: 53
        anchors.top: parent.top
        anchors.topMargin: 21
        border.bottom: 5
        border.top: 5
        border.right: 5
        border.left: 5
        source: "images/status_update_background.png"

        TextInput {
            id: queryInput
            color: "#b0b0b0"
            text: ""
            font.pointSize: 6
            cursorVisible: true
            font.family: "Segoe UI"
            anchors.bottomMargin: 6
            anchors.leftMargin: 6
            anchors.rightMargin: 6
            anchors.topMargin: 6
            anchors.fill: parent
            focus: true

            onAccepted: model.startSearch(queryInput.text)
        }
    }

    ListView {
        id: searchListView
        anchors.topMargin: 16
        //anchors.fill: parent
        anchors.top: queryInputBackground.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        delegate: TweetDelegate {
            tweetSinceTime: sinceTimeRole
            tweetScreenName: screenNameRole
            tweetAvatar: avatarUrlRole
            tweetText: statusTextRole
            tweetid: statusIdRole

            onTweetClicked: container.tweetClicked(tweetid, tweetText, tweetScreenName)

            //onReplyClicked: reply(tweetid, tweetScreenName, tweetText)
            //onRetweetClicked: retweet(tweetText, tweetScreenName)
            //onConversationClicked: conversation(tweetid)
            //onMentionLinkClicked: userinformation(screenname)
            //onHashtagLinkClicked: hashtagClicked(hashtag)
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
