import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: container

    property alias model: dmListView.model

    signal replyClicked(string screenname)

    ListView {
        id: dmListView
        anchors.fill: parent
        clip: true
        delegate: TweetDelegate {
            tweetScreenName: screenNameRole
            tweetText: statusTextRole
            tweetAvatar: avatarUrlRole
            tweetid:  statusIdRole
            isNewTweet: newTweetRole
            tweetSinceTime: sinceTimeRole

            //onReplyButtonClicked: replyClicked(tweetScreenName)
        }
    }
}
