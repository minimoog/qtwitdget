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

#include "tweetview.h"
#include "tweetviewitem.h"
#include <itemviews-ng/qlistmodelinterface.h>

TweetView::TweetView(QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QtGraphicsListView(Qt::Vertical, parent, wFlags)
{
    setItemCreator(new QtGraphicsListViewItemCreator<TweetViewItem>());
}

void TweetView::setModel(QtListModelInterface *model_)
{
    if (model()) {
        disconnect(model(), SIGNAL(itemsInserted(int, int)), this, SLOT(itemsInserted(int,int)));
        disconnect(model(), SIGNAL(itemsChanged(int, int, const QList<QByteArray>&)),
                   this, SLOT(itemsChanged(int,int,QList<QByteArray>&)));
    }

    QtGraphicsListView::setModel(model_);

    if (model()) {
        connect(model(), SIGNAL(itemsInserted(int, int)), this, SLOT(itemsInserted(int,int)));
        connect(model(), SIGNAL(itemsChanged(int, int, const QList<QByteArray>&)),
                this, SLOT(itemsChanged(int,int,QList<QByteArray>&)));
    }
}

void TweetView::itemsInserted(int index, int count)
{
    for (int i = 0; i < count; ++i) {
        if (TweetViewItem *item = static_cast<TweetViewItem*>(itemForIndex(index + i))) {
            //something
        }
    }

    doLayout();
}

void TweetView::itemsChanged(int index, int count, const QList<QByteArray> &roles)
{
    for (int i = 0; i < count; ++i) {
        if (TweetViewItem *item = static_cast<TweetViewItem*>(itemForIndex(index + i))) {
            //do something
        }
    }

    doLayout();
}

