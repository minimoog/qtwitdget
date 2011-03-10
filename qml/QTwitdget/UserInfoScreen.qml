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
    property int numTweets
    property int numFollowers
    property int numFollowing
    property int numFavorites

    signal hashTagClicked(string hashtag)
    signal mentionClicked(string mention)   //doesn't need to go outside
    signal replyButtonClicked
    signal retweetButtonClicked
    signal messageButtonClicked

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
                font.pointSize: 10
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
            anchors.right: labelFollowers.left
            anchors.rightMargin: 5
            font.pointSize: 10
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
            anchors.right: labelFollowers.left
            anchors.rightMargin: 5
            font.pointSize: 10
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
            anchors.right: labelFollowers.left
            anchors.rightMargin: 5
            anchors.left: userAvatar.right; anchors.leftMargin: 11
            anchors.top: userName.bottom
            font.family: "Segoe UI"
        }
        Text {
            id: userLocation
            color: "#666666"
            text: container.location
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            anchors.right: labelFollowers.left
            anchors.rightMargin: 5
            anchors.left: userAvatar.right; anchors.leftMargin: 11
            anchors.top: userUrl.bottom
            font.family: "Segoe UI"
        }
        Text {
            id: userDescription
            color: "#919191"
            text: container.description
            font.pointSize: 10
            anchors.right: parent.right; anchors.rightMargin: 5
            anchors.left: parent.left; anchors.leftMargin: 5
            anchors.top: numFavoritesBackground.bottom; anchors.topMargin: 12
            font.family: "Segoe UI"
            wrapMode: Text.WordWrap
        }
        Image {
            id: numTweetsBackground
            anchors.right: parent.right
            anchors.top: oblace.bottom; anchors.topMargin: 5
            source: "images/lenta_nalevo.png"
            Text {
                id: userNumTweets
                color: "#ffffff"
                text: container.numTweets
                anchors.right: parent.right; anchors.rightMargin: 5
                anchors.top: parent.top
                font.pointSize: 8
                font.family: "Segoe UI"
                anchors.topMargin: 2
            }
        }
        Text {
            id: labelTweets
            color: "#919191"
            text: "tweets"
            anchors.top: oblace.bottom; anchors.topMargin: 5
            anchors.right: numTweetsBackground.left
            font.pointSize: 10
            font.family: "Segoe UI"
        }
        Image {
            id: numFollowersBackground
            anchors.right: parent.right
            anchors.top: numTweetsBackground.bottom
            source: "images/lenta_nalevo.png"
            Text {
                id: userNumFollowers
                color: "#ffffff"
                text: container.numFollowers
                anchors.rightMargin: 5
                anchors.right: parent.right
                anchors.top: parent.top
                font.pointSize: 8
                font.family: "Segoe UI"
                anchors.topMargin: 2
            }
        }
        Text {
            id: labelFollowers
            color: "#919191"
            text: 'followers'
            anchors.top: labelTweets.bottom
            anchors.topMargin: 1
            anchors.right: numFollowersBackground.left
            font.pointSize: 10
            font.family: "Segoe UI"
        }

        Image {
            id: numFollowingBackground
            anchors.top: numFollowersBackground.bottom
            anchors.right: parent.right
            source: "images/lenta_nalevo.png"
            Text {
                id: userNumFollowing
                color: "#ffffff"
                text: container.numFollowing
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.right: parent.right
                anchors.rightMargin: 5
                font.pointSize: 8
                font.family: "Segoe UI"
            }
        }
        Text {
            id: labelFollowing
            color: "#919191"
            text: "following"
            anchors.right: numFollowingBackground.left
            anchors.top: labelFollowers.bottom
            anchors.topMargin: 1
            font.pointSize: 10
            font.family: "Segoe UI"
        }

        Image {
            id: numFavoritesBackground
            anchors.top: numFollowingBackground.bottom
            anchors.right: parent.right
            source: "images/lenta_nalevo.png"
            Text {
                id: userNumFavorites
                color: "#ffffff"
                text: container.numFavorites
                anchors.top: parent.top
                anchors.topMargin: 2
                anchors.right: parent.right
                anchors.rightMargin: 5
                font.pointSize: 8
                font.family: "Segoe UI"
            }
        }
        Text {
            id: labelFavorites
            color: "#919191"
            text: "favorites"
            anchors.right: numFavoritesBackground.left
            anchors.top: labelFollowing.bottom
            font.pointSize: 10
            font.family: "Segoe UI"
        }

        ButtonImage {
            id: unfollowButton
            width: 84; height: 23
            anchors.top: userDescription.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 5
            buttonImageUrl: "images/unfollow_button.png"
            pressedButtonImageUrl: "images/unfollow_button_pressed.png"
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
