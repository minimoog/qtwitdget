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

#include "listviewiteminterface.h"
#include "listviewinterface.h"


ListViewItemInterface::ListViewItemInterface(int index, ListViewInterface *view)
    :   QGraphicsItem(view)
{
    m_index = index;
    m_view = view;
}

int ListViewItemInterface::index() const
{
    return m_index;
}

void ListViewItemInterface::setIndex(int index)
{
    m_index = index;
}

ListViewInterface* ListViewItemInterface::view() const
{
    return m_view;
}
