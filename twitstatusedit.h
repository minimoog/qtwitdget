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

#ifndef TWITSTATUSEDIT_H
#define TWITSTATUSEDIT_H

#include <QPlainTextEdit>

/*!
    Editor for tweet updates
*/
class TwitStatusEdit : public QPlainTextEdit
{
	Q_OBJECT
	Q_PROPERTY(int limit READ limit WRITE setLimit)
public:
	TwitStatusEdit(QWidget *parent = 0);
	void setLimit(int limit);
	int limit() const;
	void setStatusId(qint64 statusId);
	qint64 statusId() const;

signals:
	/*! emitted when size of text goes over limit and under limit (from overlimit) */
	void overLimit(bool);
	/*! emitted when Return or Enter are pressed */
	void returnPressed();

public slots:
	void shortUrls();
	void setReply(qint64 id, const QString& screenName);
    void setReply(const QString& id, const QString& screenName);
	void setRetweet(const QString& text, const QString& screenName);

protected:
	void keyPressEvent(QKeyEvent *e);

private slots:
	void onTextChanged();

private:
	int m_limit;
	qint64 m_statusId;
	bool m_isOverLimit;
};

#endif //TWITSTATUSEDIT_H
