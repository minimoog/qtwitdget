/* Copyright (c) 2008, Antonie Jovanoski
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

#include <QApplication>
#include <QTranslator>
#include <QNetworkAccessManager>
#include "mainwindow.h"

QTranslator *appTranslator;		//global translator object
QNetworkAccessManager* MainWindow::m_s_netManager = 0;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
	QString lang = settings.value("Language").toString();

	appTranslator = new QTranslator;
	appTranslator->load(":/translations/qtwitdget_" + lang);
	app.installTranslator(appTranslator);

	MainWindow t;
	t.show();
	t.startUp();

	return app.exec();
}
