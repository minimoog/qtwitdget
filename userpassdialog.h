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

#ifndef USERPASSDIALOG_H
#define USERPASSDIALOG_H

#include <QDialog>
#include "ui_userpass.h"

/*! \class UserPassDialog */
class UserPassDialog : public QDialog
{
	Q_OBJECT

public:
	/*! Constructor */
	UserPassDialog(QWidget *parent = 0);

	/*! Gets username */
	QString username() const;
	/*! Sets username */
	void setUsername(const QString &user);
	/*! Sets focus to be on password line edit */
	void setPasswordEditFocut() const;
	/*! Gets password */
	QString password() const;

protected:
	void changeEvent(QEvent *e);

private:
	Ui::UserPassDialog ui;
};

#endif //USERPASSDIALOG_H