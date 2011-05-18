//hometimelineListModel - external
//mentionsListModel - external
//directMessagesListModel - external
//searchListModel - external
//rootWindow - external
//userInfo - external
//userTimelineListModel - external
//conversationListModel - external

import QtQuick 1.0

Item {
    width: 360
    height: 640

    id: mainview

    AuthScreen {
        id: authorizepage
        anchors.fill: parent
        opacity: 0

        onLoginClicked: {
            //if user clicks Login at the moment of auth, there are problems
            //### TODO disable somehow login button when doing auth process
            rootWindow.authorize(username, password);
        }
    }

    TimelinesScreen {
        id: timelines
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width

        homeTimelineModel: hometimelineListModel
        mentionsModel: mentionsListModel
        directMessagesModel: directMessagesListModel
        searchModel: searchListModel
        conversationModel: conversationListModel
        username: rootWindow.userScreenName

        onSendClicked: rootWindow.updateButtonClicked(tweetid, text, screenname)
        onRetweet: rootWindow.retweet(tweetid)
    }

    states: [
        State {
            name: "authscreen"
            when: !rootWindow.authed
            PropertyChanges { target: timelines; x: mainview.width }
            PropertyChanges { target: timelines; opacity: 0 }
            PropertyChanges { target: authorizepage; opacity: 1 }
        },

        State {
            name: "timelinesscreen"
            when:  rootWindow.authed
            StateChangeScript { script: authorizepage.clear() }
            PropertyChanges { target: timelines; x: 0 }
            PropertyChanges { target: timelines; opacity: 1 }
            PropertyChanges { target: authorizepage; opacity: 0 }
        }
    ]

    transitions: [
        Transition {
            PropertyAnimation { properties: "x"; duration: 250; target: timelines }
        }
    ]
}

