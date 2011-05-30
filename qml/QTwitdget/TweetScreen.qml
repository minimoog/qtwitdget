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
import qtwitdget.components 1.0

Item {
    id: container

    property string text
    property string tweetid
    property string screenname

    signal replyClicked(string tweetid, string screenname, string tweettext)
    signal retweetClicked(string tweetid)
    signal conversationClicked(string tweetid)
    signal entityMentionClicked(string screenname)
    signal entityHashtagClicked(string hashtag)

    function handleEntity(entity) {
        if (entity.slice(0, 1) == '@')
            entityMentionClicked(entity.slice(1));
        else if (entity.slice(0, 1) == '#')
            entityHashtagClicked(entity);
        else if (entity.slice(0, 7) == 'http://')
            Qt.openUrlExternally(entity);
    }

    Rectangle {
        id: textBackground
        height: 4 + tweettext.paintedHeight + 4
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#A8E3FF"
            }
            GradientStop {
                position: 0.7
                color: "#DBF3FF"
            }
        }
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        Text {
            id: tweettext
            text: container.text
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignTop
            anchors.top: parent.top; anchors.topMargin: 4
            anchors.left: parent.left; anchors.leftMargin: 4
            anchors.right: parent.right; anchors.rightMargin: 4
            font.pointSize: 8
        }
    }

    Rectangle {
        id: background
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#d9d9d9"
            }

            GradientStop {
                position: 1
                color: "#ffffff"
            }
        }
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: textBackground.bottom

        Row {
            id: rowMainButtons
            x: 6
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5
            //height: 200
            anchors.top: parent.top

            ButtonImage {
                id: replyButton
                width: 76; height: 40
                sourceSizeWidth: 76; sourceSizeHeight: 40
                pressedButtonImageUrl: "images/reply_button_pressed.png"
                buttonImageUrl: "images/reply_button.png"

                onClicked: replyClicked(container.tweetid, container.screenname, container.text)
            }

            ButtonImage {
                id: retweetButton
                width: 76; height: 40
                sourceSizeWidth: 76; sourceSizeHeight: 40
                pressedButtonImageUrl: "images/retweet_button_pressed.png"
                buttonImageUrl: "images/retweet_button.png"

                onClicked: retweetClicked(container.tweetid)
            }

            ButtonImage {
                id: favouriteButton
                width: 76; height: 40
                sourceSizeWidth: 76; sourceSizeHeight: 40
                pressedButtonImageUrl: "images/favourite_button_pressed.png"
                buttonImageUrl: "images/favourite_button.png"
                //onClicked: favoriteButtonClicked()
            }

            ButtonImage {
                id: conversationButton
                width: 76; height: 40
                sourceSizeWidth: 76; sourceSizeHeight: 40
                pressedButtonImageUrl: "images/conversation_button_pressed.png"
                buttonImageUrl: "images/conversation_button.png"
                onClicked: conversationClicked(container.tweetid)
            }
        }

        GridView {
            id: gridView
            width: {
                var numCells = Math.floor(parent.width / cellWidth);
                return numCells * cellWidth;
            }

            anchors.topMargin: 4
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: rowMainButtons.bottom
            cellWidth: 132
            cellHeight: 44
            delegate: EntityDelegate {
                entityText: display
                onClicked: handleEntity(entityText)
            }

            model: TweetEntityListModel {
                id: entityModel
                tweetid: container.tweetid
            }
        }
    }
}
