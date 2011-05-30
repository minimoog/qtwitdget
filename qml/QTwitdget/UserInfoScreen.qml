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
    property alias model : tweetSimpleList.model

    id: container

    property string avatar : "images/avatar.png"
    property string screenname
    property string name
    property string url
    property string location
    property string description
    property bool isFriend

    signal message
    signal follow
    signal unfollow
    signal tweetClicked(string tweetid, string tweettext, string screenname)

    Rectangle {
        id: userInfoBackground
        //97
        height: (userDescription.paintedHeight < 70) ? 97 : userDescription.paintedHeight + 35
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
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

        Image {
            id: userAvatar
            anchors.top: parent.top; anchors.topMargin: 4
            anchors.left: parent.left; anchors.leftMargin: 4
            source: container.avatar
        }
        Text {
            id: userDescription
            text: container.screenname + '<br \/> ' + container.name + '<br \/> ' + container.url + '<br \/> ' + container.location + '<br \/> ' + container.description
            anchors.right: parent.right
            wrapMode: Text.WordWrap
            anchors.topMargin: 4
            anchors.rightMargin: 4
            font.pointSize: 6
            anchors.left: userAvatar.right; anchors.leftMargin: 4
            anchors.top: parent.top
            font.family: "Segoe UI"
        }

        ButtonImage {
            id: unfollowButton
            width: 84; height: 23
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.left: parent.left
            anchors.leftMargin: 4
            smooth: true
            buttonImageUrl: container.isFriend ? 'images/unfollow_button.png' : 'images/follow_button.png'
            pressedButtonImageUrl: container.isFriend ? 'images/unfollow_button_pressed.png' : 'images/follow_button_pressed.png'

            onClicked: {
                if (container.isFriend)
                    unfollow()
                else
                    follow()
            }
        }
        ButtonImage {
            id: blockButton
            width: 84; height: 23
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.left: unfollowButton.right
            anchors.leftMargin: 4
            buttonImageUrl: "images/block_button.png"
            pressedButtonImageUrl: "images/block_button_pressed.png"
        }

        ButtonImage {
            id: messageButton
            width: 84
            height: 23
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.left: blockButton.right
            anchors.leftMargin: 4
            pressedButtonImageUrl: "images/message_button_pressed.png"
            buttonImageUrl: "images/message_button.png"

            onClicked: message()
        }
    }

    TweetSimpleList {
        id: tweetSimpleList
        anchors.top: userInfoBackground.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        onTweetClicked: container.tweetClicked(tweetid, tweettext, container.screenname)
    }
}
