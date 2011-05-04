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

#ifndef SEARCHQMLLISTMODEL_H
#define SEARCHQMLLISTMODEL_H

#include <QAbstractListModel>
#include "qtweetlib/qtweetsearchpageresults.h"

class OAuthTwitter;

class SearchQmlListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SearchResultRoles {
        SinceTimeRole = Qt::UserRole + 1,
        ScreenNameRole,
        AvatarUrlRole,
        StatusTextRole,
        StatusIdRole
    };

    explicit SearchQmlListModel(QObject *parent = 0);
    SearchQmlListModel(OAuthTwitter* oauthTwitter, QObject *parent = 0);
    virtual ~SearchQmlListModel();
    void setOAuthTwitter(OAuthTwitter* oauthTwitter);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void startSearch(const QString& query);

private slots:
    void finishedSearch(const QTweetSearchPageResults& pageResults);

private:
    OAuthTwitter *m_oauthTwitter;
    QTweetSearchPageResults m_searchPageResult;
};

#endif // SEARCHQMLLISTMODEL_H
