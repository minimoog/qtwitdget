import QtQuick 1.0

Item {
    property alias model : tweetSimpleList.model

    id: container

    property string avatar : "images/avatar.png"
    property string screenname
    property string name
    property string url
    property string location
    property string description
    property bool isFriend

    signal reply(string id, string screenname, string tweettext)
    signal retweet(string text, string screenname)
    signal conversation(string id)
    signal hashTagClicked(string hashtag)
    signal userinformation(string screenname)
    signal message
    signal follow
    signal unfollow

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

    //width: 360; height: 640

    Rectangle {
        id: userInfoBackground
        //97
        height: (userDescription.paintedHeight < 70) ? 97 : userDescription.paintedHeight + 35
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
            id: userAvatar
            anchors.top: parent.top; anchors.topMargin: 4
            anchors.left: parent.left; anchors.leftMargin: 4
            source: container.avatar
        }
        Text {
            id: userDescription
            text: container.screenname + '<br \/> ' + container.name + '<br \/> ' + container.url + '<br \/> ' + container.location + '<br \/> ' + container.description
            anchors.right: parent.right
            wrapMode: Text.WordWrap
            anchors.topMargin: 4
            anchors.rightMargin: 4
            font.pointSize: 6
            anchors.left: userAvatar.right; anchors.leftMargin: 4
            anchors.top: parent.top
            font.family: "Segoe UI"
        }

        ButtonImage {
            id: unfollowButton
            width: 84; height: 23
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.left: parent.left
            anchors.leftMargin: 4
            smooth: true
            buttonImageUrl: container.isFriend ? 'images/unfollow_button.png' : 'images/follow_button.png'
            pressedButtonImageUrl: container.isFriend ? 'images/unfollow_button_pressed.png' : 'images/follow_button_pressed.png'

            onClicked: {
                if (container.isFriend)
                    unfollow()
                else
                    follow()
            }
        }
        ButtonImage {
            id: blockButton
            width: 84; height: 23
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.left: unfollowButton.right
            anchors.leftMargin: 4
            buttonImageUrl: "images/block_button.png"
            pressedButtonImageUrl: "images/block_button_pressed.png"
        }

        ButtonImage {
            id: messageButton
            width: 84
            height: 23
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.left: blockButton.right
            anchors.leftMargin: 4
            pressedButtonImageUrl: "images/message_button_pressed.png"
            buttonImageUrl: "images/message_button.png"

            onClicked: message()
        }
    }

    TweetSimpleList {
        id: tweetSimpleList
        anchors.top: userInfoBackground.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        // ### TODO rename the signal this is confusing
        onReplyButtonClicked: reply(statusid, screenname, statustext)
        onRetweet: container.retweet(tweettext, screenname)
        onConversation: container.conversation(id)
        onUserinformation: container.userinformation(screenname)
        onHashtagClicked: container.hashTagClicked(hashtag)
    }
}
