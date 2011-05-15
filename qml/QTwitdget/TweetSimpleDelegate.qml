import QtQuick 1.0

Rectangle {
    id: background

    property string tweetid;
    property string tweetText
    property string tweetSinceTime

    signal conversationButtonClicked
    signal retweetButtonClicked
    signal replyButtonClicked
//    signal hashtagLinkClicked(string hashtag)
//    signal mentionLinkClicked(string screenname)

//    function handleLink(link) {
//        if (link.slice(0, 3) == 'tag') {
//            hashtagLinkClicked(link.slice(6))
//        } else if (link.slice(0, 4) == 'http') {
//            Qt.openUrlExternally(link);
//        } else if (link.slice(0, 7) == 'mention') {
//            mentionLinkClicked(link.slice(10));
//        }
//    }

    //### TODO: Make it global function
    function addTags(str) {
        //surrounds http links with html link tags
        var ret1 = str.replace(/@[a-zA-Z0-9_]+/g, '<a href="mention://$&">$&</a>');
        var ret2 = ret1.replace(/(\b(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig, "<a href='$1'>$1</a>");
        var ret3 = ret2.replace(/[#]+[A-Za-z0-9-_]+/g, '<a href="tag://$&">$&</a>')
        return ret3;
    }

    width: ListView.view.width;
    //width: 360
    height: (statusText.paintedHeight > 37) ? statusText.paintedHeight + 24 : 61

    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#ffffff"
        }
        GradientStop {
            position: 1
            color: "#d9d9d9"
        }
    }

    Text {
        id: statusText
        color: "#666666"
        text: addTags(tweetText)
        //text:  tweetText
        textFormat: Text.RichText
        anchors.right: retweetButton.left
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4
        wrapMode: Text.WordWrap
        font.pointSize: 9
        font.family: "Segoe UI"

//        onLinkActivated: {
//            background.handleLink(link)
//            console.log('link clicked')
//        }
    }

    Text {
        id: sinceText
        color: "#616161"
        text: tweetSinceTime
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4
        font.family: "Segoe UI"
        font.pointSize: 7
    }

    ButtonImage {
        id: conversationButton
        width: 15; height: 15
        anchors.top: parent.top
        anchors.topMargin: 4
        anchors.right: parent.right
        anchors.rightMargin: 4
        buttonImageUrl: 'images/small_conversation_button.png'
        pressedButtonImageUrl: 'images/small_conversation_button_pressed.png'

        onClicked: conversationButtonClicked()
    }

    ButtonImage {
        id: retweetButton
        width: 15; height: 15
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.verticalCenter: parent.verticalCenter
        buttonImageUrl: 'images/small_retweet_button.png'
        pressedButtonImageUrl: 'images/small_retweet_button_pressed.png'

        onClicked: retweetButtonClicked()
    }

    ButtonImage {
        id: replyButton
        width: 15; height: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.right: parent.right
        anchors.rightMargin: 4
        buttonImageUrl: 'images/small_reply_button.png'
        pressedButtonImageUrl: 'images/small_reply_button_pressed.png'

        onClicked: replyButtonClicked()
    }
}
