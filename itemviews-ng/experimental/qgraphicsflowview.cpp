/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Itemviews NG project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "qgraphicsflowview.h"
#include "../qgraphicslistview_p.h"
#include "../qlistmodelinterface.h"
#include "../qlistselectionmanager.h"

#include <qstyleoption.h>
#include <qmath.h>
#include <qdebug.h>
#include <qlist.h>
#include <qpainter.h>

#define MARGIN 6

class QtGraphicsFlowViewPrivate : public QtGraphicsListViewPrivate
{
public:
    QtGraphicsFlowViewPrivate();

    int iconsPerRow() const;

    QSizeF itemSize;
};

class QtGraphicsFlowViewItem : public QtGraphicsListViewItem
{
public:
    QtGraphicsFlowViewItem(int index, QtGraphicsListView *layout);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    Q_DECLARE_PRIVATE(QtGraphicsListViewItem)
};

class QtGraphicsFlowViewItemCreator : public QtGraphicsListViewItemCreatorBase
{
public:
    virtual QtGraphicsListViewItem *create(int index, QtGraphicsListView *view);
    virtual void recycle(QtGraphicsListViewItem *item);

private:
    QList<QtGraphicsListViewItem*> m_recycledItems;
};

QtGraphicsFlowViewPrivate::QtGraphicsFlowViewPrivate()
    : itemSize(80, 60)
{
    textElideMode = Qt::ElideNone;
    creator = new QtGraphicsListViewItemCreator<QtGraphicsFlowViewItem>();
}

int QtGraphicsFlowViewPrivate::iconsPerRow() const
{
    return qMax(1, (int) (q_ptr->size().width() / (MARGIN + itemSize.width())));
}

void QtGraphicsFlowViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(QtGraphicsListViewItem);
    Q_UNUSED(widget);

    if (!d->view || !d->view->model())
        return;
    d->view->initStyleOption(&d->option);
    d->view->initStyleOption(&d->option, d->index);

    QHash<int, QVariant> itemData = d->view->model()->data(d->index, QList<int>() << Qt::DecorationRole);
    QVariant var = itemData.value(Qt::DecorationRole);
    if (var.isNull())
        return;

    if (d->view->selectionManager() && d->view->selectionManager()->isSelected(d->index)) {
        // TODO this 'active' may not be the proper state
        painter->fillRect(option->rect.adjusted(-3, -3, 3, 3), option->palette.brush(QPalette::Active, QPalette::Highlight));
    }

    if (var.type() == QVariant::Icon) {
        QMatrix matrix = painter->matrix();
        const qreal scaleX = matrix.m11();
        const qreal scaleY = matrix.m22();
        matrix.translate(option->rect.x(), option->rect.y());
        matrix.scale(1/scaleX, 1/scaleY);
        painter->setMatrix(matrix);

        const QIcon icon = var.value<QIcon>();
        icon.paint(painter, 0, 0, option->rect.width() * scaleX, option->rect.height() * scaleY);
    } else if(var.type() == QVariant::Pixmap) {
        const QPixmap pixmap = qvariant_cast<QPixmap>(var);
        painter->drawPixmap(option->rect, pixmap);
    }
}

QtGraphicsFlowViewItem::QtGraphicsFlowViewItem(int index, QtGraphicsListView *layout)
    : QtGraphicsListViewItem(index, layout)
{
    setCacheMode(DeviceCoordinateCache);
}

QtGraphicsListViewItem *QtGraphicsFlowViewItemCreator::create(int index, QtGraphicsListView *view)
{
    QtGraphicsListViewItem *item;
    if (m_recycledItems.isEmpty()) {
        item = new QtGraphicsFlowViewItem(index, view);
    } else {
        item = m_recycledItems.takeLast();
        item->setIndex(index);
    }
    return item;
}

void QtGraphicsFlowViewItemCreator::recycle(QtGraphicsListViewItem *item)
{
    m_recycledItems.append(item);
}


/*!
  \class QtGraphicsFlowView
  \brief A view that shows a single list of items as a flow of icons.

  This implementation of the QtGraphicsListView does layout of each of its items
  in a flow, either horizontal or vertical. If the user adjusts the size of the view,
  the amount of icons per row (or column if orientation isvertical) will be adjusted
  to fit.
*/

