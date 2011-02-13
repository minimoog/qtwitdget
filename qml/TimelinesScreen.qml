import QtQuick 1.0

Item {
    property alias homeTimelineModel: homeTimelineList.model
    property alias mentionsModel: mentionsList.model

    id: timelines

    Rectangle {
        id: bottomToolbar
        height: 96
        border.color: "#474747"
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#737477"
            }
            GradientStop {
                position: 1
                color: "#6D6E71"
            }
        }

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ButtonWithNotification {
            id: htButton
            width: 77; height: 77
            anchors.left: parent.left; anchors.leftMargin: 10
            anchors.top: parent.top; anchors.topMargin: 9
            buttonImageUrl:  "../images/hometimeline.png"
            pressedButtonImageUrl: "../images/hometimeline_pressed.png"
            showNotification: homeTimelineModel.numNewTweets

            onClicked: {
                homeTimelineModel.showNewTweets();
                timelines.state = "";   //default state
                mentionsButton.toggled = false;
                directMessagesButton.toggled = false;
                searchButton.toggled = false;
            }
        }

        ButtonWithNotification {
            id: mentionsButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: htButton.right; anchors.leftMargin: 11
            buttonImageUrl: "../images/replies.png"
            pressedButtonImageUrl: "../images/hometimeline_pressed.png"
            showNotification: mentionsModel.numNewTweets

            onClicked: {
                mentionsModel.showNewTweets();
                timelines.state = "mentions";
                htButton.toggled = false;
                directMessagesButton.toggled = false;
                searchButton.toggled = false;
            }
        }

        ButtonWithNotification {
            id: directMessagesButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: mentionsButton.right; anchors.leftMargin: 11
            buttonImageUrl: "../images/directmessages.png"
            pressedButtonImageUrl: "../images/directmessages_pressed.png"
            //showNotification: mentionsModel.numNewTweets

            onClicked: {
            //    mentionsModel.showNewTweets();
            //    timelines.state = "mentions"
                htButton.toggled = false;
                mentionsButton.toggled = false;
                searchButton.toggled = false;
            }
        }

        ButtonWithNotification {
            id: searchButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: directMessagesButton.right; anchors.leftMargin: 11
            buttonImageUrl: "../images/search.png"
            pressedButtonImageUrl: "../images/search_pressed.png"
            //showNotification: mentionsModel.numNewTweets

            onClicked: {
            //    mentionsModel.showNewTweets();
            //    timelines.state = "mentions"
                htButton.toggled = false;
                mentionsButton.toggled = false;
                directMessagesButton.toggled = false;
            }
        }
    }

    Rectangle {
        id: topToolbar

        height: 82
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#939598"
            }
            GradientStop {
                position: 1
                color: "#8E9093"
            }
        }

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        ButtonImage {
            id: statusUpdateButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: parent.left; anchors.leftMargin: 10
            width: 39; height: 39

            buttonImageUrl: "../images/statusupdate.png"
            pressedButtonImageUrl: "../images/statusupdate_pressed.png"
        }

        ButtonImage {
            id: reloadButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: statusUpdateButton.right; anchors.leftMargin: 9
            width: 39; height: 39

            buttonImageUrl: "../images/reload.png"
            pressedButtonImageUrl: "../images/reload_pressed.png"
        }

        ButtonText {
            id: usernameButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: reloadButton.right; anchors.leftMargin: 10
            width: 145; height: 39

            buttonImageUrl: "../images/username.png"
            pressedButtonImageUrl: "../images/username_pressed.png"
            text: "@minimoog77" //test
        }

        ButtonImage {
            id: settingsButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: usernameButton.right; anchors.leftMargin: 10
            width: 39; height: 39

            buttonImageUrl: "../images/settings.png"
            pressedButtonImageUrl: "../images/settings_pressed.png"
        }

        ButtonImage {
            id: closeButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: settingsButton.right; anchors.leftMargin: 10
            width: 39; height: 39

            buttonImageUrl: "../images/close.png"
            pressedButtonImageUrl: "../images/close_pressed.png"

            onClicked: Qt.quit()
        }
    }

    TweetList {
        id: homeTimelineList
        width: parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: topToolbar.bottom
    }

    TweetList {
        id: mentionsList
        x: parent.width
        width: parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: topToolbar.bottom
        opacity: 0
    }

    states: [
        State {
            name: "mentions"
            PropertyChanges { target: mentionsList; opacity: 1 }
            PropertyChanges { target: mentionsList; x: 0 }
            PropertyChanges { target: homeTimelineList; opacity: 0 }
            PropertyChanges { target: homeTimelineList; x: - parent.width }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { properties: "x, opacity"; duration: 300 }
        }
    ]
}
