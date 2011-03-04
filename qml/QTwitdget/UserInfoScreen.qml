import QtQuick 1.0

Item {
    id: container

    property string text
    property string time
    property string avatar : "images/avatar.png"
    property string screenname
    property string name
    property string url
    property string location
    property string description
    property int numTweets : 0
    property int numFollowers : 0
    property int numFollowing : 0
    property int numFavorites : 0

    //width: 360; height: 640

    Rectangle {
        id: userInfoBackground
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
        anchors.fill: parent

        Image {
            id: oblace
            anchors.left: parent.left
            anchors.right: parent.right

            source: "images/oblace.png"

            Text {
                id: tweetText
                text: container.text
                font.pointSize: 10
                style: Text.Normal
                font.family: "Segoe UI"
                wrapMode: Text.WordWrap
                anchors.right: parent.right; anchors.rightMargin: 11
                anchors.left: parent.left; anchors.leftMargin: 11
                anchors.top: parent.top; anchors.topMargin: 18
            }
            Text {
                id: since
                text: container.time
                anchors.bottom: oblace.bottom
                anchors.right: parent.right
                anchors.rightMargin: 16
            }
        }

        Image {
            id: userAvatar
            anchors.top: oblace.bottom
            anchors.topMargin: 12
            anchors.left: parent.left
            anchors.leftMargin: 11
            source: container.avatar
        }
        Text {
            id: userScreenname
            text: container.screenname
            font.pointSize: 10
            anchors.left: userAvatar.right
            anchors.leftMargin: 11
            anchors.top: userAvatar.top
            font.family: "Segoe UI"
            font.bold: true
        }
        Text {
            id: userName
            color: "#666666"
            text: container.name
            font.pointSize: 10
            font.bold: true
            font.family: "Segoe UI"
            anchors.left: userAvatar.right
            anchors.leftMargin: 11
            anchors.top: userScreenname.bottom
            anchors.topMargin: 0
        }
        Text {
            id: userUrl
            color: "#528ca8"
            text:  container.url
            anchors.left: userAvatar.right
            anchors.leftMargin: 11
            anchors.top: userName.bottom
            anchors.topMargin: 0
            font.family: "Segoe UI"
        }
        Text {
            id: userLocation
            color: "#666666"
            text: container.location
            anchors.left: userAvatar.right
            anchors.leftMargin: 11
            anchors.top: userUrl.bottom
            anchors.topMargin: 0
            font.family: "Segoe UI"
        }
        Text {
            id: userDescription
            color: "#919191"
            text: container.description
            font.pointSize: 10
            anchors.right: parent.right
            anchors.rightMargin: 11
            anchors.left: parent.left
            anchors.leftMargin: 11
            anchors.top: userAvatar.bottom
            anchors.topMargin: 13
            font.family: "Segoe UI"
            wrapMode: Text.WordWrap
        }
        Image {
            id: numTweetsBackground
            anchors.top: userDescription.bottom
            anchors.topMargin: 40
            source: "images/lenta_nadesno.png"
            Text {
                id: userNumTweets
                color: "#ffffff"
                text: container.numTweets
                font.pointSize: 8
                font.family: "Segoe UI"
                anchors.topMargin: 2
                anchors.leftMargin: 5
                anchors.fill: parent
            }
        }
        Text {
            color: "#919191"
            text: "tweets"
            font.pointSize: 10
            font.family: "Segoe UI"
            anchors.top: userDescription.bottom
            anchors.topMargin: 40
            anchors.left: numTweetsBackground.right
            anchors.leftMargin: 7
        }
        Image {
            id: numFollowersBackground
            anchors.top: numTweetsBackground.bottom
            anchors.topMargin: 6
            source: "images/lenta_nadesno.png"
            Text {
                id: userNumFollowers
                color: "#ffffff"
                text: container.numFollowers
                font.pointSize: 8
                font.family: "Segoe UI"
                anchors.topMargin: 2
                anchors.leftMargin: 5
                anchors.fill: parent
            }
        }
        Text {
            color: "#919191"
            text: 'followers'
            anchors.top: userDescription.bottom
            anchors.topMargin: 64
            anchors.left: numFollowersBackground.right
            anchors.leftMargin: 7
            font.pointSize: 10
            font.family: "Segoe UI"
        }

        Image {
            id: numFollowingBackground
            anchors.top: userDescription.bottom
            anchors.topMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 0
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
            color: "#919191"
            text: "following"
            anchors.right: numFollowingBackground.left
            anchors.rightMargin: 7
            anchors.top: userDescription.bottom
            anchors.topMargin: 40
            font.pointSize: 10
            font.family: "Segoe UI"
        }

        Image {
            id: numFavoritesBackground
            anchors.top: numFollowingBackground.bottom
            anchors.topMargin: 6
            anchors.right: parent.right
            anchors.rightMargin: 0
            source: "images/lenta_nalevo.png"
            Text {
                id: userNumFavorites
                x: 0
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
            color: "#919191"
            text: "favorites"
            anchors.right: numFavoritesBackground.left
            anchors.rightMargin: 7
            anchors.top: userDescription.bottom
            anchors.topMargin: 64
            font.pointSize: 10
            font.family: "Segoe UI"
        }

        ButtonImage {
            id: unfollowButton
            width: 94; height: 26
            anchors.left: parent.left
            anchors.leftMargin: 71
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 42
            buttonImageUrl: "images/unfollow_button.png"
            pressedButtonImageUrl: "images/unfollow_button_pressed.png"
        }
        ButtonImage {
            id: blockButton
            width: 94; height: 26
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 42
            anchors.right: parent.right
            anchors.rightMargin: 79
            buttonImageUrl: "images/block_button.png"
            pressedButtonImageUrl: "images/block_button_pressed.png"
        }
    }
}
