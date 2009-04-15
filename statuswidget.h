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
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
 */

#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QDateTime>
#include "ui_statusform.h"

/*! class StatusWidget */
class StatusWidget : public QFrame
{
	Q_OBJECT
public:
	/*! Constructor */
	StatusWidget(QWidget *parent = 0);
	/*! sets avatar */
	void setProfileImagePixmap(const QPixmap &pixmap);
	/*! gets status text */
	QString text() const;
	/*! sets status text */
	void setText(const QString &text);
	/*! gets screen name */
	QString screenName() const;
	/*! sets screen name */
	void setScreenName(const QString &screenName);
	/*! gets source of status */
	QString source() const;
	/*! sets source of status */
	void setSource(const QString& source);
	/*! gets datetime of the status */
	QDateTime created() const;
	/*! sets datetime of the status */
	void setCreated(const QDateTime &datetime);
	/*! gets ID of the status */
	int statusId() const;
	/*! sets ID of the status */
	void setStatusId(int id);
	/*! enables/disables reply button */
	void setEnabledReplyDeleteButton(bool state);
	bool isEnabledReplyDeleteButton() const;
	/*! sets if the status is user own written status */
	/*! \remark hides groupBox */
	void setUserOwnStatus(bool own);
	bool isUserOwnStatus() const;

signals:
	/*! emited when text is entered in the reply text edit */
	/* replyText "@screenName " plus reply text */
	void replyRequest(const QString& replyText, int replyToStatusId);
	/*! emited when delete button is pressed */
	void deleteRequest(int statusId);

protected:
	void changeEvent(QEvent *e);

private slots:
	void replyReturnPressed();
	void deleteButtonClicked();

private:
	Ui::StatusForm ui;
	int m_statusId;
	bool m_ownStatus;

	QString m_screenName;
	QString m_source;
};

#endif //STATUSWIDGET_H