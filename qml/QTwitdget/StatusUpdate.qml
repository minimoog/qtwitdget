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
    id: rectangle1

    property string tweetid
    property string screenname  //non empty if it's direct message
    property alias updateText: statusTextInput.text

    signal sendButtonClicked

    function setReply(id, name, text) {
        tweetid = id
        statusTextInput.text = '@' + name + ' '
        tweetReplyText.text = '@' + name + ': ' + text
        screenname = ''
    }

    function setRetweet(text, name) {
        statusTextInput.text = 'RT @' + name + ': ' + text
        tweetReplyText.text = ''
        screenname = ''
    }

    function setDirectMessage(name) {
        statusTextInput.text = ''
        tweetReplyText.text = 'Sending DM to @' + name
        screenname = name
    }

    function clear() {
        statusTextInput.text = ''
        tweetReplyText.text = ''
        screenname = ''
    }

    width: 360
    height:170 + tweetReplyText.paintedHeight
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

    BorderImage {
        id: statusTextBackground
        anchors.right: parent.right; anchors.rightMargin: 20
        anchors.left: parent.left; anchors.leftMargin: 20
        anchors.top: parent.top; anchors.topMargin: 20
        border.bottom: 5
        border.top: 5
        border.right: 5
        border.left: 5
        source: "images/status_update_background.png"

        TextEdit {
            id: statusTextInput
            color: "#919191"
            font.pointSize: 6
            font.family: "Segoe UI"
            wrapMode: TextEdit.WordWrap
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8
            anchors.fill: parent
            selectByMouse: true
            textFormat: TextEdit.PlainText

            ShortUrlsComponent {
                id: shortUrlsText

                onShortedText: {
                    statusTextInput.text = text
                }
            }
        }
    }

    Text {
        id: tweetReplyText
        color: "#a0a0a0"
        font.family: "Segoe UI"
        font.pointSize: 6
        wrapMode: Text.WordWrap
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: statusTextBackground.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
    }

    ButtonImage {
        id: sendButton
        width: 77; height: 41
        anchors.top: tweetReplyText.bottom; anchors.topMargin: 10
        anchors.left: parent.left; anchors.leftMargin: 20
        buttonImageUrl: "images/send_button.png"
        pressedButtonImageUrl: "images/send_button_pressed.png"

        onClicked: sendButtonClicked()
    }

    ButtonImage {
        id: shortUrlButton
        width: 78; height: 41
        anchors.top: tweetReplyText.bottom; anchors.topMargin: 10
        anchors.left: sendButton.right; anchors.leftMargin: 9
        buttonImageUrl: "images/shorturl.png"
        pressedButtonImageUrl: "images/shorturl_pressed.png"

        onClicked: {
            shortUrlsText.shortUrls(statusTextInput.text)
        }
    }

    Text {
        id: counter
        text: 140 - statusTextInput.text.length
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        font.family: "Segoe UI"
        font.pointSize: 6
    }
}
