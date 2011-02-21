import QtQuick 1.0

Item {

    property alias model: searchListView.model

    //signal searchClicked

    //add search box
    //width: 360
    //height: 462

    TextInput {
        id: queryInput

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20
        focus: true

        onAccepted: {
            console.log('Oooh accepted');
            model.startSearch(queryInput.text);
        }
    }

    ListView {
        id: searchListView
        //anchors.fill: parent
        anchors.top: queryInput.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        clip: true
        delegate: TweetDelegate {
            tweetSinceTime: sinceTimeRole
            tweetScreenName: screenNameRole
            tweetAvatar: avatarUrlRole
            tweetText: statusTextRole
        }
    }

    Rectangle {
        id: scrollbar
        anchors.right: searchListView.right
        y: searchListView.visibleArea.yPosition * searchListView.height
        width: 2
        height: searchListView.visibleArea.heightRatio * searchListView.height
        color: "black"
    }
}
