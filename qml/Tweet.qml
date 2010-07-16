import Qt 4.7

Rectangle {
    id: gradRect
    property string tweetid     //no support for 64 integers
    property string tweetScreenName: "Screen Name"
    property string tweetText: "Lorem ipsum dolor sit amet."
    property string tweetAvatar: "../images/default_avatar.png"

    property bool isOwnTweet: false

    signal replyDeleteButtonClicked(string id)
    signal retweetButtonClicked(string id)
    //signal favoriteButtonClicked(string id) // ### TODO

    width: gradRect.ListView.view.width;
    height: 100
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#ddf0f5"
        }

        GradientStop {
            position: 1
            color: "#e5faff"
        }
    }

    Image {
        id: avatarBox
        x: 7; y: 7
        source: "../images/avatar_box.png"

        Rectangle {
            id: whiteBorder
            x: 10; y: 10
            width: 49; height: 49
            color: "#ffffff"

            Image {
                id: avatarImage
                x: 1; y: 1
                width: 48; height: 48
                fillMode: Image.Stretch
                smooth: true
                source: tweetAvatar
            }
        }
    }

    Text {
        id: screenName
        x: 84; y: avatarBox.y
        width: gradRect.width - 84
        color: "#018ad9"
        text: gradRect.tweetScreenName
        smooth: false
        font.pointSize: 11
        font.family: "Segoe UI"
    }

    Text {
        id: statusText
        x: 84; y: 27
        width: gradRect.width - statusText.x
        color: "#222222"
        text: gradRect.tweetText
        wrapMode: "WordWrap"
        font.pointSize: 9
        font.family: "Segoe UI"
    }

    Button {
        id: replyButton
        x: 10; y: 80
        buttonImageUrl: {
            if (isOwnTweet)
                return "../images/button_delete.png"
            else
                return "../images/button_reply.png"
        }

        hoverButtonImageUrl: {
            if (isOwnTweet)
                return "../images/button_delete_hover.png"
            else
                return "../images/button_reply_hover.png"
        }

        clickedButtonImageUrl: {
            if (isOwnTweet)
                return "../images/button_delete_click.png"
            else
                return "../images/button_reply_click.png"
        }

        onClicked: gradRect.replyDeleteButtonClicked(gradRect.tweetid)
    }

    Button {
        id: retweetButton
        x: 43; y: 80
        buttonImageUrl: "../images/button_retweet.png"
        hoverButtonImageUrl: "../images/button_retweet_hover.png"
        clickedButtonImageUrl: "../images/button_retweet_click.png"

        onClicked: gradRect.retweetButtonClicked(gradRect.tweetid)
    }

    Button {
        id: favoriteButton
        x: gradRect.width - 50; y: 80
        buttonImageUrl: "../images/button_favorited.png"
        hoverButtonImageUrl: "../images/button_favorited_hover.png"
        clickedButtonImageUrl: "../images/button_favorited_click.png"
    }
}
