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
    property bool isOwnTweet: false

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
        source: "../images/oblace.png"

        Text {
            id: statusText
            x: 13; y: 3
            width: parent.width - 16
            color: "#333333"
            text: gradRect.tweetText
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
            width: 12; height: 12
            buttonImageUrl: {
                if (isOwnTweet)
                    return "../images/delete.png"
                else
                    return "../images/mention.png"
            }
            clickedButtonImageUrl: {
                if (isOwnTweet)
                    return "../images/delete_pressed.png"
                else
                    return "../images/mention_pressed.png"
            }
            onClicked: {
                if (isOwnTweet)
                    gradRect.deleteButtonClicked()
                else
                    gradRect.replyButtonClicked()
            }
        }

        Button {
            id: retweetButton
            width:  12; height: 12
            buttonImageUrl: "../images/retweet.png"
            clickedButtonImageUrl: "../images/retweet_pressed.png"
            onClicked: gradRect.retweetButtonClicked()
        }

        Button {
            id: extraButton
            width: 12; height: 12
            buttonImageUrl: "../images/poraka.png"
            clickedButtonImageUrl: "../images/poraka_pressed.png"
        }
    }

    ListView.onRemove: SequentialAnimation {
        PropertyAction { target: gradRect; property: "ListView.delayRemove"; value: true }
        NumberAnimation { target: gradRect; property: "scale"; easing.type: "InOutBack"; to: 0; duration: 500 }
        PropertyAction { target: gradRect; property: "ListView.delayRemove"; value: false }
    }

    ListView.onAdd: SequentialAnimation {
        NumberAnimation { target: gradRect; property: "scale"; easing.type: "InOutBack"; from: 0; to: 1; duration: 500 }
    }
}
