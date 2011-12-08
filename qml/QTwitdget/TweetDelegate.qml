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

Rectangle  {
    id: container

    property string tweetid
    property string tweetScreenName
    property string tweetText
    property string tweetAvatar
    property string tweetSinceTime
    property bool isOwnTweet: false
    property bool isNewTweet: false

    signal avatarClicked
    signal tweetClicked

    width: ListView.view.width;
    //width: 360;
    height: (statusText.paintedHeight < 87) ? 97 : (statusText.paintedHeight + 3)
    color: {
        if (isNewTweet)
            return (index % 2) ? '#ffd954' : '#ffb300'
        else
            return (index % 2) ? '#ffffff' : '#d9d9d9'
    }

    Behavior on color { ColorAnimation { easing.type: Easing.OutExpo; duration: 500 } }

    Rectangle {
        id: avatarBackground
        width: 52
        height: 52
        color:  '#2bace2'
        anchors.top: parent.top
        anchors.topMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4

        Image {
            id: avatarImage
            width: 48
            height: 48
            sourceSize.width: 48
            sourceSize.height: 48
            anchors.centerIn: parent
            fillMode: Image.Stretch
        }
    }

    Text {
        id: statusText
        color: "#333333"
        text: '<b>' + tweetScreenName + ':<\/b><br \/> ' + tweetText
        anchors.topMargin: 0
        anchors.top: parent.top;
        anchors.right: parent.right
        anchors.left: avatarBackground.right; anchors.leftMargin: 4
        textFormat: Text.RichText
        wrapMode: "WordWrap"
        font.family: "Segoe UI"
        font.pointSize: 6
    }

    Text {
        id: sinceText
        text: tweetSinceTime
        anchors.top: avatarBackground.bottom
        anchors.leftMargin: 10
        anchors.topMargin: 4
        anchors.left: parent.left
        font.family: "Segoe UI"
        font.pointSize: 5
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

    ListView.onAdd: SequentialAnimation {
        ParallelAnimation {
            PropertyAction { target: container; property: "height"; value:  0 }
            PropertyAction { target: container; property: "x"; value: container.width }
            PropertyAction { target: container; property: "opacity"; value: 0 }
        }
        NumberAnimation { target: container; property: "height"; easing.type: Easing.OutExpo; from: 0; to: (statusText.paintedHeight < 87) ? 95 : (statusText.paintedHeight + 4); duration: 500 }
        PropertyAction { target: container; property: "opacity"; value: 1 }
        PauseAnimation { duration: index * 20 }
        NumberAnimation { target: container; property: "x"; easing.type: Easing.OutBack; to: 0; duration: 500 }
        PropertyAction { target: avatarImage; property: "source"; value: tweetAvatar }
    }

    ListView.onRemove: SequentialAnimation {
        PropertyAction { target: container; property: "ListView.delayRemove"; value: true }
        NumberAnimation { target: container; property: "opacity"; to: 0; duration: 500 }
        NumberAnimation { target: container; property: "height"; to: 0; duration: 500 }
        PropertyAction { target: container; property: "ListView.delayRemove"; value: false }
    }
}
