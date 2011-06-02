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

import QtQuick 1.0

Item {
    id: container

    property alias model: searchListView.model

    signal tweetClicked(string tweetid, string tweettext, string screenname)
    signal avatarClicked(string screenname)

    //signal searchClicked

    function doSearch(query) {
        queryInput.text = query
        model.startSearch(query)
    }

    Rectangle {
        id: background
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#ffffff"
            }
            GradientStop {
                position: 1
                color: "#D1D1D1"
            }
        }
        anchors.fill: parent
    }

    BorderImage {
        id: queryInputBackground
        height: 33
        anchors.right: parent.right
        anchors.rightMargin: 54
        anchors.left: parent.left
        anchors.leftMargin: 53
        anchors.top: parent.top
        anchors.topMargin: 21
        border.bottom: 5
        border.top: 5
        border.right: 5
        border.left: 5
        source: "images/status_update_background.png"

        TextInput {
            id: queryInput
            color: "#b0b0b0"
            text: ""
            font.pointSize: 6
            cursorVisible: true
            font.family: "Segoe UI"
            anchors.bottomMargin: 6
            anchors.leftMargin: 6
            anchors.rightMargin: 6
            anchors.topMargin: 6
            anchors.fill: parent
            focus: true

            onAccepted: model.startSearch(queryInput.text)
        }
    }

    ListView {
        id: searchListView
        anchors.topMargin: 16
        //anchors.fill: parent
        anchors.top: queryInputBackground.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        delegate: TweetDelegate {
            tweetSinceTime: sinceTimeRole
            tweetScreenName: screenNameRole
            tweetAvatar: avatarUrlRole
            tweetText: statusTextRole
            tweetid: statusIdRole

            onTweetClicked: container.tweetClicked(tweetid, tweetText, tweetScreenName)
            onAvatarClicked: container.avatarClicked(tweetScreenName)
        }
    }

    Rectangle {
        id: scrollbar
        anchors.right: searchListView.right
        y: searchListView.visibleArea.yPosition * searchListView.height
        width: 2
        height: searchListView.visibleArea.heightRatio * searchListView.height
        color: "black"
    }
}
