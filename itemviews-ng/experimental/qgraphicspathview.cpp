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

#include "qgraphicspathview.h"
#include "../qgraphicslistview_p.h"

#include "../qlistselectionmanager.h"
#include "../qlistmodelinterface.h"

#include <qdebug.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qtimeline.h>
#include <qapplication.h>

QT_BEGIN_NAMESPACE

class QtGraphicsPathViewPrivate : public QtGraphicsListViewPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsPathView)
public:
    QtGraphicsPathViewPrivate();
    void _q_currentChanged(int current, int previous);

    QPainterPath path;
    QTimeLine timeline;
};

QtGraphicsPathViewPrivate::QtGraphicsPathViewPrivate()
    : QtGraphicsListViewPrivate()
{
    /*
    path.moveTo(50,0);
    path.lineTo(50,100);
    *//**/
    path.moveTo(0,0);
    path.quadTo(QPointF(100, 25), QPointF(50,50));
    path.quadTo(QPointF(0, 75), QPointF(100,100));
    /**/
}

void QtGraphicsPathViewPrivate::_q_currentChanged(int current, int previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    QTimeLine::Direction direction = current > previous ? QTimeLine::Backward : QTimeLine::Forward;
    if (previous == 0 && current == model->count() - 1)
        direction = QTimeLine::Forward;
    if (current == 0 && previous == model->count() - 1)
        direction = QTimeLine::Backward;
    timeline.stop();
    timeline.setDuration(500);
    timeline.setDirection(direction);
    timeline.start();
}

/*!
  \class QtGraphicsPathView
  \brief A path list view
*/

QtGraphicsPathView::QtGraphicsPathView(QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QtGraphicsListView(*(new QtGraphicsPathViewPrivate), Qt::Vertical, parent, wFlags)
{
}

QtGraphicsPathView::~QtGraphicsPathView()
{
}

QPainterPath QtGraphicsPathView::path() const
{
    Q_D(const QtGraphicsPathView);
    return d->path;
}

void QtGraphicsPathView::setPath(const QPainterPath &path)
{
    Q_D(QtGraphicsPathView);
    d->path = path;
}

int QtGraphicsPathView::maximumFirstIndex() const
{
    return 0;
}

qreal QtGraphicsPathView::maximumHorizontalOffset() const
{
    return 0;
}

qreal QtGraphicsPathView::maximumVerticalOffset() const
{
    return 0;
}

QRectF QtGraphicsPathView::itemGeometry(int index) const
{
    Q_UNUSED(index);
    // ### FIXME: implementation goes here
    return QRectF();
}

int QtGraphicsPathView::itemAt(const QPointF &position) const
{
    Q_UNUSED(position);
    // ### FIXME: implementation goes here
    return -1;
}

void QtGraphicsPathView::doLayout()
{
    Q_D(QtGraphicsPathView);
    QStyleOptionViewItemV4 option;
    initStyleOption(&option);

    // ### ignores offsets
    const QPainterPath path = d->path;
    const int firstItem = 0; // ###
    const int count = model() ? model()->count() - (firstItem * 2) : 0; // ###
    const int current = qBound(firstItem, d->currentItem(), count);
    const int n = (current - (count / 2));

    int index = (n < 0 ? n + count : n);
    int counter = firstItem;

    // scale the path
    const QRectF boundingRect = this->boundingRect(); // ###
    const QRectF pathBoundingRect = path.boundingRect();
    qreal pathWidth = pathBoundingRect.width();
    qreal pathHeight = pathBoundingRect.height();
    qreal horizontalFactor = (pathWidth > 0 ? (boundingRect.width() / pathWidth) : 1);
    qreal verticalFactor = (pathHeight > 0 ?  (boundingRect.height() / pathHeight) : 1);

    // animation
    bool running = d->timeline.state() == QTimeLine::Running;
    bool forward = d->timeline.direction() == QTimeLine::Forward;
    qreal progress = (running ? (forward ? d->timeline.currentValue() - 1 : d->timeline.currentValue()) : 0);

    while (!path.isEmpty() && counter < count) {
        qreal c = qreal(counter) + progress;
        qreal t = qBound(qreal(0), c / qreal(count), qreal(1));
        QPointF pos = path.pointAtPercent(t);
        pos.rx() *= horizontalFactor;
        pos.ry() *= verticalFactor;
        pos += boundingRect.topLeft();

        // ### FIXME: look up and recycle the items
        QGraphicsObject *item = 0;
        if (counter >= d->items.count()) {
            item = d->creator->create(index, this);
            d->items.append(QPair<int,QGraphicsObject*>(index, item));
        } else {
            item = d->items.at(counter).second;
            d->creator->reassign(index, item, this);
        }
        item->setPos(pos);

        ++index;
        if (index >= count) // loop
            index = firstItem;
        ++counter;
    }
}

void QtGraphicsPathView::setSelectionManager(QtListSelectionManager *selectionManager)
{
    if (QtListSelectionManager *previous = QtGraphicsListView::selectionManager())
        QObject::disconnect(previous, SIGNAL(currentChanged(int,int)), this, SLOT(_q_currentChanged(int,int)));
    QtGraphicsListView::setSelectionManager(selectionManager);
    if (QtListSelectionManager *current = QtGraphicsListView::selectionManager())
        QObject::connect(current, SIGNAL(currentChanged(int,int)), this, SLOT(_q_currentChanged(int,int)));
}

QT_END_NAMESPACE

#include "moc_qgraphicspathview.cpp"
