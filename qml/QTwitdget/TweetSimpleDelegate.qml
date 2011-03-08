import QtQuick 1.0

Rectangle {
    id: background

    property string tweetid;
    property string tweetText : "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam ac venenatis ante. Ut euismod tempor erat, eget tincidunt elit ultricies sed."
    property string tweetSinceTime : "Sometimes ago"

    width: 360
    //width: ListView.view.width;
    height: 96
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

    Text {
        id: statusText
        color: "#666666"
        text: tweetText
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        wrapMode: Text.WordWrap
        font.pointSize: 10
        font.family: "Segoe UI"
    }

    Text {
        id: sinceText
        color: "#616161"
        text: tweetSinceTime
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: statusText.bottom
        anchors.topMargin: 7
        font.family: "Segoe UI"
    }
}
