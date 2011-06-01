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

Rectangle {
    id: container

    property alias model: tweetListView.model

    signal tweetClicked(string tweetid, string tweettext, string screenname)
    signal avatarClicked(string screenname)

    //signal reply(string id, string screenname, string tweettext)
    //signal retweet(string tweetid)
    //signal conversation(string id)
    //signal userinformation(string screenname)
    //signal deleteClicked(string id)

    //since time is not updated

    //right arrow pressed or mention clicked
   //signal hashtagClicked(string hashtag)

     color: "#ffffff"

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
            isOwnTweet: ownTweetRole
            isNewTweet: newTweetRole
            tweetSinceTime: sinceTimeRole

            onTweetClicked: container.tweetClicked(tweetid, tweetText, tweetScreenName)
            onAvatarClicked: container.avatarClicked(tweetScreenName)

            //onReplyClicked: reply(tweetid, tweetScreenName, tweetText)
            //onRetweetClicked: retweet(tweetid)
            //onConversationClicked: conversation(tweetid)
            //onMentionLinkClicked: userinformation(screenname)
            //onHashtagLinkClicked: hashtagClicked(hashtag)
        }
    }

    Rectangle {
        id: scrollbar
        anchors.right: tweetListView.right
        y: tweetListView.visibleArea.yPosition * tweetListView.height
        width: 2
        height: tweetListView.visibleArea.heightRatio * tweetListView.height
        color: "black"
    }
}
