import Qt 4.7

Item {
    id: gradRect

    x: 5; y: 0
    width: ListView.view.width;
    height: 80

    property string tweetid     //no support for 64 integers
    property string tweetScreenName: "Screen Name"
    property string tweetText: "Lorem ipsum dolor sit amet."
    property string tweetAvatar: "../images/default_avatar.png"
    property string tweetSinceTime: "sometimes ago"
    property bool isOwnTweet: false
    property bool isNewTweet: false

    signal replyButtonClicked
    signal retweetButtonClicked
    signal deleteButtonClicked

    function handleLink(link) {
        if (link.slice(0, 3) == 'tag') {
            console.log('tag clicked');
        } else if (link.slice(0, 4) == 'http') {
            Qt.openUrlExternally(link);
        } else if (link.slice(0, 7) == 'mention') {
            console.log('mention clicked');
        }
    }

    function addTags(str) {
        //surrounds http links with html link tags
        var ret1 = str.replace(/@[a-zA-Z0-9_]+/g, '<a href="mention://$&">$&</a>');
        var ret2 = ret1.replace(/(\b(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig, "<a href='$1'>$1</a>");
        var ret3 = ret2.replace(/[#]+[A-Za-z0-9-_]+/g, '<a href="tag://$&">$&</a>')
        return ret3;
    }

    Image {
        id: avatarImage

        width: 45; height: 45
        anchors.top: screenNameText.bottom
        anchors.topMargin: 4
        fillMode: Image.Stretch
        smooth: true
        source: tweetAvatar
    }

    Text {
        id: screenNameText
        color: "#49c8f5"
        text: gradRect.tweetScreenName
        smooth: true
        font.pointSize: 10
        font.bold: true
        font.family: "Arial"
    }

    BorderImage {
        id: oblace
        border.left: 10; border.top: 2; border.bottom: 2; border.right: 2
        anchors.right: parent.right; anchors.rightMargin: 10
        anchors.top: screenNameText.bottom; anchors.topMargin: 4
        anchors.left: avatarImage.right; anchors.leftMargin: 3
        source: {
            if (isOwnTweet)
                return "../images/own_oblace.png"
            else if (isNewTweet)
                return "../images/new_oblace.png"
            else
                return "../images/oblace.png"
        }

        Text {
            id: statusText
            x: 13; y: 1
            width: parent.width - 16
            color: "#333333"
            text: addTags(gradRect.tweetText)
            textFormat: Text.RichText
            wrapMode: "WordWrap"
            smooth: true
            font.pointSize: 9
            font.family: "Segoe UI"
            onLinkActivated: gradRect.handleLink(link)
        }
    }

    Row {
        anchors.top: oblace.bottom
        anchors.right: oblace.right

        Button {
            id: replyButton
            width: 20; height: 20
            buttonImageUrl: isOwnTweet ? "../images/delete.png" : "../images/mention.png"
            clickedButtonImageUrl: isOwnTweet? "../images/delete_pressed.png" : "../images/mention_pressed.png"
            onClicked: {
                if (isOwnTweet)
                    gradRect.deleteButtonClicked()
                else
                    gradRect.replyButtonClicked()
            }
        }

        Button {
            id: retweetButton
            width:  20; height: 20
            buttonImageUrl: "../images/retweet.png"
            clickedButtonImageUrl: "../images/retweet_pressed.png"
            onClicked: gradRect.retweetButtonClicked()
        }

        Button {
            id: extraButton
            width: 20; height: 20
            buttonImageUrl: "../images/poraka.png"
            clickedButtonImageUrl: "../images/poraka_pressed.png"
        }
    }

    Text {
        id: sinceTimeText
        x: 0
        color: "#b4b4b4"
        text: gradRect.tweetSinceTime
        smooth: true
        anchors.top: avatarImage.bottom
        font.family: "Arial"
        font.pixelSize: 10
    }

    ListView.onRemove: SequentialAnimation {
        PropertyAction { target: gradRect; property: "ListView.delayRemove"; value: true }
        ParallelAnimation {
            NumberAnimation { target: gradRect; property: "scale"; easing.type: "InOutBack"; to: 0; duration: 500 }
            NumberAnimation { target: gradRect; property: "height"; easing.type: "InOutBack"; to: 0; duration: 500 }
        }
        PropertyAction { target: gradRect; property: "ListView.delayRemove"; value: false }
    }

    ListView.onAdd: ParallelAnimation {
        NumberAnimation { target: gradRect; property: "scale"; easing.type: "InOutBack"; from: 0; to: 1; duration: 500 }
        NumberAnimation { target: gradRect; property: "height"; easing.type: "InOutBack"; from: 0; to: 80; duration: 500 }
    }
}
