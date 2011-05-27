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

#ifndef TWEETENTITYLISTMODEL_H
#define TWEETENTITYLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class QTweetStatus;

/*
 *  Model class for entities in the tweet (mentions, links, hashtags)
 */
class TweetEntityListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString tweetid READ tweetid WRITE setTweetid /*NOTIFY tweetidChanged*/)
public:
    explicit TweetEntityListModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void loadEntities(const QString& tweetid);

    QString tweetid() const { return m_tweetid; }
    void setTweetid(const QString& id);

private slots:
    void finishedStatusShow(const QTweetStatus& status);

private:
    void captureEntities(const QString& text);
    QStringList captureRegex(const QString& rx, const QString& text);

    QStringList m_entities;
    QString m_tweetid;
};

#endif // TWEETENTITYLISTMODEL_H
