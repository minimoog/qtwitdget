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

//hometimelineListModel - external
//mentionsListModel - external
//directMessagesListModel - external
//searchListModel - external
//rootWindow - external
//userInfo - external
//userTimelineListModel - external
//conversationListModel - external

import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: authPage

    Connections {
        target: rootWindow
        onAuthedChanged: {
            if (rootWindow.authed)
                pageStack.replace(homePage)
        }
    }

    AuthPage {
        id: authPage
    }

    HomePage {
        id: homePage
        model: hometimelineListModel

        tools: ToolBarLayout {
            ToolIcon { iconId: "toolbar-refresh1"; onClicked: { hometimelineListModel.showNewTweets() } }
            ToolIcon { iconId: "toolbar-send-email"; onClicked: { pageStack.push(mentionsPage) } }
            ToolIcon { iconId: "toolbar-new-message"; onClicked: { pageStack.push(directMessagesPage) } }
            ToolIcon { iconId: "toolbar-search"; onClicked: { pageStack.push(searchPage) } }
            ToolIcon { iconId: "toolbar-view-menu"; onClicked: console.debug('crap') }
        }
    }

    MentionPage {
        id: mentionsPage
        model: mentionsListModel

        tools: ToolBarLayout {
            ToolIcon { iconId: "toolbar-back"; onClicked: { pageStack.pop(); } }
            ToolIcon { iconId: "toolbar-refresh1"; onClicked: { mentionsListModel.showNewTweets() } }
            ToolIcon { iconId: "toolbar-view-menu"; onClicked: console.debug('crap') }
        }
    }

    DirectMessagesPage {
        id: directMessagesPage
        model: directMessagesListModel

        tools: ToolBarLayout {
            ToolIcon { iconId: "toolbar-back"; onClicked: { pageStack.pop(); } }
            ToolIcon { iconId: "toolbar-refresh1"; onClicked: { directMessagesListModel.showNewTweets() } }
            ToolIcon { iconId: "toolbar-view-menu"; onClicked: console.debug('crap') }
        }
    }

    SearchPage {
        id: searchPage
        model: searchListModel

        tools: ToolBarLayout {
            ToolIcon { iconId: "toolbar-back"; onClicked: { pageStack.pop(); } }
            ToolIcon { iconId: "toolbar-view-menu"; onClicked: console.debug('crap') }
        }
    }
}
