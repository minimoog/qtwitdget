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

Rectangle {
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

    gradient: Gradient {
        GradientStop { position: 0; color: '#d9d9d9' }
        GradientStop { position: 1; color: '#ffffff' }
    }

    VisualItemModel {
        id: itemModel

        Rectangle {
            id: textBackground
            height: 4 + tweettext.paintedHeight + 4
            width: view.width

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

        Row {
            id: rowMainButtons
            x: 6
            height: 40
            width: view.width
            spacing: 5

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
            width: view.width
            height: {
                var numHorizontalCells = Math.floor(width / cellWidth);
                var numVerticallCells = Math.ceil(entityModel.count / numHorizontalCells);

                return numVerticallCells * cellHeight;
            }
            cellWidth: 132
            cellHeight: 44
            delegate: EntityDelegate {
                entityText: display
                onClicked: handleEntity(entityText)
            }
            interactive: false

            model: TweetEntityListModel {
                id: entityModel
                tweetid: container.tweetid
            }
        }
    }

    ListView {
        id: view
        anchors.fill: parent
        model: itemModel
    }
}
