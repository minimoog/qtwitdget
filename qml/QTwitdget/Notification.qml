import QtQuick 2.0

Rectangle {
    id: notificationArea

    function show(text) {
        if (showTimer.running) {
            showTimer.restart()
        }
        else {
            showTimer.start()
            notificationArea.y -= notificationArea.height
        }

        notificationText.text = text
    }

    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#dcdcdc"
        }

        GradientStop {
            position: 1
            color: "#b4b4b4"
        }
    }

    Behavior on y { NumberAnimation { duration: 200 } }

    Text {
        id: notificationText
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        anchors.fill: parent
        font.family: "Segoe UI"
    }

    Timer {
        id: showTimer
        interval: 5000

        onTriggered: {
            notificationArea.y += notificationArea.height
            console.log(notificationArea.y)
        }
    }
}
