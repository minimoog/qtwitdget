import Qt 4.7

Rectangle {
    height: updateTweetInput.height + rowButtons.height + 5
    radius: 0
    border.width: 2
    border.color: "#000000"

    TextEdit {
        id: updateTweetInput
        width: parent.width
        height: 70
        cursorVisible: true
        font.pointSize: 10
        font.family: "Segoe UI"
        smooth: true
        selectByMouse: true
        wrapMode: TextEdit.WordWrap
        textFormat: TextEdit.PlainText
    }

    Row {
        id: rowButtons
        y: updateTweetInput.height + 5
        spacing: 5

        TestButton {
            width: 60
            height: 20
            text: "Update"
        }

        TestButton {
            width: 70
            height: 20
            text: "Short urls"
        }
    }
}
