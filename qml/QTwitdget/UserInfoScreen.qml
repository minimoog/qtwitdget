import QtQuick 1.0

Item {
    property alias model : tweetSimpleList.model

    id: container

    property string text
    property string statusid
    property string time
    property string avatar : "images/avatar.png"
    property string screenname
    property string name
    property string url
    property string location
    property string description
    property bool isFriend

    signal hashTagClicked(string hashtag)
    signal mentionClicked(string mention)   //doesn't need to go outside
    signal replyButtonClicked
    signal retweetButtonClicked
    signal messageButtonClicked
    signal conversationButtonClicked
    signal followButtonClicked
    signal unfollowButtonClicked
    signal favoriteButtonClicked

    function addTags(str) {
        //surrounds http links with html link tags
        var ret1 = str.replace(/@[a-zA-Z0-9_]+/g, '<a href="mention://$&">$&</a>');
        var ret2 = ret1.replace(/(\b(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig, "<a href='$1'>$1</a>");
        var ret3 = ret2.replace(/[#]+[A-Za-z0-9-_]+/g, '<a href="tag://$&">$&</a>')
        return ret3;
    }

    function handleLink(link) {
        if (link.slice(0, 3) == 'tag') {
            hashTagClicked(link.slice(6))
        } else if (link.slice(0, 4) == 'http') {
            Qt.openUrlExternally(link);
        } else if (link.slice(0, 7) == 'mention') {
            mentionClicked(link.slice(10));
        }
    }

    width: 360; height: 640

    Rectangle {
        id: userInfoBackground
        height: oblace.height + userDescription.paintedHeight + 140 //296
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
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

        Image {
            id: oblace
            height: (container.text) ? (tweetText.paintedHeight + 55) : 0
            visible: container.text
            anchors.left: parent.left
            anchors.right: parent.right

            source: "images/oblace.png"

            Behavior on height { PropertyAnimation { duration: 200 } }

            Text {
                id: tweetText
                text: addTags(container.text)
                font.pointSize: 6
                style: Text.Normal
                font.family: "Segoe UI"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                anchors.right: parent.right; anchors.rightMargin: 5
                anchors.left: parent.left; anchors.leftMargin: 5
                anchors.top: parent.top; anchors.topMargin: 5

                onLinkActivated: container.handleLink(link)
            }
            Text {
                id: since
                text: container.time
                font.pointSize: 5
                anchors.bottom: oblace.bottom
                anchors.right: parent.right; anchors.rightMargin: 16
            }

            ButtonImage {
                id: retweetButton
                width: 84
                height: 23
                anchors.top: tweetText.bottom
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 5
                pressedButtonImageUrl: "images/retweet_button_pressed.png"
                buttonImageUrl: "images/retweet_button.png"

                onClicked: retweetButtonClicked()
            }

            ButtonImage {
                id: replyButton
                width: 84
                height: 23
                pressedButtonImageUrl: "images/reply_button_pressed.png"
                anchors.top: tweetText.bottom
                anchors.topMargin: 10
                anchors.left: retweetButton.right
                anchors.leftMargin: 5
                buttonImageUrl: "images/reply_button.png"

                onClicked: replyButtonClicked()
            }

            ButtonImage {
                id: favouriteButton
                width: 84
                height: 23
                anchors.top: tweetText.bottom
                anchors.topMargin: 10
                anchors.left: replyButton.right
                anchors.leftMargin: 5
                pressedButtonImageUrl: "images/favourite_button_pressed.png"
                buttonImageUrl: "images/favourite_button.png"

                onClicked: favoriteButtonClicked()
            }

            ButtonImage {
                id: conversationButton
                width: 84
                height: 23
                anchors.left: favouriteButton.right
                anchors.leftMargin: 5
                anchors.top: tweetText.bottom
                anchors.topMargin: 10
                pressedButtonImageUrl: "images/conversation_button_pressed.png"
                buttonImageUrl: "images/conversation_button.png"

                onClicked: conversationButtonClicked()
            }
        }

        Image {
            id: userAvatar
            anchors.top: oblace.bottom; anchors.topMargin: 5
            anchors.left: parent.left; anchors.leftMargin: 11
            source: container.avatar
        }
        Text {
            id: userScreenname
            text: container.screenname
            anchors.rightMargin: 5
            font.pointSize: 6
            anchors.left: userAvatar.right; anchors.leftMargin: 11
            anchors.top: userAvatar.top
            font.family: "Segoe UI"
            font.bold: true
        }
        Text {
            id: userName
            color: "#666666"
            text: container.name
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Text.AlignLeft
            anchors.rightMargin: 5
            font.pointSize: 6
            font.bold: true
            font.family: "Segoe UI"
            anchors.left: userAvatar.right; anchors.leftMargin: 11
            anchors.top: userScreenname.bottom
        }
        Text {
            id: userUrl
            color: "#528ca8"
            text:  container.url
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            anchors.rightMargin: 5
            anchors.left: userAvatar.right; anchors.leftMargin: 11
            anchors.top: userName.bottom
            font.family: "Segoe UI"
            font.pointSize: 6
        }
        Text {
            id: userLocation
            color: "#666666"
            text: container.location
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            anchors.rightMargin: 5
            anchors.left: userAvatar.right; anchors.leftMargin: 11
            anchors.top: userUrl.bottom
            font.family: "Segoe UI"
            font.pointSize: 6
        }
        Text {
            id: userDescription
            color: "#919191"
            text: container.description
            font.pointSize: 6
            anchors.right: parent.right; anchors.rightMargin: 5
            anchors.left: parent.left; anchors.leftMargin: 5
            font.family: "Segoe UI"
            wrapMode: Text.WordWrap
        }

        ButtonImage {
            id: unfollowButton
            width: 84; height: 23
            anchors.top: userDescription.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 5
            smooth: true
            buttonImageUrl: container.isFriend ? 'images/unfollow_button.png' : 'images/follow_button.png'
            pressedButtonImageUrl: container.isFriend ? 'images/unfollow_button_pressed.png' : 'images/follow_button_pressed.png'

            onClicked: {
                if (container.isFriend)
                    unfollowButtonClicked()
                else
                    followButtonClicked()
            }
        }
        ButtonImage {
            id: blockButton
            width: 84; height: 23
            anchors.left: unfollowButton.right
            anchors.leftMargin: 5
            anchors.top: userDescription.bottom
            anchors.topMargin: 10
            buttonImageUrl: "images/block_button.png"
            pressedButtonImageUrl: "images/block_button_pressed.png"
        }

        ButtonImage {
            id: messageButton
            width: 84
            height: 23
            anchors.top: userDescription.bottom
            anchors.topMargin: 10
            anchors.left: blockButton.right
            anchors.leftMargin: 5
            pressedButtonImageUrl: "images/message_button_pressed.png"
            buttonImageUrl: "images/message_button.png"

            onClicked: messageButtonClicked()
        }
    }

    TweetSimpleList {
        id: tweetSimpleList
        anchors.top: userInfoBackground.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        onClickedDelegate: {
            statusid = delegateID
            text = delegateText
            time = delegateSinceTime
        }
//        onHashtagClicked: container.hashTagClicked(hashtag)
    }
}
