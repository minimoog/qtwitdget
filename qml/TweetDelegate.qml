import QtQuick 1.0

Rectangle  {
    id: container

    property string tweetid
    property string tweetScreenName: "Screen Name"
    property string tweetText: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam ac venenatis ante. Ut euismod tempor erat, eget tincidunt elit ultricies sed."
    property string tweetAvatar
    property string tweetSinceTime: "sometimes ago"
    property bool isOwnTweet: false
    property bool isNewTweet: false

    width: ListView.view.width;
    //width: 360;
    height: (statusText.paintedHeight < 96) ? 96 : statusText.paintedHeight
    //height: 116;

    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#FEFEFE"
        }
        GradientStop {
            position: 1
            color: "#DBDCDE"
        }
    }

    function handleLink(link) {
        if (link.slice(0, 3) == 'tag') {
            console.log('tag clicked');
        } else if (link.slice(0, 4) == 'http') {
            Qt.openUrlExternally(link);
        } else if (link.slice(0, 7) == 'mention') {
            console.log('mention clicked');
        }
    }

//    function addTags(str) {
//        //surrounds http links with html link tags
//        var ret1 = str.replace(/@[a-zA-Z0-9_]+/g, '<a href="mention://$&">$&</a>');
//        var ret2 = ret1.replace(/(\b(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig, "<a href='$1'>$1</a>");
//        var ret3 = ret2.replace(/[#]+[A-Za-z0-9-_]+/g, '<a href="tag://$&">$&</a>')
//        return ret3;
//    }

    Rectangle {
        id: avatarContainer
        width: 61
        height: 61
        color: "#b3b3b3"
        radius: 6
        border.width: 2
        anchors.left: parent.left; anchors.leftMargin: 11
        anchors.top: parent.top; anchors.topMargin: 11
        border.color: "#717171"

        Image {
            id: avatarImage

            width: 48; height: 48
            anchors.centerIn: parent
            fillMode: Image.Stretch
            source: tweetAvatar
        }
    }

    Text {
        id: statusText
        color: "#333333"
        //text: addTags(tweetText)
        text: '<b>' + tweetScreenName + ':<\/b><br \/> ' + tweetText
        anchors.topMargin: 11
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: avatarContainer.right; anchors.leftMargin: 10
        textFormat: Text.RichText
        wrapMode: "WordWrap"
        font.family: "Segoe UI"
        font.pointSize: 10

        onLinkActivated: container.handleLink(link)
    }

    Text {
        id: sinceText
        text: tweetSinceTime
        anchors.left: avatarContainer.left
        anchors.top: avatarContainer.bottom
        font.family: "Segoe UI"
        font.pointSize: 7
    }

//    ListView.onAdd: ParallelAnimation {
//        NumberAnimation { target: container; property: "scale";from: 0; to: 1; duration: 500 }
//        NumberAnimation { target: container; property: "height"; from: 0; to: 80; duration: 500 }
//    }
}
