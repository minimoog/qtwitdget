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

// userInfo external - make it register type

import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    //property alias model : tweetSimpleList.model

    id: container

    signal message
    signal follow
    signal unfollow
    signal tweetClicked(string tweetid, string tweettext, string screenname)

    Rectangle {
        id: userInfoBackground
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
            anchors.top: parent.top; anchors.topMargin: 16
            anchors.left: parent.left; anchors.leftMargin: 16
            source: userInfo.avatarUrl
        }
        Text {
            id: userDescription
            text: userInfo.screenName + '<br \/> ' + userInfo.name + '<br \/> ' + userInfo.url + '<br \/> ' + userInfo.location + '<br \/> ' + userInfo.description
            font.pixelSize: 18
            anchors.right: parent.right
            wrapMode: Text.WordWrap
            anchors.topMargin: 16
            anchors.rightMargin: 16
            anchors.left: userAvatar.right; anchors.leftMargin: 16
            anchors.top: parent.top
        }
    }

    ButtonRow {
        id: userActions
        exclusive: false
        anchors.top: userInfoBackground.bottom
        anchors.topMargin: 16
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16

        Button {
            text: userInfo.isFriend ? 'Unfollow' : 'Follow'

            onClicked: {
                if (userInfo.isFriend)
                    unfollow()
                else
                    follow()
            }
        }

        Button { text: 'Block' }

        Button {
            text: 'Message'
            onClicked: message()
        }
    }

//    TweetSimpleList {
//        id: tweetSimpleList
//        anchors.top: userActions.top
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left

//        onTweetClicked: container.tweetClicked(tweetid, tweettext, userInfo.screenName)
//    }

    tools: ToolBarLayout {
        ToolIcon { iconId: "toolbar-back"; onClicked: { pageStack.pop() } }
    }
}
