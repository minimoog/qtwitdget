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

//    signal replyClicked
//    signal retweetClicked
//    signal favoriteClicked
//    signal conversationClicked
//    signal mentionLinkClicked(string screenname)
//    signal hashtagLinkClicked(string hashtag)

//    function handleLink(link) {
//        if (link.slice(0, 3) == 'tag') {
//            hashtagLinkClicked(link.slice(6))
//        } else if (link.slice(0, 4) == 'http') {
//            Qt.openUrlExternally(link);
//        } else if (link.slice(0, 7) == 'mention') {
//            mentionLinkClicked(link.slice(10));
//        }
//    }

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

//        onLinkActivated: container.handleLink(link)
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

    //    Image {
    //        id: rightArrow
    //        source: "images/right_arrow.png"
    //        width: 11; height: 16
    //        anchors.right: parent.right
    //        anchors.rightMargin: 4
    //        anchors.verticalCenter: parent.verticalCenter
    //    }

    //    MouseArea {
    //        id: moreMouseArea
    //        anchors.left: rightArrow.right
    //        anchors.leftMargin: -20
    //        anchors.bottom: buttonLoader.top
    //        anchors.bottomMargin: 0
    //        anchors.top: parent.top
    //        anchors.topMargin: 0
    //        anchors.right: parent.right
    //        anchors.rightMargin: 0

    //        onClicked: {
    //            if (container.state == 'showButtons')
    //                container.state = ''
    //            else
    //                container.state = 'showButtons'
    //        }
    //    }

//    Loader {
//        id: buttonLoader
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//    }

//    Connections { target: buttonLoader.item; onReplyButtonClicked: replyClicked() }
//    Connections { target: buttonLoader.item; onRetweetButtonClicked: retweetClicked() }
//    Connections { target: buttonLoader.item; onFavoriteButtonClicked: favoriteClicked() }
//    Connections { target: buttonLoader.item; onConversationButtonClicked: conversationClicked() }

//    Component {
//        id: buttonRow

//        Item {
//            id: buttonContainer

//            signal replyButtonClicked()
//            signal retweetButtonClicked()
//            signal favoriteButtonClicked()
//            signal conversationButtonClicked()

//            ButtonImage {
//                id: replyButton
//                width: 84
//                height: 23
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.left: parent.left
//                anchors.leftMargin: 5
//                pressedButtonImageUrl: "images/reply_button_pressed.png"
//                buttonImageUrl: "images/reply_button.png"

//                onClicked: replyButtonClicked()
//            }

//            ButtonImage {
//                id: retweetButton
//                width: 84
//                height: 23
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.left: replyButton.right
//                anchors.leftMargin: 5
//                pressedButtonImageUrl: "images/retweet_button_pressed.png"
//                buttonImageUrl: "images/retweet_button.png"

//                onClicked: retweetButtonClicked()
//            }

//            ButtonImage {
//                id: favouriteButton
//                width: 84
//                height: 23
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.left: retweetButton.right
//                anchors.leftMargin: 5
//                pressedButtonImageUrl: "images/favourite_button_pressed.png"
//                buttonImageUrl: "images/favourite_button.png"

//                onClicked: favoriteButtonClicked()
//            }

//            ButtonImage {
//                id: conversationButton
//                width: 84
//                height: 23
//                anchors.bottom: parent.bottom
//                anchors.bottomMargin: 4
//                anchors.left: favouriteButton.right
//                anchors.leftMargin: 5
//                pressedButtonImageUrl: "images/conversation_button_pressed.png"
//                buttonImageUrl: "images/conversation_button.png"

//                onClicked: conversationButtonClicked()
//            }
//        }
//    }

//    states: State {
//        name: 'showButtons'
//        PropertyChanges { target: buttonLoader; sourceComponent: buttonRow }
//        PropertyChanges { target: container; height: container.height + 23; explicit: true }
//        PropertyChanges { target: rightArrow; rotation: 90 }
//    }

//    transitions: Transition {
//            NumberAnimation { target: container; property: "height"; duration: 200 }
//            NumberAnimation { target: rightArrow; property: "rotation"; duration: 200 }
//    }
}
