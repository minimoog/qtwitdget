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

import QtQuick 1.1

Rectangle  {
    id: container

    property string tweetid
    property string tweetScreenName
    property string tweetText
    property string tweetAvatar
    property string tweetSinceTime

    signal avatarClicked
    signal tweetClicked

    width: ListView.view.width;
    //width: 360;
    //height: (statusText.paintedHeight < 87) ? 97 : (statusText.paintedHeight + 3)
    height: 90
    color: '#d9d9d9'

    Rectangle {
        id: avatarBackground
        width: 52
        height: 52
        color:  '#2bace2'
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8

        Image {
            id: avatarImage
            width: 48
            height: 48
            sourceSize.width: 48
            sourceSize.height: 48
            anchors.centerIn: parent
            fillMode: Image.Stretch
            source: tweetAvatar
        }
    }

    Text {
        id: statusText
        color: "#333333"
        text: '<b>' + tweetScreenName + ':<\/b><br \/> ' + tweetText
        anchors.top: parent.top;
        anchors.topMargin: 4
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: avatarBackground.right; anchors.leftMargin: 8
        textFormat: Text.RichText
        wrapMode: "WordWrap"
        font.family: "Nokia Pure"
        font.pixelSize: 18
    }

    Text {
        id: sinceText
        text: tweetSinceTime
        anchors.top: avatarBackground.bottom
        anchors.leftMargin: 8
        anchors.topMargin: 8
        anchors.left: parent.left
        font.family: "Nokia Pure"
        font.pixelSize: 14
    }

    MouseArea {
        id: tweetMouseArea
        anchors.left: avatarMouseArea.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.leftMargin: 0

        onClicked: tweetClicked()
    }

    MouseArea {
        id: avatarMouseArea
        width: 100
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: avatarBackground.right

        onClicked: avatarClicked()
    }
}
