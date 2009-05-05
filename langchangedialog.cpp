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
 
#include <QSettings>
#include <QTranslator>
#include "langchangedialog.h"

extern QTranslator *appTranslator;	//global

LangChangeDialog::LangChangeDialog(QWidget *parent)
	:	QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	QSettings settings("QTwitdget", "QTwitdget");
	QString lang = settings.value("Language").toString();

	if(lang == "en"){
		ui.enButton->setChecked(true);
	} 
	else if(lang == "mk") {
		ui.mkButton->setChecked(true);
	}
}

void LangChangeDialog::changeEvent(QEvent *e)
{
	if(e->type() == QEvent::LanguageChange)
		ui.retranslateUi(this);

	QDialog::changeEvent(e);
}

void LangChangeDialog::languageChanged()
{
	QSettings settings;

	if(ui.enButton->isChecked()){
		appTranslator->load(":/translations/qtwitdget_en");
		settings.setValue("Language", "en");
	}
	else if(ui.mkButton->isChecked()){
		appTranslator->load(":/translations/qtwitdget_mk");
		settings.setValue("Language", "mk");
	}
}