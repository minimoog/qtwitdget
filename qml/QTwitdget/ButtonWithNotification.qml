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

Item  {
    id: btnNotification

    property string buttonImageUrl
    property string pressedButtonImageUrl
    property bool toggled: false
    property bool showNotification: false

    signal clicked

    Image {
        id: buttonImage
        source: buttonImageUrl
        smooth: true
        anchors.fill: parent

        Image {
            id: notificationMarkerImage
            opacity: 0
            anchors.top: parent.top
            anchors.right: parent.right
            source:  "images/red_marker.png"

            states: [
                State {
                    name: "showMarker"
                    when: showNotification
                    PropertyChanges { target: notificationMarkerImage; opacity: 1 }
                }
            ]

            transitions: [
                Transition {
                    NumberAnimation { property: "opacity"; duration: 500 }
                }
            ]
        }
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent

        onClicked: {
            btnNotification.clicked();
            if (!toggled)
                toggled = true;   //stuckiness, not toggling button
        }
    }

    states: [
        State {
            name: "toggled_on"
            when: toggled
            PropertyChanges { target: buttonImage; source: pressedButtonImageUrl }
        }
    ]
}
