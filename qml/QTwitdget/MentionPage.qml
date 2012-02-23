import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: container

    property alias model: tweetListView.model

    signal tweetClicked(string tweetid, string tweettext, string screenname)
    signal avatarClicked(string screenname)

    //since time is not updated

    ListView {
        id: tweetListView
        anchors.fill: parent
        clip: true
        cacheBuffer: 120 * 20
        delegate: TweetDelegate {
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
            tweetAvatar: avatarUrlRole
            tweetid:  statusIdRole
            tweetSinceTime: sinceTimeRole

            onTweetClicked: container.tweetClicked(tweetid, tweetText, tweetScreenName)
            onAvatarClicked: container.avatarClicked(tweetScreenName)
        }
    }
}
