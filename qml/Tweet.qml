import Qt 4.7

Rectangle {
    id: gradRect
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
    width: 200

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
                source: "../images/default_avatar.png"
            }
        }
    }

    Text {
        id: screenName
        x: 84; y: avatarBox.y
        width: gradRect.width - 84
        color: "#018ad9"
        text: "Screen name"
        smooth: false
        font.pointSize: 11
        font.family: "Segoe UI"
    }

    Text {
        id: statusText
        x: 84; y: 27
        width: gradRect.width - statusText.x
        color: "#222222"
        text: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In ac bibendum lectus. Cras et lacus nec nisi sagittis auctor sit amet quis nullam. "
        wrapMode: "WordWrap"
        font.pointSize: 9
        font.family: "Segoe UI"
    }

    Button {
        id: replyButton
        x: 10; y: 80
        buttonImageUrl: "../images/button_reply.png"
        hoverButtonImageUrl: "../images/button_reply_hover.png"
        clickedButtonImageUrl: "../images/button_reply_click.png"
    }

    Button {
        id: retweetButton
        x: 43; y: 80
        buttonImageUrl: "../images/button_retweet.png"
        hoverButtonImageUrl: "../images/button_retweet_hover.png"
        clickedButtonImageUrl: "../images/button_retweet_click.png"
    }

    Button {
        id: favoriteButton
        x: gradRect.width - 50; y: 80
        buttonImageUrl: "../images/button_favorited.png"
        hoverButtonImageUrl: "../images/button_favorited_hover.png"
        clickedButtonImageUrl: "../images/button_favorited_click.png"
    }
}
