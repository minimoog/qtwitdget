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

#ifndef LISTVIEWINTERFACE_H
#define LISTVIEWINTERFACE_H

#include <QGraphicsItem>

class ListModelInterface;

class ListViewInterface : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    ListViewInterface(QGraphicsItem *parent = 0);
    ListModelInterface* model() const;
    void setModel(ListModelInterface* model);
    virtual int itemAt(const QPointF& position) const = 0;
    virtual QSizeF itemGeometry(int index) const = 0;
    virtual QGraphicsItem* itemForIndex(int index) const = 0;
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected slots:
    virtual void itemsInserted(int index, int count) = 0;
    virtual void itemsRemoved(int index, int count) = 0;
    virtual void itemsMoved(int from, int to, int count) = 0;
    virtual void itemsChanged(int index, int count, const QList<QByteArray> &roles) = 0;

private:
    ListModelInterface *m_model;
};

#endif // LISTVIEWINTERFACE_H
