/* Copyright (c) 2010, Antonie Jovanoski
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

#ifndef QTWITSCENEUNREAD_H
#define QTWITSCENEUNREAD_H

#include "qtwitscene.h"

class QTwitSceneUnread : public QTwitScene
{
    Q_OBJECT
public:
    QTwitSceneUnread(QObject * parent);
    /*! refreshes statuses */
    virtual void updateStatuses();
    void addNextUnreadStatus();
    /*! doesnt' do nothing */
    virtual void nextStatuses();
    /*! doesn't marks just removes all statuses */
    virtual void markAllRead();
};

#endif //QTWITSCENEUNREAD_H