QtGraphicsFlowView::QtGraphicsFlowView(Qt::Orientation orientation, QGraphicsWidget *parent)
    : QtGraphicsListView(*(new QtGraphicsFlowViewPrivate), orientation, parent)
{
}

QtGraphicsFlowView::~QtGraphicsFlowView()
{
}

QRectF QtGraphicsFlowView::itemGeometry(int index) const
{
    Q_UNUSED(index);
    return QRectF(); // ### FIXME: implement
}

int QtGraphicsFlowView::itemAt(const QPointF &position) const
{
    Q_D(const QtGraphicsFlowView);
    if (! d->model)
        return -1;
    qreal x = (orientation() == Qt::Horizontal ? offset() : 0) + position.x() + MARGIN; // lets add the margin here so we don't have to special case the first column later
    const int column = x / (d->itemSize.width() + MARGIN);
    const int itemsPerRow = d->iconsPerRow();
    if (column > itemsPerRow) // right of our items
        return -1;
    x -= column * (d->itemSize.width() + MARGIN);
    if (x < MARGIN) // in between items
        return -1;

    qreal y = (orientation() == Qt::Vertical ? offset() : 0) + position.y() + MARGIN; // lets add the margin here so we don't have to special case the first row later
    const int row = y / (d->itemSize.height() + MARGIN);
    y -= row * (d->itemSize.height() + MARGIN);
    if (y < MARGIN)
        return -1;
    const int result = row * itemsPerRow + column + firstIndex();
    if (result >= d->model->count())
        return -1;
    return result;
}

int QtGraphicsFlowView::maximumFirstIndex() const
{
    Q_D(const QtGraphicsFlowView);

    if (!d->model)
        return 0;
    const int rowsVisible = size().height() / (d->itemSize.height() + MARGIN);
    const int rowsRequired = qCeil(d->model->count() / (float) d->iconsPerRow());
    return qMax(0, rowsRequired - rowsVisible);
}

void QtGraphicsFlowView::doLayout()
{
    Q_D(QtGraphicsFlowView);

    const int itemsPerRow = d->iconsPerRow();
    const int totalRows = qCeil((d->model->count() - d->firstIndex) / (float) itemsPerRow);
    const QRectF area = geometry();
    const int visibleRows = qMin(totalRows, qCeil(area.height() / (d->itemSize.height() + MARGIN)));
    const int amountNeeded = qMin(d->model->count() - d->firstIndex, visibleRows * itemsPerRow);

    if (!d->items.isEmpty()) {
        // reuse old items by moving them around. Working hard to make them keep their index.
        // Starting from firstIndex, ending at firstIndex + amountNeeded
        const int index = d->firstIndex;
        while (d->items.first()->index() < index) { // scrolling down
            QtGraphicsListViewItem *item = d->items.takeFirst();
            item->setIndex(d->items.last()->index() + 1);
            d->items.append(item);
        }
        const int lastIndex = index + d->items.count();
        while (d->items.last()->index() > lastIndex) { // scrolling up
            QtGraphicsListViewItem *item = d->items.takeLast();
            item->setIndex(d->items.first()->index() - 1);
            d->items.prepend(item);
        }
    }

    while (amountNeeded < d->items.count()) // remove old ones
        d->creator->recycle(d->items.takeLast());
    while (amountNeeded > d->items.count() && (d->items.isEmpty() || d->items.first()->index() > d->firstIndex))
            d->items.prepend(d->creator->create(d->firstIndex, this));
    qreal x = 0;
    qreal y = 0;
    for (int index = d->firstIndex; index < d->firstIndex + amountNeeded; ++index) {
        QtGraphicsListViewItem *item = 0;
        const int indexInItems = index - d->firstIndex;
        if (indexInItems >= d->items.count()) {
            item = d->creator->create(index, this);
            d->items.append(item);
        } else {
            item = d->items.at(indexInItems);
        }
        item->setGeometry(x, y, d->itemSize.width(), d->itemSize.height());
        x += d->itemSize.width();
        if (x + d->itemSize.width() >= area.width()) {
            x = 0;
            y += d->itemSize.height() + MARGIN;
        } else {
            x += MARGIN;
        }
    }
}

QSizeF QtGraphicsFlowView::itemSize(const QStyleOptionViewItemV4 *, int ) const
{
    Q_D(const QtGraphicsFlowView);
    return d->itemSize;
}


