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

#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QHash>
#include "qtwit/qtwitfriends.h"
#include "ui_groupdialogform.h"

class GroupDialog : public QDialog 
{
    Q_OBJECT
   
public:
    GroupDialog(QWidget *parent = 0);
	GroupDialog(QNetworkAccessManager* netManager, OAuthTwitter *oauthTwitter, QWidget *parent = 0);
	void setNetworkManager(QNetworkAccessManager* netManager);
	void setOAuthTwitter(OAuthTwitter* oauthTwitter);
	QList<int> getGroupList() const;

protected:
    virtual void changeEvent(QEvent *e);

private slots:
	void finishedFriends();
	void insertButtonClicked();
	void createGroupButtonClicked();
	void removeButtonClicked();

private:
    Ui::GroupDialog m_ui;
	QTwitFriends *m_twitFriends;
	QHash<QString, int> m_scrNameToId;
	QList<int> m_groupList;

};

#endif // GROUPDIALOG_H