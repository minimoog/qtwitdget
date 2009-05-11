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

#include <QtDebug>
#include "groupdialog.h"

GroupDialog::GroupDialog(QWidget *parent) 
	: QDialog(parent), m_twitFriends(new QTwitFriends(this))
{
    m_ui.setupUi(this);
	connect(m_twitFriends, SIGNAL(finished()), this, SLOT(finishedFriends()));
	connect(m_ui.insertButton, SIGNAL(clicked()), this, SLOT(insertButtonClicked()));
	connect(m_ui.createGroupButton, SIGNAL(clicked()), this, SLOT(createGroupButtonClicked()));
	connect(m_ui.removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
	m_twitFriends->updateFriends();
}

GroupDialog::GroupDialog(QNetworkAccessManager* netManager, OAuthTwitter *oauthTwitter, QWidget *parent)
:	QDialog(parent),
	m_twitFriends(new QTwitFriends(netManager, oauthTwitter, this))
{
	m_ui.setupUi(this);
	connect(m_twitFriends, SIGNAL(finished()), this, SLOT(finishedFriends()));
	connect(m_ui.insertButton, SIGNAL(clicked()), this, SLOT(insertButtonClicked()));
	connect(m_ui.createGroupButton, SIGNAL(clicked()), this, SLOT(createGroupButtonClicked()));
	connect(m_ui.removeButton, SIGNAL(clicked()), this, SLOT(removeButtonClicked()));
	m_twitFriends->updateFriends();
}

void GroupDialog::setNetworkManager(QNetworkAccessManager* netManager)
{
	m_twitFriends->setNetworkAccessManager(netManager);
}

void GroupDialog::setOAuthTwitter(OAuthTwitter* oauthTwitter)
{
	m_twitFriends->setOAuthTwitter(oauthTwitter);
}

void GroupDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
	
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void GroupDialog::finishedFriends()
{
	QList<QTwitExtUserInfo> listFriends = m_twitFriends->getFriends();

	foreach(const QTwitExtUserInfo& fr, listFriends){
		m_ui.friendsListWidget->addItem(fr.screenName());
		m_scrNameToId.insert(fr.screenName(), fr.id());
	}
}

void GroupDialog::insertButtonClicked()
{
	QList<QListWidgetItem*> items = m_ui.friendsListWidget->selectedItems();
	foreach(QListWidgetItem* it, items){
		//find duplicate
		QList<QListWidgetItem*> sameItems = m_ui.groupListWidget->findItems(it->text(), Qt::MatchExactly);
		if(sameItems.isEmpty()){
			m_ui.groupListWidget->addItem(it->text());
		}
	}
}

QList<int> GroupDialog::getGroupList() const
{
	return m_groupList;
}

void GroupDialog::createGroupButtonClicked()
{
	for(int i = 0; i < m_ui.groupListWidget->count(); ++i){
		QListWidgetItem *lwit = m_ui.groupListWidget->item(i);
		m_groupList.append(m_scrNameToId.value(lwit->text()));
	}
}

void GroupDialog::removeButtonClicked()
{
	QList<QListWidgetItem*> selected = m_ui.groupListWidget->selectedItems();
	qDebug() << selected.count();
	foreach(QListWidgetItem* it, selected){
		m_ui.groupListWidget->removeItemWidget(it);
		delete it;
	}
}