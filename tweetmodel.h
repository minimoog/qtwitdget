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

#ifndef TWEETMODEL_H
#define TWEETMODEL_H

#include <QtSql>
#include "qtwit/qtwitstatus.h"
#include "itemviews-ng/qlistmodelinterface.h"

class TweetModel : public QtListModelInterface
{
    Q_OBJECT
public:
    TweetModel(QObject *parent = 0);
    ~TweetModel();

    int count() const;
    QHash<QByteArray,QVariant> data(int index, const QList<QByteArray> &roles) const;
    void fetchNewTweets();

private:
    QList<QTwitStatus> m_statuses;

};

#endif // TWEETMODEL_H
