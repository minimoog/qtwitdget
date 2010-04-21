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

#include "listviewinterface.h"
#include "listmodelinterface.h"

ListViewInterface::ListViewInterface(QGraphicsItem *parent) :
    QGraphicsItem(parent), m_model(0)
{
}

ListModelInterface* ListViewInterface::model() const
{
    return m_model;
}

void ListViewInterface::setModel(ListModelInterface *model)
{
    if (model == m_model)
        return;

    if (m_model) {
        disconnect(m_model, SIGNAL(itemsChanged(int,int,QList<QByteArray>)),
                   this, SLOT(itemsChanged(int,int,QList<QByteArray>)));
        disconnect(m_model, SIGNAL(itemsInserted(int,int)), this, SLOT(itemsInserted(int,int)));
        disconnect(m_model, SIGNAL(itemsRemoved(int,int)), this, SLOT(itemsRemoved(int,int)));
        disconnect(m_model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(itemsMoved(int,int,int)));
        // ### signal: destroyed()  ????
    }

    m_model = model;
    m_model->setView(this);

    if (m_model) {
        connect(m_model, SIGNAL(itemsChanged(int,int,QList<QByteArray>)),
                this, SLOT(itemsChanged(int,int,QList<QByteArray>)));
        connect(m_model, SIGNAL(itemsInserted(int,int)), this, SLOT(itemsInserted(int,int)));
        connect(m_model, SIGNAL(itemsRemoved(int,int)), this, SLOT(itemsRemoved(int,int)));
        connect(m_model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(itemsMoved(int,int,int)));
        // ### signal: destroyed()  ????
    }
}

QRectF ListViewInterface::boundingRect() const
{
    // nothing to do
    return QRectF();
}

void ListViewInterface::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // nothing to paint, just a holder for list items
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
