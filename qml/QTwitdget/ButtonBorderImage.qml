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

Item {
    id: buttonImage

    property string buttonImageUrl
    property string pressedButtonImageUrl
    property int borderBottom
    property int borderTop
    property int borderLeft
    property int borderRight

    signal clicked

    BorderImage {
        id: brdImage
        source: buttonImageUrl
        anchors.fill: parent
        border.bottom: borderBottom
        border.top: borderTop
        border.left: borderLeft
        border.right: borderRight
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: parent

        onClicked: buttonImage.clicked()
    }

    states: [
        State {
            name: 'pressed'
            when: mouseRegion.pressed
            PropertyChanges { target: brdImage; source: pressedButtonImageUrl }
        }
    ]
}
