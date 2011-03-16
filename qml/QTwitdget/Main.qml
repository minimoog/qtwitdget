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

    property bool authed: true

    id: mainview
    objectName: "mainview"

    AuthScreen {
        id: authorizepage
        anchors.fill: parent
        opacity: 0

        onLoginClicked: {
            authed = true;
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
    }

    states: [
        State {
            name: "authscreen"
            when: !mainview.authed
            PropertyChanges { target: timelines; x: mainview.width }
            PropertyChanges { target: timelines; opacity: 0 }
            PropertyChanges { target: authorizepage; opacity: 1 }
        },

        State {
            name: "timelinesscreen"
            when:  mainview.authed
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

