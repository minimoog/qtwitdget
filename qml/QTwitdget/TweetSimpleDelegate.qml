import QtQuick 1.0

Rectangle {
    id: background

    property string tweetid;
    property string tweetText //: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam ac venenatis ante. Ut euismod tempor erat, eget tincidunt elit ultricies sed."
    property string tweetSinceTime //: "Sometimes ago"

    signal clicked
//    signal hashtagLinkClicked(string hashtag)
//    signal mentionLinkClicked(string screenname)

//    function handleLink(link) {
//        if (link.slice(0, 3) == 'tag') {
//            hashtagLinkClicked(link.slice(6))
//        } else if (link.slice(0, 4) == 'http') {
//            Qt.openUrlExternally(link);
//        } else if (link.slice(0, 7) == 'mention') {
//            mentionLinkClicked(link.slice(10));
//        }
//    }

    //### TODO: Make it global function
    function addTags(str) {
        //surrounds http links with html link tags
        var ret1 = str.replace(/@[a-zA-Z0-9_]+/g, '<a href="mention://$&">$&</a>');
        var ret2 = ret1.replace(/(\b(https?|ftp):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig, "<a href='$1'>$1</a>");
        var ret3 = ret2.replace(/[#]+[A-Za-z0-9-_]+/g, '<a href="tag://$&">$&</a>')
        return ret3;
    }

    width: ListView.view.width;
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
        color: background.ListView.isCurrentItem ? "red" : "#666666"
        text: addTags(tweetText)
        textFormat: Text.RichText
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        wrapMode: Text.WordWrap
        font.pointSize: 10
        font.family: "Segoe UI"

//        onLinkActivated: {
//            background.handleLink(link)
//            console.log('link clicked')
//        }
    }

    Text {
        id: sinceText
        color: background.ListView.isCurrentItem ? "red" :"#616161"
        text: tweetSinceTime
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: statusText.bottom
        anchors.topMargin: 7
        font.family: "Segoe UI"
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        onClicked: {
            background.ListView.view.currentIndex = index;
            background.clicked();
        }
    }
}
