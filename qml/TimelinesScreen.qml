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
                color: "#6D6E72"
            }
        }

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        ButtonWithNotification {
            id: htButton
            width: 78
            height: 78
            anchors.left: parent.left; anchors.leftMargin: 12
            anchors.top: parent.top; anchors.topMargin: 12
            buttonImageUrl:  "../images/hometimeline.png"
            //pressedButtonImageUrl: "../images/hometimeline_pressed.png"
            showNotification: homeTimelineModel.numNewTweets

            onClicked: {
                homeTimelineModel.showNewTweets();
                timelines.state = "";   //default state
            }
        }

        ButtonWithNotification {
            id: mentionsButton
            width: 78
            height: 78
            anchors.top: parent.top; anchors.topMargin: 12
            anchors.left: htButton.right; anchors.leftMargin: 12
            buttonImageUrl: "../images/replies.png"
            //pressedButtonImageUrl: "../images/hometimeline_pressed.png"
            showNotification: mentionsModel.numNewTweets

            onClicked: {
                mentionsModel.showNewTweets();
                timelines.state = "mentions"
            }
        }
    }

    TweetList {
        id: homeTimelineList
        width: parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: parent.top
    }

    TweetList {
        id: mentionsList
        x: parent.width
        width: parent.width
        anchors.bottom: bottomToolbar.top
        anchors.top: parent.top
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
