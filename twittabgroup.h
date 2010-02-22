/* Copyright (c) 2009, Antonie Jovanoski
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
 
#ifndef TWITTABGROUP_H
#define TWITTABGROUP_H

#include <QString>

/*! \class TwitTabGroup */
/*! Stores settings per tab */
class TwitTabGroup
{
public:

    enum Type {
        Normal,
        Unread
    };

	/* Constructor */
    TwitTabGroup() {}

	/* Sets/gets additional query for tab group */
	void setQuery(const QString& query) { m_queryString = query; }
	QString query() const { return m_queryString; }

	/* Sets/gets tab name */
	void setTabName(const QString& tabName) { m_tabName = tabName; }
	QString tabName() const { return m_tabName; }

    void setType(Type type) { m_type = type; }
    Type type() const { return m_type; }

    void setCloseable(bool closeable) { m_closeable = closeable; }
    bool closeable() const { return m_closeable; }

private:
	QString m_queryString;
	QString m_tabName;
    Type m_type;
    bool m_closeable;
};
 
#endif //TWITTABGROUP_H
