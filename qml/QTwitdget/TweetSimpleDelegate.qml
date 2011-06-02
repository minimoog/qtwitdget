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

Rectangle {
    id: container

    property string tweetid;
    property string tweetText : "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam ac venenatis ante. Ut euismod tempor erat, eget tincidunt elit ultricies sed."
    property string tweetSinceTime : "Sometimes ago"

    signal tweetClicked

    width: ListView.view.width;
    //width: 360
    height: statusText.paintedHeight + 30

    color: (index % 2) ? '#ffffff' : '#d9d9d9'

    Text {
        id: statusText
        text: tweetText
        textFormat: Text.RichText
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        wrapMode: Text.WordWrap
        font.pointSize: 6
        font.family: "Segoe UI"
    }

    Text {
        id: sinceText
        text: tweetSinceTime
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: statusText.bottom
        anchors.topMargin: 5
        font.family: "Segoe UI"
        font.pointSize: 5
    }

    MouseArea {
        id: tweetArea

        anchors.fill: parent

        onClicked: tweetClicked()
    }
}
