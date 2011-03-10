//userInfo external
//userTimelineListModel external
//UserInfoScreen properties should be exposed in Main.qml, too many properties
//maybe later

import QtQuick 1.0

Item {
    property alias homeTimelineModel: homeTimelineList.model
    property alias mentionsModel: mentionsList.model
    property alias directMessagesModel: dmList.model
    property alias searchModel: searchResultList.model

    signal sendClicked(string tweetid, string text, string screenname)

    id: timelines

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

            onMoreClicked: {
                userInfo.fetchByName(screenname)
                userTimelineListModel.fetch(screenname)
                timelines.state = 'userinfo'
                userinformation.statusid = statusid
                userinformation.text = text
                userinformation.time = sincetime
            }
            onHashtagClicked: {
                searchResultList.doSearch(hashtag)
                timelines.state = 'search'
            }
        }

        TweetList {
            id: mentionsList
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            //anchors.left: homeTimelineList.right

            onMoreClicked: {
                userInfo.fetchByName(screenname)
                userTimelineListModel.fetch(screenname)
                timelines.state = 'userinfo'
                userinformation.statusid = statusid
                userinformation.text = text
                userinformation.time = sincetime
            }
            onHashtagClicked: {
                searchResultList.doSearch(hashtag)
                timelines.state = 'search'
            }
        }

        TweetList {
            id: dmList
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            //anchors.left: mentionsList.right

            onMoreClicked:  {
                userInfo.fetchByName(screenname)
                userTimelineListModel.fetch(screenname)
                timelines.state = 'userinfo'
                userinformation.statusid = statusid
                userinformation.text = text
                userinformation.time = sincetime
            }
            onHashtagClicked: {
                searchResultList.doSearch(hashtag)
                timelines.state = 'search'
            }
        }

        SearchList {
            id: searchResultList
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            onMoreClicked: {
                userInfo.fetchByName(screenname)
                userTimelineListModel.fetch(screenname)
                timelines.state = 'userinfo'
                userinformation.statusid = statusid
                userinformation.text = text
                userinformation.time = sincetime
            }
        }

        UserInfoScreen {
            id: userinformation
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            //should be exposed in Main.qml
            avatar: userInfo.avatarUrl
            screenname: userInfo.screenName
            name: userInfo.name
            url: userInfo.url
            location: userInfo.location
            description: userInfo.description
            numTweets: userInfo.numTweets
            numFollowers: userInfo.numFollowers
            numFollowing: userInfo.numFollowing
            numFavorites: userInfo.numFavorites
            model: userTimelineListModel

            onHashTagClicked: {
                searchResultList.doSearch(hashtag)
                timelines.state = 'search'
            }
            onMentionClicked: {
                console.log(mention)
                userInfo.fetchByName(mention)
                userTimelineListModel.fetch(mention)
                userinformation.text = ''
            }
            onReplyButtonClicked: {
                tweetUpdate.state = 'show'
                tweetUpdate.setReply(statusid, screenname)
            }
            onRetweetButtonClicked: {
                tweetUpdate.state = 'show'
                tweetUpdate.setRetweet(text, screenname)
            }
            onMessageButtonClicked: {
                tweetUpdate.state = 'show'
                tweetUpdate.tweetid = ''
                tweetUpdate.updateText = ''
                tweetUpdate.screenname = screenname
            }
        }
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
            tweetUpdate.tweetid = "";
            tweetUpdate.screenname = "";
            tweetUpdate.updateText = "";
            tweetUpdate.state = "";
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

            buttonImageUrl: "images/statusupdate.png"
            pressedButtonImageUrl: "images/statusupdate_pressed.png"

            onClicked: {
                // ### TODO Toogle Button
                if (tweetUpdate.state == 'show')
                    tweetUpdate.state = ''
                else
                    tweetUpdate.state = 'show'
            }
        }

        ButtonText {
            id: usernameButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: statusUpdateButton.right; anchors.leftMargin: 10
            width: 145; height: 39

            buttonImageUrl: "images/username.png"
            pressedButtonImageUrl: "images/username_pressed.png"
            text: "@minimoog77" //test
        }

        ButtonImage {
            id: settingsButton

            anchors.top: parent.top; anchors.topMargin: 21
            anchors.left: usernameButton.right; anchors.leftMargin: 10
            width: 39; height: 39

            buttonImageUrl: "images/settings.png"
            pressedButtonImageUrl: "images/settings_pressed.png"
        }
    }

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
            id: homeTimelineButton
            width: 77; height: 77
            anchors.left: parent.left; anchors.leftMargin: 10
            anchors.top: parent.top; anchors.topMargin: 9
            buttonImageUrl:  "images/hometimeline.png"
            pressedButtonImageUrl: "images/hometimeline_pressed.png"
            toggled: true
            showNotification: homeTimelineModel.numNewTweets

            onClicked: {
                homeTimelineModel.showNewTweets();
                timelines.state = "";   //default state
            }
        }

        ButtonWithNotification {
            id: mentionsButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: homeTimelineButton.right; anchors.leftMargin: 11
            buttonImageUrl: "images/replies.png"
            pressedButtonImageUrl: "images/replies_pressed.png"
            showNotification: mentionsModel.numNewTweets

            onClicked: {
                mentionsModel.showNewTweets();
                timelines.state = "mentions";
            }
        }

        ButtonWithNotification {
            id: directMessagesButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: mentionsButton.right; anchors.leftMargin: 11
            buttonImageUrl: "images/directmessages.png"
            pressedButtonImageUrl: "images/directmessages_pressed.png"
            showNotification: directMessagesModel.numNewDirectMessages

            onClicked: {
                dmList.model.showNewTweets();
                timelines.state = "directMessages"
            }
        }

        ButtonWithNotification {
            id: searchButton
            width: 77; height: 77
            anchors.top: parent.top; anchors.topMargin: 9
            anchors.left: directMessagesButton.right; anchors.leftMargin: 11
            buttonImageUrl: "images/search.png"
            pressedButtonImageUrl: "images/search_pressed.png"

            onClicked: {
                timelines.state = "search"
            }
        }
    }

    //need some work

    states: [
        State {
            name: "mentions"
            PropertyChanges { target: rowTimelines; x: - parent.width }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: true }
            PropertyChanges { target: directMessagesButton; toggled: false }
            PropertyChanges { target: searchButton; toggled: false }
        },
        State {
            name: "directMessages"
            PropertyChanges { target: rowTimelines; x: - 2 * parent.width }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: false }
            PropertyChanges { target: directMessagesButton; toggled: true }
            PropertyChanges { target: searchButton; toggled: false }
        },
        State {
            name: "search"
            PropertyChanges { target: rowTimelines; x: - 3 * parent.width }
            PropertyChanges { target: homeTimelineButton; toggled: false }
            PropertyChanges { target: mentionsButton; toggled: false }
            PropertyChanges { target: directMessagesButton; toggled: false }
            PropertyChanges { target: searchButton; toggled: true }
        },
        State {
            name: "userinfo"
            PropertyChanges { target: rowTimelines; x: - 4 * parent.width }
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
