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

#include <QRegExp>
#include "twitstatusedit.h"
#include "shortenedurl.h"

TwitStatusEdit::TwitStatusEdit(QWidget *parent)
	:	QPlainTextEdit(parent), m_limit(16777215), m_isOverLimit(false)
{
	connect(this, SIGNAL(textChanged()), SLOT(onTextChanged()));
}

void TwitStatusEdit::setLimit(int limit)
{
	m_limit = limit;
}

int TwitStatusEdit::limit() const
{
	return m_limit;
}

void TwitStatusEdit::onTextChanged()
{
	if(toPlainText().size() > m_limit){

		if(!m_isOverLimit){
			m_isOverLimit = true;
			emit overLimit(true);
			setStyleSheet("QPlainTextEdit { background-color: red }");
		}
		
	} else {

		if(m_isOverLimit){
			m_isOverLimit = false;
			emit overLimit(false);
			//Be careful TODO: what if style sheet is set in QDesigner? append? 
			setStyleSheet(QString());
		}
	}
}

void TwitStatusEdit::keyPressEvent(QKeyEvent *e)
{
	switch(e->key()){
		case Qt::Key_Tab:	//ignore Tab
			break;

		case Qt::Key_Return:
		case Qt::Key_Enter:

			if(toPlainText().size() <= m_limit)
				emit returnPressed();
			break;

		default:
			QPlainTextEdit::keyPressEvent(e);
	}
}

void TwitStatusEdit::shortUrls()
{
	QRegExp rx("\\(?\\bhttp://[-A-Za-z0-9+&@#/%?=~_()|!:,.;]*[-A-Za-z0-9+&@#/%=~_()|]", Qt::CaseInsensitive, QRegExp::RegExp2);	
	QString txt = toPlainText();

	bool textChanged = false;
		
	int pos = 0;
	while(pos >= 0){
		pos = rx.indexIn(txt, pos);

		if(pos >= 0){
			int length = rx.matchedLength();
			QString matchedUrl = txt.mid(pos, length);

			if(!matchedUrl.contains("http://is.gd/")){
				ShortenedUrl sUrl;
				QString newUrl = sUrl.shortUrl(matchedUrl);

				txt.replace(pos, length, newUrl);
				int advance = newUrl.size() - matchedUrl.size();
				pos += (length + advance);
				textChanged = true;
			} else {
				pos += length;
			}
		}
	}

	if(textChanged)
		setPlainText(txt);
}

