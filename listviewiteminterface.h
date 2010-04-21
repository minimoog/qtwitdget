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

#ifndef LISTVIEWITEMINTERFACE_H
#define LISTVIEWITEMINTERFACE_H

#include <QGraphicsItem>

class ListViewInterface;

class ListViewItemInterface : public QGraphicsItem
{
public:
    ListViewItemInterface(int index, ListViewInterface* view);
    int index() const;
    void setIndex(int index);
    virtual QSizeF size() const = 0;
    virtual void itemChanged(const QList<QByteArray> &roles = QList<QByteArray>()) = 0;
    virtual QVariant data() const = 0;
    ListViewInterface* view() const;

private:
    int m_index;
    ListViewInterface *m_view;
};

#endif // LISTVIEWITEMINTERFACE_H
