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

#include "usertimelinelistmodel.h"
#include <QDateTime>
#include "qtweetlib/oauthtwitter.h"
#include "qtweetlib/qtweetstatus.h"
#include "qtweetlib/qtweetusertimeline.h"

static QString SinceTimeString(const QDateTime& from)
{
    int passedSeconds = from.secsTo(QDateTime::currentDateTimeUtc());

    if (passedSeconds < 0)
        return QString("Time travel!");

    if (passedSeconds < 60)
        return QString("%1 seconds ago").arg(passedSeconds);

    if (passedSeconds < 3600)
        return QString("%1 minutes ago").arg(passedSeconds / 60);

    if (passedSeconds < 86400)
        return QString("%1 hours ago").arg(passedSeconds / 3600);

    return QString("%1 days ago").arg(passedSeconds / 86400);
}

/**
 * Constructor
 */
UserTimelineListModel::UserTimelineListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    m_roles[StatusTextRole] = "statusTextRole";
    m_roles[SinceTimeRole] = "sinceTimeRole";
    m_roles[StatusIdRole] = "statusIdRole";
}

/**
 *  Constructor
 */
UserTimelineListModel::UserTimelineListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
    QAbstractListModel(parent),
    m_oauthTwitter(oauthTwitter)
{
    m_roles[StatusTextRole] = "statusTextRole";
    m_roles[SinceTimeRole] = "sinceTimeRole";
    m_roles[StatusIdRole] = "statusIdRole";
}

/**
 *  Sets oauth twitter object
 */
void UserTimelineListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *  @reimp
 */
int UserTimelineListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_statuses.count();
}

/**
 *  @reimp
 */
QVariant UserTimelineListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_statuses.count())
        return QVariant();

    const QTweetStatus& st = m_statuses.at(index.row());

    if (role == StatusTextRole)
        return st.text();
    else if (role == SinceTimeRole)
        return SinceTimeString(st.createdAt());
    else if (role == StatusIdRole)
        return QString::number(st.id());

    return QVariant();
}

QHash<int, QByteArray> UserTimelineListModel::roleNames() const
{
    return m_roles;
}

/**
 *  Fetches user timeline
 *  @param userid id of the user to fetch the timeline
 */
void UserTimelineListModel::fetch(qint64 userid)
{
    QTweetUserTimeline *userTimeline = new QTweetUserTimeline(m_oauthTwitter, this);
    userTimeline->fetch(userid, QString(), 0, 0, 20);
    connect(userTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(finishedFetching(QList<QTweetStatus>)));
    connect(userTimeline, SIGNAL(error(ErrorCode,QString)),
            this, SLOT(errorFetching()));
}

/**
 *  Fetches user timeline
 *  @param screenname screenname of the user to fetch the timeline
 */
void UserTimelineListModel::fetch(const QString &screenname)
{
    QTweetUserTimeline *userTimeline = new QTweetUserTimeline(m_oauthTwitter, this);
    userTimeline->fetch(0, screenname, 0, 0, 20);
    connect(userTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(finishedFetching(QList<QTweetStatus>)));
    connect(userTimeline, SIGNAL(error(ErrorCode,QString)),
            this, SLOT(errorFetching()));
}

/**
 *  Called when fetching user timeline is finished
 */
void UserTimelineListModel::finishedFetching(const QList<QTweetStatus> &statuses)
{
    QTweetUserTimeline *userTimeline = qobject_cast<QTweetUserTimeline*>(sender());

    if (userTimeline) {

        if (!m_statuses.isEmpty()) {
            beginRemoveRows(QModelIndex(), 0, m_statuses.count() - 1);
            m_statuses.clear();
            endRemoveRows();
        }

        if (!statuses.isEmpty()) {
            beginInsertRows(QModelIndex(), 0, statuses.count() - 1);
            m_statuses.append(statuses);
            endInsertRows();
        }

        userTimeline->deleteLater();
    }
}

void UserTimelineListModel::errorFetching()
{
    QTweetUserTimeline *userTimeline = qobject_cast<QTweetUserTimeline*>(sender());

    if (userTimeline)
        userTimeline->deleteLater();
}
