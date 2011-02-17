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

#ifndef MENTIONSQMLLISTMODEL_H
#define MENTIONSQMLLISTMODEL_H

#include "tweetqmllistmodel.h"

class QTweetStatus;

class MentionsQmlListModel : public TweetQmlListModel
{
    Q_OBJECT
public:
    MentionsQmlListModel(QObject *parent = 0);
    void fetchLastTweets();

protected:
    void loadTweetsFromDatabase();

public slots:
    void onStatusesStream(const QTweetStatus& status);

private slots:
    void finishedFetchTweets(const QList<QTweetStatus>& statuses);
};

#endif // MENTIONSQMLLISTMODEL_H
