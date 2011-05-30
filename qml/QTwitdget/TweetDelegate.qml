import QtQuick 1.0

Rectangle  {
    id: container

    property string tweetid
    property string tweetScreenName
    property string tweetText
    property string tweetAvatar
    property string tweetSinceTime
    property bool isOwnTweet: false
    property bool isNewTweet: false

    signal avatarClicked
    signal tweetClicked

    //signal replyClicked
    //signal retweetClicked
    //signal favoriteClicked
    //signal conversationClicked
    //signal mentionLinkClicked(string screenname)
    //signal hashtagLinkClicked(string hashtag)

//    function handleLink(link) {
//        if (link.slice(0, 3) == 'tag') {
//            hashtagLinkClicked(link.slice(6))
//        } else if (link.slice(0, 4) == 'http') {
//            Qt.openUrlExternally(link);
//        } else if (link.slice(0, 7) == 'mention') {
//            mentionLinkClicked(link.slice(10));
//        }
//    }

    width: ListView.view.width;
    //width: 360;
    height: (statusText.paintedHeight < 87) ? 97 : (statusText.paintedHeight + 3)
    color: {
        if (isNewTweet)
            return (index % 2) ? '#ffd954' : '#ffb300'
        else
            return (index % 2) ? '#ffffff' : '#d9d9d9'
    }

    Behavior on color { ColorAnimation { duration: 300 } }

    Rectangle {
        id: avatarBackground
        width: 52
        height: 52
        color:  '#2bace2'
        anchors.top: parent.top
        anchors.topMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4

        Image {
            id: avatarImage
            width: 48
            height: 48
            sourceSize.width: 48
            sourceSize.height: 48
            anchors.centerIn: parent
            fillMode: Image.Stretch
            source: tweetAvatar
        }

//        MouseArea {
//            id: avatarMouseArea
//            anchors.fill: parent

//            //onClicked: mentionLinkClicked(tweetScreenName)
//            onClicked: avatarClicked()
//        }
    }

//    MouseArea {
//        id: moreArea
//        anchors.bottomMargin: 0
//        anchors.leftMargin: -20
//        anchors.top: parent.top
//        anchors.bottom: buttonLoader.top
//        anchors.left: rightArrow.left
//        anchors.right: parent.right

//        onClicked: {
//            if (container.state == 'showButtons')
//                container.state = ''
//            else
//                container.state = 'showButtons'
//        }
//    }

    Text {
        id: statusText
        color: "#333333"
        text: '<b>' + tweetScreenName + ':<\/b><br \/> ' + tweetText
        anchors.topMargin: 0
        anchors.top: parent.top;
        anchors.right: parent.right
        anchors.left: avatarBackground.right; anchors.leftMargin: 4
        textFormat: Text.RichText
        wrapMode: "WordWrap"
        font.family: "Segoe UI"
        font.pointSize: 6

        //onLinkActivated: container.handleLink(link);
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

    MouseArea {
        id: tweetArea
        anchors.fill: parent

        onClicked: tweetClicked()
    }

//    Loader {
//        id: buttonLoader
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//    }

//    Connections { target: buttonLoader.item; onReplyButtonClicked: replyClicked() }
//    Connections { target: buttonLoader.item; onRetweetButtonClicked: retweetClicked() }
//    Connections { target: buttonLoader.item; onFavoriteButtonClicked: favoriteClicked() }
//    Connections { target: buttonLoader.item; onConversationButtonClicked: conversationClicked() }

//    Component {
//        id: buttonRow

//        Item {
//            id: buttonContainer

//            signal replyButtonClicked()
//            signal retweetButtonClicked()
//            signal favoriteButtonClicked()
//            signal conversationButtonClicked()

//            ButtonImage {
//                id: replyButton
//                width: 76; height: 40
//                sourceSizeWidth: 76; sourceSizeHeight: 40
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.right: retweetButton.left
//                anchors.rightMargin: 5
//                pressedButtonImageUrl: "images/reply_button_pressed.png"
//                buttonImageUrl: "images/reply_button.png"

//                onClicked: replyButtonClicked()
//            }

//            ButtonImage {
//                id: retweetButton
//                width: 76; height: 40
//                sourceSizeWidth: 76; sourceSizeHeight: 40
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.right: parent.horizontalCenter
//                anchors.rightMargin: 2
//                pressedButtonImageUrl: "images/retweet_button_pressed.png"
//                buttonImageUrl: "images/retweet_button.png"

//                onClicked: retweetButtonClicked()
//            }

//            ButtonImage {
//                id: favouriteButton
//                width: 76; height: 40
//                sourceSizeWidth: 76; sourceSizeHeight: 40
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.left: retweetButton.right
//                anchors.leftMargin: 5
//                pressedButtonImageUrl: "images/favourite_button_pressed.png"
//                buttonImageUrl: "images/favourite_button.png"

//                onClicked: favoriteButtonClicked()
//            }

//            ButtonImage {
//                id: conversationButton
//                width: 76; height: 40
//                sourceSizeWidth: 76; sourceSizeHeight: 40
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.left: favouriteButton.right
//                anchors.leftMargin: 5
//                pressedButtonImageUrl: "images/conversation_button_pressed.png"
//                buttonImageUrl: "images/conversation_button.png"

//                onClicked: conversationButtonClicked()
//            }
//        }
//    }

//    states: State {
//        name: 'showButtons'
//        PropertyChanges { target: buttonLoader; sourceComponent: buttonRow }
//        PropertyChanges { target: container; height: container.height + 40; explicit: true }
//        PropertyChanges { target: rightArrow; rotation: 90 }
//    }

//    transitions: Transition {
//            NumberAnimation { target: container; property: "height"; duration: 200 }
//            NumberAnimation { target: rightArrow; property: "rotation"; duration: 200 }
//    }

    ListView.onAdd: SequentialAnimation {
        ParallelAnimation {
            PropertyAction { target: container; property: "height"; value:  0 }
            PropertyAction { target: container; property: "x"; value: container.width }
            PropertyAction { target: container; property: "opacity"; value: 0 }
        }
        NumberAnimation { target: container; property: "height"; easing.type: Easing.OutExpo; from: 0; to: (statusText.paintedHeight < 87) ? 95 : (statusText.paintedHeight + 4); duration: 300 }
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
