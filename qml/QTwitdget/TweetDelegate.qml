import QtQuick 1.0

Item  {
    id: container

    property string tweetid
    property string tweetScreenName: "Screen Name"
    property string tweetText: "Lorem ipsum dolor sit amet consectetur adipiscing elit. Etiam ac venenatis ante. Ut euismod tempor erat, eget tincidunt elit ultricies sed."
    property string tweetAvatar
    property string tweetSinceTime: "sometimes ago"
    property bool isOwnTweet: false
    property bool isNewTweet: false

    signal replyClicked
    signal retweetClicked
    signal favoriteClicked
    signal conversationClicked
    signal mentionLinkClicked(string screenname)
    signal hashtagLinkClicked(string hashtag)

    width: ListView.view.width;
    //width: 360;
    height: (statusText.paintedHeight < 87) ? 97 : (statusText.paintedHeight + 3)

    function handleLink(link) {
        if (link.slice(0, 3) == 'tag') {
            hashtagLinkClicked(link.slice(6))
        } else if (link.slice(0, 4) == 'http') {
            Qt.openUrlExternally(link);
        } else if (link.slice(0, 7) == 'mention') {
            mentionLinkClicked(link.slice(10));
        }
    }

    function addTags(str) {
        //surrounds http links with html link tags
        var ret1 = str.replace(/@[a-zA-Z0-9_]+/g, '<a href="mention://$&" style="color:rgb(0,129,230)">$&</a>');
        var ret2 = ret1.replace(/(\b(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig, '<a href="$1" style="color:rgb(0,129,230)">$1</a>');
        var ret3 = ret2.replace(/[#]+[A-Za-z0-9-_]+/g, '<a href="tag://$&" style="color:rgb(0,129,230)">$&</a>')
        return ret3;
    }

    Image {
        id: background
        anchors.fill: parent
        source: {
            if (isOwnTweet)
                return "images/blue_gradient.png"
            else if (isNewTweet)
                return "images/yellow_gradient.png"
            else
                return "images/gray_gradient.png"
        }
    }

    Rectangle {
        id: avatarBackground
        width: 60
        height: 60
        color: "#00000000"
        border.width: 4
        anchors.top: parent.top
        anchors.topMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4
        border.color: "#2bace2"

        Image {
            id: avatarImage
            width: 56
            height: 56
            anchors.centerIn: parent
            smooth: true
            fillMode: Image.Stretch
            source: tweetAvatar
        }

        MouseArea {
            id: avatarMouseArea
            anchors.fill: parent

            onClicked: mentionLinkClicked(tweetScreenName)
        }
    }

    MouseArea {
        id: moreArea
        anchors.bottomMargin: 0
        anchors.leftMargin: -20
        anchors.top: parent.top
        anchors.bottom: buttonLoader.top
        anchors.left: rightArrow.left
        anchors.right: parent.right

        onClicked: {
            if (container.state == 'showButtons')
                container.state = ''
            else
                container.state = 'showButtons'
        }
    }

    Image {
        id: rightArrow
        source: "images/right_arrow.png"
        width: 11; height: 16
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        id: statusText
        color: "#333333"
        text: '<b>' + tweetScreenName + ':<\/b><br \/> ' + addTags(tweetText)
        //text: '<b>' + tweetScreenName + ':<\/b><br \/> ' + tweetText
        anchors.topMargin: 0
        anchors.top: parent.top;
        anchors.right: rightArrow.left; anchors.rightMargin: 0
        anchors.left: avatarBackground.right; anchors.leftMargin: 4
        textFormat: Text.RichText
        wrapMode: "WordWrap"
        font.family: "Segoe UI"
        font.pointSize: 6

        onLinkActivated: container.handleLink(link);
    }

    Text {
        id: sinceText
        text: tweetSinceTime
        anchors.top: avatarBackground.bottom
        anchors.leftMargin: 10
        anchors.topMargin: 4
        anchors.left: parent.left
        font.family: "Segoe UI"
        font.pointSize: 5
    }

    Loader {
        id: buttonLoader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    Connections { target: buttonLoader.item; onReplyButtonClicked: replyClicked() }
    Connections { target: buttonLoader.item; onRetweetButtonClicked: retweetClicked() }
    Connections { target: buttonLoader.item; onFavoriteButtonClicked: favoriteClicked() }
    Connections { target: buttonLoader.item; onConversationButtonClicked: conversationClicked() }

    Component {
        id: buttonRow

        Item {
            id: buttonContainer

            signal replyButtonClicked()
            signal retweetButtonClicked()
            signal favoriteButtonClicked()
            signal conversationButtonClicked()

            ButtonImage {
                id: replyButton
                width: 84
                height: 23
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.left: parent.left
                anchors.leftMargin: 5
                pressedButtonImageUrl: "images/reply_button_pressed.png"
                buttonImageUrl: "images/reply_button.png"

                onClicked: replyButtonClicked()
            }

            ButtonImage {
                id: retweetButton
                width: 84
                height: 23
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.left: replyButton.right
                anchors.leftMargin: 5
                pressedButtonImageUrl: "images/retweet_button_pressed.png"
                buttonImageUrl: "images/retweet_button.png"

                onClicked: retweetButtonClicked()
            }

            ButtonImage {
                id: favouriteButton
                width: 84
                height: 23
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.left: retweetButton.right
                anchors.leftMargin: 5
                pressedButtonImageUrl: "images/favourite_button_pressed.png"
                buttonImageUrl: "images/favourite_button.png"

                onClicked: favoriteButtonClicked()
            }

            ButtonImage {
                id: conversationButton
                width: 84
                height: 23
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.left: favouriteButton.right
                anchors.leftMargin: 5
                pressedButtonImageUrl: "images/conversation_button_pressed.png"
                buttonImageUrl: "images/conversation_button.png"

                onClicked: conversationButtonClicked()
            }
        }
    }

    states: State {
        name: 'showButtons'
        PropertyChanges { target: buttonLoader; sourceComponent: buttonRow }
        PropertyChanges { target: container; height: container.height + 23; explicit: true }
        PropertyChanges { target: rightArrow; rotation: 90 }
    }

    transitions: Transition {
            NumberAnimation { target: container; property: "height"; duration: 200 }
            NumberAnimation { target: rightArrow; property: "rotation"; duration: 200 }
    }

    ListView.onAdd: SequentialAnimation {
        ParallelAnimation {
            PropertyAction { target: container; property: "height"; value:  0 }
            PropertyAction { target: container; property: "x"; value: container.width }
            PropertyAction { target: container; property: "opacity"; value: 0 }
        }
        NumberAnimation { target: container; property: "height"; easing.type: Easing.OutBounce; from: 0; to: (statusText.paintedHeight < 87) ? 95 : (statusText.paintedHeight + 4); duration: 300 }
        PropertyAction { target: container; property: "opacity"; value: 1 }
        PauseAnimation { duration: index * 20 }
        NumberAnimation { target: container; property: "x"; easing.type: Easing.InOutBack; to: 0; duration: 500 }
    }

    ListView.onRemove: SequentialAnimation {
        PropertyAction { target: container; property: "ListView.delayRemove"; value: true }
        NumberAnimation { target: container; property: "opacity"; to: 0; duration: 300 }
        NumberAnimation { target: container; property: "height"; to: 0; duration: 300 }
        PropertyAction { target: container; property: "ListView.delayRemove"; value: false }
    }
}
