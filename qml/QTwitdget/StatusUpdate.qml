import QtQuick 1.0

Rectangle {
    id: rectangle1

    property string tweetid
    property string screenname  //non empty if it's direct message
    property alias updateText: statusTextInput.text

    signal sendButtonClicked

    function setReply(id, name, text) {
        tweetid = id;
        statusTextInput.text = '@' + name + ' ';
        tweetReplyText.text = '@' + name + ': ' + text;
        screenname = '';
    }

    function setRetweet(text, name) {
        statusTextInput.text = 'RT @' + name + ': ' + text;
        screenname = '';
    }

    width: 360
    height:184 + tweetReplyText.paintedHeight
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
            font.pointSize: 10
            font.family: "Segoe UI"
            wrapMode: TextEdit.WordWrap
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8
            anchors.fill: parent
            selectByMouse: true
            textFormat: TextEdit.PlainText
        }
    }

    Text {
        id: tweetReplyText
        color: "#a0a0a0"
        font.family: "Segoe UI"
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
        width: 56; height: 56
        anchors.top: tweetReplyText.bottom; anchors.topMargin: 10
        anchors.left: parent.left; anchors.leftMargin: 20
        buttonImageUrl: "images/send_button.png"
        pressedButtonImageUrl: "images/send_button_pressed.png"

        onClicked: sendButtonClicked()
    }

    ButtonImage {
        id: shortUrlButton
        width: 56; height: 56
        anchors.top: tweetReplyText.bottom; anchors.topMargin: 10
        anchors.left: sendButton.right; anchors.leftMargin: 9
        buttonImageUrl: "images/shorturl.png"
        pressedButtonImageUrl: "images/shorturl_pressed.png"
    }

    Text {
        id: counter
        text: 140 - statusTextInput.text.length
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        font.family: "Segoe UI"
        font.pixelSize: 12
    }
}
