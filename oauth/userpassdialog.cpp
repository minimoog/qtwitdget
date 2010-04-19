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

#include "userpassdialog.h"
#include "ui_userpassdialog.h"

UserPassDialog::UserPassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserPassDialog)
{
    ui->setupUi(this);
}

QString UserPassDialog::username() const
{
    return ui->usernameLineEdit->text();
}

QString UserPassDialog::password() const
{
    return ui->passwordLineEdit->text();
}

UserPassDialog::~UserPassDialog()
{
    delete ui;
}

void UserPassDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
