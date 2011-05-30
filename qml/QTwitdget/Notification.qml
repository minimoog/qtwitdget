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
