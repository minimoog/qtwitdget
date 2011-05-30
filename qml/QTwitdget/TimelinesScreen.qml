//userInfo external
//userTimelineListModel external
//UserInfoScreen properties should be exposed in Main.qml, too many properties
//maybe later

import QtQuick 1.0
import 'timelinesScreenScripts.js' as StateFunctions

Item {
    id: timelines

    property alias homeTimelineModel: homeTimelineList.model
    property alias mentionsModel: mentionsList.model
    property alias directMessagesModel: dmList.model
    property alias searchModel: searchResultList.model
    property alias conversationModel: conversationList.model
    property string username
    property string notification

    signal sendClicked(string tweetid, string text, string screenname)
    signal retweet(string tweetid)

    //width: 360; height: 640

    function doReply(id, name, text) {
        tweetUpdate.setReply(id, name, text)
        tweetUpdate.state = 'show'
    }

    Row {
        id: rowTimelines
        anchors.top: topToolbar.bottom
        anchors.bottom: bottomToolbar.top
        width: parent.width

        TweetList {
            id: homeTimelineList
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onTweetClicked: StateFunctions.showTweetInfo(tweetid, tweettext, screenname)

            //onUserinformation: StateFunctions.showUser(screenname)
            //onHashtagClicked: StateFunctions.searchHashtag(hashtag)
            //onReply: doReply(id, screenname, tweettext)
            //onRetweet: timelines.retweet(tweetid)
            //onConversation: StateFunctions.showConversation(id)
        }

        TweetList {
            id: mentionsList
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onTweetClicked: StateFunctions.showTweetInfo(tweetid, tweettext, screenname)

            //onUserinformation: StateFunctions.showUser(screenname)
            //onHashtagClicked: StateFunctions.searchHashtag(hashtag)
            //onReply: doReply(id, screenname, tweettext)
            //onRetweet: timelines.retweet(tweetid)
            //onConversation: StateFunctions.showConversation(id)
        }

        TweetList {
            id: dmList
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            //anchors.left: mentionsList.right

            //onUserinformation: StateFunctions.showUser(screenname)
            //onHashtagClicked: StateFunctions.searchHashtag(hashtag)
            //onReply: {
            //    tweetUpdate.setDirectMessage(screenname)
            //   tweetUpdate.state = 'show'
            //}
        }

        SearchList {
            id: searchResultList
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onTweetClicked: StateFunctions.showTweetInfo(tweetid, tweettext, screenname)

            //onUserinformation: StateFunctions.showUser(screenname)
            //onHashtagClicked: StateFunctions.searchHashtag(hashtag)
            //onReply: doReply(id, screenname, tweettext)
            //onRetweet: timelines.retweet(tweetid)
            //onConversation: StateFunctions.showConversation(id)
        }
    }

    UserInfoScreen {
        id: userinformation
        opacity: 0
        width: parent.width
        anchors.top: topToolbar.bottom
        anchors.bottom: bottomToolbar.top

        //should be exposed in Main.qml
        avatar: userInfo.avatarUrl
        screenname: userInfo.screenName
        name: userInfo.name
        url: userInfo.url
        location: userInfo.location
        description: userInfo.description
        isFriend: userInfo.isFriend
        model: userTimelineListModel

        Behavior on opacity {
            NumberAnimation { duration: 500 }
        }

        onReply: doReply(id, screenname, tweettext)
        onRetweet: retweet(tweetid)
        onConversation: StateFunctions.showConversation(id)
        onUserinformation: StateFunctions.showUser(screenname)
        onHashTagClicked: StateFunctions.searchHashtag(hashtag)

        onMessage: {
            tweetUpdate.setDirectMessage(screenname)
            tweetUpdate.state = 'show'
        }

        onFollow: userInfo.followUser(screenname)
        onUnfollow: userInfo.unfollowUser(screenname)
        //onFavoriteButtonClicked: userInfo.createFavorite(statusid)
    }

    TweetList {
        id: conversationList
        opacity: 0
        width: parent.width
        anchors.top: topToolbar.bottom
        anchors.bottom: bottomToolbar.top

        Behavior on opacity {
            NumberAnimation { duration: 500 }
        }

        //onUserinformation: StateFunctions.showUser(screenname)
        //onHashtagClicked: StateFunctions.searchHashtag(hashtag)
        //onReply: doReply(id, screenname, tweettext)
        //onRetweet: timelines.retweet(tweetid)

        // ### TODO add conversation
    }

    TweetScreen {
        id: tweetScreen
        opacity: 0
        width: parent.width
        anchors.top: topToolbar.bottom
        anchors.bottom: bottomToolbar.top

        Behavior on opacity { SmoothedAnimation { } }

        onReplyClicked: doReply(tweetid, screenname, tweettext)
        onRetweetClicked: retweet(tweetid)
        onConversationClicked: StateFunctions.showConversation(tweetid)
        onEntityMentionClicked: StateFunctions.showUser(screenname)
        onEntityHashtagClicked: StateFunctions.searchHashtag(hashtag)
    }

    StatusUpdate {
        id: tweetUpdate
        y: - (height - topToolbar.height) //topToolbar.height
        opacity: 0
        anchors.left: parent.left
        anchors.right: parent.right

        states: State {
            name: "show"
            PropertyChanges { target: tweetUpdate;  y: topToolbar.height; opacity: 1}
        }

        transitions: [
            Transition {
                from: "";
                to: "show"
                SequentialAnimation {
                    PropertyAction { property: "opacity"; value: 1 }
                    NumberAnimation { property: "y"; duration: 500; easing.type: Easing.InOutBack }
                }
            },
            Transition {
                from: "show"
                to: ""
                SequentialAnimation {
                    NumberAnimation { property: "y"; duration: 500; easing.type: Easing.InOutBack }
                    PropertyAction { property: "opacity"; value: 0 }
                }
            }
        ]

        onSendButtonClicked: {
            sendClicked(tweetUpdate.tweetid, tweetUpdate.updateText, tweetUpdate.screenname);
            tweetUpdate.clear();
            tweetUpdate.state = "";
        }
    }

    Rectangle {
        id: topToolbar

        height: 51
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

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

        ButtonImage {
            id: backButton
            buttonImageUrl: 'images/back.png'
            pressedButtonImageUrl: 'images/back_pressed.png'
            width: 87; height: 39
            //sourceSizeHeight: 87; sourceSizeWidth: 39
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left; anchors.leftMargin: 4

            onClicked: StateFunctions.goBack()
        }

        ButtonImage {
            id: statusUpdateButton

            anchors.verticalCenter: parent.verticalCenter
            width: 39; height: 39
            anchors.left: backButton.right
            anchors.leftMargin: 5

            buttonImageUrl: "images/statusupdate.png"
            pressedButtonImageUrl: "images/statusupdate_pressed.png"

            onClicked: {
                // ### TODO Toogle Button
                if (tweetUpdate.state == 'show')
                    tweetUpdate.state = ''
                else {
                    tweetUpdate.state = 'show'
                    tweetUpdate.clear()
                }
            }
        }

        ButtonText {
            id: usernameButton

            width: 125; height: 39

            buttonImageUrl: "images/username.png"
            pressedButtonImageUrl: "images/username_pressed.png"
            text: timelines.username
            anchors.left: statusUpdateButton.right; anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter

            onClicked: StateFunctions.showUser(text)
        }

        ButtonImage {
            id: settingsButton

            anchors.verticalCenter: parent.verticalCenter
            anchors.right: closeButton.left; anchors.rightMargin: 5
            width: 39; height: 39

            buttonImageUrl: "images/settings.png"
            pressedButtonImageUrl: "images/settings_pressed.png"
        }
        
        ButtonImage {
            id: closeButton

            anchors.top: parent.top; anchors.topMargin: 5
            anchors.right: parent.right; anchors.rightMargin: 5
            width: 39; height: 39

            buttonImageUrl: "images/close.png"
            pressedButtonImageUrl: "images/close_pressed.png"

            onClicked: Qt.quit()
        }
    }
    
    Notification {
        id: notificationArea

        height: 50
        anchors.left: parent.left
        anchors.right: parent.right
        y: bottomToolbar.y

        Connections {
            target: rootWindow
            onShowNotification: notificationArea.show(notificationText)
        }
    }

    Rectangle {
        id: bottomToolbar
        height: 50
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

        Row {
            spacing: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            ButtonWithNotification {
                id: homeTimelineButton
                width: 76; height: 40
                buttonImageUrl:  "images/hometimeline.png"
                pressedButtonImageUrl: "images/hometimeline_pressed.png"
                toggled: true
                showNotification: homeTimelineModel.numNewTweets

                onClicked: StateFunctions.showHometimeline()
            }

            ButtonWithNotification {
                id: mentionsButton
                width: 76; height: 40
                buttonImageUrl: "images/replies.png"
                pressedButtonImageUrl: "images/replies_pressed.png"
                showNotification: mentionsModel.numNewTweets

                onClicked: StateFunctions.showMentions()
            }

            ButtonWithNotification {
                id: directMessagesButton
                width: 76; height: 40
                buttonImageUrl: "images/directmessages.png"
                pressedButtonImageUrl: "images/directmessages_pressed.png"
                showNotification: directMessagesModel.numNewDirectMessages

                onClicked: StateFunctions.showDirectMessages()
            }

            ButtonWithNotification {
                id: searchButton
                width: 76; height: 40
                buttonImageUrl: "images/search.png"
                pressedButtonImageUrl: "images/search_pressed.png"

                onClicked: StateFunctions.showSearch()
            }
        }
    }

    //need some work

    states: [
        State {
            name: "mentions"
            PropertyChanges { target: rowTimelines; x: - rowTimelines.width }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: true }
            PropertyChanges { target: directMessagesButton; toggled: false }
            PropertyChanges { target: searchButton; toggled: false }
        },
        State {
            name: "directMessages"
            PropertyChanges { target: rowTimelines; x: - 2 * rowTimelines.width }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: false }
            PropertyChanges { target: directMessagesButton; toggled: true }
            PropertyChanges { target: searchButton; toggled: false }
        },
        State {
            name: "search"
            PropertyChanges { target: rowTimelines; x: - 3 * rowTimelines.width }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: false }
            PropertyChanges { target: directMessagesButton; toggled: false }
            PropertyChanges { target: searchButton; toggled: true }
        },
        State {
            name: "userinfo"
            PropertyChanges { target: userinformation; opacity: 1}
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: false }
            PropertyChanges { target: directMessagesButton; toggled: false }
            PropertyChanges { target: searchButton; toggled: false }
        },
        State {
            name: "conversation"
            PropertyChanges { target: conversationList; opacity: 1 }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: false }
            PropertyChanges { target: directMessagesButton; toggled: false }
            PropertyChanges { target: searchButton; toggled: false }
        },
        State {
            name: 'tweetinfo'
            PropertyChanges { target: tweetScreen; opacity: 1 }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: false }
            PropertyChanges { target: directMessagesButton; toggled: false }
            PropertyChanges { target: searchButton; toggled: false }
        }
    ]

    transitions: [
        Transition {
            NumberAnimation { properties: "x"; duration: 500; easing.type: Easing.InOutExpo }
        }
    ]
}
