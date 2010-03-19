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

//Based on itemviews-ng

#ifndef TWEETLISTMODEL_H
#define TWEETLISTMODEL_H

#include <QObject>
#include <QtSql>
#include "qtwit/qtwitstatus.h"

class TweetListModel : public QObject
{
    Q_OBJECT
public:
    TweetListModel(QObject *parent = 0);
    int count() const;
    QVariant data(int index);
    void fetchNewTweets();

signals:
    void itemsInserted(int index, int count);
    void itemsRemoved(int index, int count);
    void itemsMoved(int from, int to, int count);
    void itemsChanged(int index, int count, const QList<QByteArray>& roles);

public slots:
    void replyDeleteClicked(int index);
    void retweetClicked(int index);
    void favoritedClicked(int index);

private:
    QList<QTwitStatus> m_statuses;
};

#endif // TWEETLISTMODEL_H
