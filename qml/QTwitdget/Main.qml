/* Copyright (c) 2011, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

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

