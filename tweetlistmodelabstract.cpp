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

#include "tweetlistmodelabstract.h"

TweetListModelAbstract::TweetListModelAbstract(QObject *parent) :
    QObject(parent)
{
}

int TweetListModelAbstract::userid() const
{
    return m_userid;
}

void TweetListModelAbstract::setUserid(int userid)
{
    m_userid = userid;
}

QString TweetListModelAbstract::additionalQuery() const
{
    return m_additionalQuery;
}

void TweetListModelAbstract::setAdditionalQuery(const QString &query)
{
    m_additionalQuery = query;
}

