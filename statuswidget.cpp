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

//"\\b(https?|ftp|file)://[-A-Z0-9+&@#/%?=~_|$!:,.;]*[A-Z0-9+&@#/%=~_|$]"
//"\\b(?:(?:https?|ftp|file)://|www\\.|ftp\\.)(?:\\([-A-Z0-9+&@#/%=~_|$?!:,.]*\\)|[-A-Z0-9+&@#/%=~_|$?!:,.])*(?:\\([-A-Z0-9+&@#/%=~_|$?!:,.]*\\)|[A-Z0-9+&@#/%=~_|$])"
//"\\(?\\bhttp://[-A-Za-z0-9+&@#/%?=~_()|!:,.;]*[-A-Za-z0-9+&@#/%=~_()|]"

#include <QRegExp>
#include "statuswidget.h"

StatusWidget::StatusWidget(QWidget *parent)
:	QFrame(parent)
{
	ui.setupUi(this);
	ui.replyTextEdit->setLimit(140);
	ui.groupBox->hide();

	connect(ui.replyTextEdit, SIGNAL(returnPressed()), SLOT(replyReturnPressed()));
}

void StatusWidget::setText(const QString &text)
{
	QRegExp rx("\\(?\\bhttp://[-A-Za-z0-9+&@#/%?=~_()|!:,.;]*[-A-Za-z0-9+&@#/%=~_()|]", Qt::CaseInsensitive, QRegExp::RegExp2);
	
	//make a copy
	QString textWithLinks(text);
	
	//replace urls with href
	int pos = 0;
	while(pos >= 0){
		pos = rx.indexIn(textWithLinks, pos);
	
		if(pos >= 0){
			int length = rx.matchedLength();
			QString matchedUrl = textWithLinks.mid(pos, length);
			QString href = "<a href=\"" + matchedUrl + "\">" + matchedUrl + "</a>";
			textWithLinks.replace(pos, length, href);
			int advance = href.size() - matchedUrl.size();	//how much to advanced after replacement
			pos += (length + advance);
		}
	}

	ui.textBrowser->clearHistory();
	ui.textBrowser->setHtml(textWithLinks);
}

void StatusWidget::setUserOwnStatus(bool own)
{
	if(own){
		m_ownStatus = true;
		ui.replyDeleteButton->setText(tr("Delete"));
		ui.replyDeleteButton->setCheckable(false);
		disconnect(ui.replyDeleteButton, SIGNAL(toggled(bool)), 0, 0);
		connect(ui.replyDeleteButton, SIGNAL(clicked()), SLOT(deleteButtonClicked()));
		ui.groupBox->hide();
	} else {
		m_ownStatus = false;
		ui.replyDeleteButton->setText(tr("Reply"));
		ui.replyDeleteButton->setCheckable(true);
		disconnect(ui.replyDeleteButton, SIGNAL(clicked()), 0, 0);
		connect(ui.replyDeleteButton, SIGNAL(toggled(bool)), ui.replyTextEdit, SLOT(setFocus()));
		connect(ui.replyDeleteButton, SIGNAL(toggled(bool)), ui.groupBox, SLOT(setVisible(bool)));
		ui.groupBox->hide();
	}
}

bool StatusWidget::isUserOwnStatus() const
{
	return m_ownStatus;
}

QString StatusWidget::text() const
{
	return ui.textBrowser->toPlainText();
}

void StatusWidget::setScreenName(const QString &screenName)
{
	m_screenName = screenName;
	ui.labelScreenName->setText(m_screenName + tr(" from ") + m_source);
}

QString StatusWidget::screenName() const
{
	return m_screenName;
}

void StatusWidget::setSource(const QString &source)
{
	m_source = source;
	ui.labelScreenName->setText(m_screenName + tr(" from ") + m_source);
}

QString StatusWidget::source() const
{
	return m_source;
}

void StatusWidget::setCreated(const QDateTime &datetime)
{
	ui.labelCreated->setText(datetime.toLocalTime().toString());
}

QDateTime StatusWidget::created() const
{
	return QDateTime::fromString(ui.labelCreated->text());
}

void StatusWidget::setStatusId(int id)
{
	m_statusId = id;
}

int StatusWidget::statusId() const
{
	return m_statusId;
}

void StatusWidget::setEnabledReplyDeleteButton(bool state)
{
	ui.replyDeleteButton->setEnabled(state);
}

bool StatusWidget::isEnabledReplyDeleteButton() const
{
	return ui.replyDeleteButton->isEnabled();
}

void StatusWidget::setProfileImagePixmap(const QPixmap &pixmap)
{
	ui.labelProfileImage->setPixmap(pixmap);
}

void StatusWidget::replyReturnPressed()
{
	QString replyText = QString("@") + screenName() + QString(" ") + ui.replyTextEdit->toPlainText();
	emit replyRequest(replyText, m_statusId);

	ui.replyDeleteButton->toggle();
	ui.replyTextEdit->clear();
}

void StatusWidget::deleteButtonClicked()
{
	emit deleteRequest(m_statusId);
}

void StatusWidget::changeEvent(QEvent *e)
{
	if(e->type() == QEvent::LanguageChange)
		ui.retranslateUi(this);

	QFrame::changeEvent(e);
}
