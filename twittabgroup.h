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
	/* Constructor */
	TwitTabGroup() : m_page(0), m_statusesPerPage(50) {}

	/* Sets/gets page */
	void setPage(int page) { m_page = page; }
	int page() const { return m_page; }

	/* Increments/decrements the page number */
	void increasePage() { m_page += 1; }
	void decreasePage() { if(m_page > 0 ) m_page -= 1; }

	/* Sets/gets number of statuses per page 
	 * \remark default number is 50
	 */
	void setNumStatusesPerPage(int n) { m_statusesPerPage = n; }
	int numStatusePerPage() const { return m_statusesPerPage; }

	/* Gets number of statuses */
	int numStatuses() const { return (m_page + 1) * m_statusesPerPage; }

	/* Sets/gets additional query for tab group */
	void setQuery(const QString& query) { m_queryString = query; }
	QString query() const { return m_queryString; }

	/* Sets/gets tab name */
	void setTabName(const QString& tabName) { m_tabName = tabName; }
	QString tabName() const { return m_tabName; }

    /* Sets/gets Last (most newest) status id in this group */
    void setLastStatusId(qint64 id) { m_lastStatusId = id; }
    qint64 lastStatusId() const { return m_lastStatusId; }

    /* Sets/gets first (oldest) status id in this group to be shown */
    /* Statuses between lastStatusId and firstStatusId are shown on the scene */
    /* When new status arrives lastStatusId is updated accordingly */
    /* When users wants to see more oldest statutes firstStatusId is updated accordingly */
    void setFirstStatusId(qint64 id) { m_firstStatusId = id; }
    qint64 firstStatusId() const { return m_firstStatusId; }

private:
	int m_page;
	int m_statusesPerPage;
    qint64 m_lastStatusId;
    qint64 m_firstStatusId;
	QString m_queryString;
	QString m_tabName;
};
 
#endif //TWITTABGROUP_H