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

#include "qgraphicsgridview.h"

#include "../qgraphicslistview_p.h"
#include "../qlistselectionmanager.h"
#include "../qlistmodelinterface.h"

#include <qdebug.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qapplication.h>
#include <qevent.h>
//#include <qtimeline.h>
#include <qpalette.h>

QT_BEGIN_NAMESPACE

class QtGraphicsGridViewPrivate : public QtGraphicsListViewPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsGridView)
public:
    QtGraphicsGridViewPrivate();
    void _q_selectionManagerChanged(QtListSelectionManager *current, QtListSelectionManager *previous);
    void _q_currentChanged(int current, int previous);
    //void _q_update();

    int itemAbove(int index) const;
    int itemBelow(int index) const;
    int itemLeft(int index) const;
    int itemRight(int index) const;
    
    //bool keyPressEvent(QKeyEvent *event);

    qreal gridSize() const;
    QPointF gridPosition(int index) const;

    int firstVisibleIndex(const QRectF &area) const;
    int lastVisibleIndex(const QRectF &area) const;

    int normalSectionCount() const;

    int sectionCount;
    //int prev;
};

QtGraphicsGridViewPrivate::QtGraphicsGridViewPrivate()
    : sectionCount(3)//, prev(-1)
{
    //pulse.setLoopCount(0);
    //pulse.setCurveShape(QTimeLine::SineCurve);
    //pulse.setDuration(1000);
    //movement.setDuration(250);
}

void QtGraphicsGridViewPrivate::_q_selectionManagerChanged(QtListSelectionManager *current, QtListSelectionManager *previous)
{
    Q_Q(QtGraphicsGridView);
    if (previous)
        QObject::disconnect(previous, SIGNAL(currentChanged(int,int)), q, SLOT(_q_currentChanged(int,int)));
    if (current)
        QObject::connect(current, SIGNAL(currentChanged(int,int)), q, SLOT(_q_currentChanged(int,int)));
}

void QtGraphicsGridViewPrivate::_q_currentChanged(int current, int previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    //prev = previous;
    //pulse.stop();
    //movement.stop();
    //movement.start();
}

int QtGraphicsGridViewPrivate::itemAbove(int index) const
{
    if (index == 0) {
        index = model->count() - 1;
    } else {
        index -= sectionCount;
        if (index < 0)
            index = model->count() + index - 1;
    }
    return index;
}

int QtGraphicsGridViewPrivate::itemBelow(int index) const
{
    if (index == model->count() - 1) {
        index = 0;
    } else {
        index += sectionCount;
        if (index >= model->count())
            index -= model->count() - 1;
    }
    return index;
}

int QtGraphicsGridViewPrivate::itemLeft(int index) const
{
    if (--index < 0)
        index = model->count() - 1;
    return index;
}

int QtGraphicsGridViewPrivate::itemRight(int index) const
{
    if (++index >= model->count())
        index = 0;
    return index;
}
/*
bool QtGraphicsGridViewPrivate::keyPressEvent(QKeyEvent *event)
{
    if (selectionManager) {
        int index = selectionManager->currentItem();
        switch (event->key()) {
        case Qt::Key_Up:
            if (orientation == Qt::Vertical)
                index = itemAbove(index);
            else
                index = itemLeft(index);
            break;
        case Qt::Key_Left:
            if (orientation == Qt::Vertical)
                index = itemLeft(index);
            else
                index = itemAbove(index);
            break;
        case Qt::Key_Down:
            if (orientation == Qt::Vertical)
                index = itemBelow(index);
            else
                index = itemRight(index);
            break;
        case Qt::Key_Right:
            if (orientation == Qt::Vertical)
                index = itemRight(index);
            else
                index = itemBelow(index);
            break;
        default:
            break;
        }
        if (selectionManager->currentItem() != index) {
            selectionManager->setCurrentItem(index);
            selectionManager->clearSelections();
            selectionManager->setSelected(index);
            return true;
        }
    }
    return false;
}
*/
qreal QtGraphicsGridViewPrivate::gridSize() const
{
    Q_Q(const QtGraphicsGridView);
    const QRectF rect = q->geometry();
    if (orientation == Qt::Vertical)
        return qMin<qreal>(rect.width() / sectionCount, rect.height());
    else
        return qMin<qreal>(rect.height() / sectionCount, rect.width());
}

QPointF QtGraphicsGridViewPrivate::gridPosition(int index) const
{
    qreal x, y;
    qreal s = gridSize();
    if (orientation == Qt::Vertical) {
        x = (index % sectionCount) * s;
        y = (index / sectionCount) * s;
        y -= firstIndex * s; // scrolling
        y -= offset; // scrolling
    } else {
        x = (index / sectionCount) * s;
        y = (index % sectionCount) * s;
        x -= firstIndex * s; // scrolling
        x -= offset; // scrolling
    }
    return QPointF(x, y);
}

int QtGraphicsGridViewPrivate::firstVisibleIndex(const QRectF &area) const
{
    qreal s = gridSize();
    if (orientation == Qt::Vertical)
        return qMax(0, int(area.y() / s) - 1) * sectionCount;
    else
        return qMax(0, int(area.x() / s) - 1) * sectionCount;
}

int QtGraphicsGridViewPrivate::lastVisibleIndex(const QRectF &area) const
{
    qreal s = gridSize();
    if (orientation == Qt::Vertical)
        return qMax(0, int(area.bottom() / s)) * sectionCount + sectionCount - 1;
    else
        return qMax(0, int(area.right() / s)) * sectionCount + sectionCount - 1;
}

int QtGraphicsGridViewPrivate::normalSectionCount() const
{
    int count = model ? model->count() : 0;
    int remainder = (count % sectionCount);
    return (count / sectionCount) + (remainder > 0 ? 1 : 0);
}

// QtGraphicsGridView

/*!
  \class QtGraphicsGridView
  \brief An view which positions the list items in a grid

  The QtGraphicsGridView is either used horizontally or vertically, which changes
  the orientation of a scrollbar would one be required.
 */

/*!
 */
QtGraphicsGridView::QtGraphicsGridView(int sections, Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QtGraphicsListView(*(new QtGraphicsGridViewPrivate), orientation, parent, wFlags)
{
    //Q_D(QtGraphicsGridView);

    //connect(this, SIGNAL(selectionManagerChanged(QtListSelectionManager*, QtListSelectionManager*)),
    //        this, SLOT(_q_selectionManagerChanged(QtListSelectionManager*, QtListSelectionManager*)));

    //connect(&d->movement, SIGNAL(valueChanged(qreal)), this, SLOT(_q_update()));
    //connect(&d->movement, SIGNAL(finished()), this, SLOT(_q_update()));
    //connect(&d->movement, SIGNAL(finished()), &d->pulse, SLOT(start()));

    //connect(&d->pulse, SIGNAL(valueChanged(qreal)), this, SLOT(_q_update()));
    //connect(&d->pulse, SIGNAL(finished()), this, SLOT(_q_update()));

    setOrientation(orientation);
    setSectionCount(sections);
}

/*!
 */
QtGraphicsGridView::QtGraphicsGridView(QtGraphicsGridViewPrivate &dd, Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QtGraphicsListView(dd, orientation, parent, wFlags)
{
    //Q_D(QtGraphicsGridView);

    //connect(this, SIGNAL(selectionManagerChanged(QtListSelectionManager*, QtListSelectionManager*)),
    //        this, SLOT(_q_selectionManagerChanged(QtListSelectionManager*, QtListSelectionManager*)));

    //connect(&d->movement, SIGNAL(valueChanged(qreal)), this, SLOT(_q_update()));
    //connect(&d->movement, SIGNAL(finished()), this, SLOT(_q_update()));
    //connect(&d->movement, SIGNAL(finished()), &d->pulse, SLOT(start()));
    
    //connect(&d->pulse, SIGNAL(valueChanged(qreal)), this, SLOT(_q_update()));
    //connect(&d->pulse, SIGNAL(finished()), this, SLOT(_q_update()));
}

/*!
 */
QtGraphicsGridView::~QtGraphicsGridView()
{
}

/*!
 */
int QtGraphicsGridView::sectionCount() const
{
    Q_D(const QtGraphicsGridView);
    return d->sectionCount;
}

/*!
 */
void QtGraphicsGridView::setSectionCount(int count)
{
    Q_D(QtGraphicsGridView);
    d->sectionCount = count;
}

/*!
  \reimp
 */
int QtGraphicsGridView::maximumFirstIndex() const
{
    Q_D(const QtGraphicsGridView);
    int visible = (d->orientation == Qt::Vertical ? geometry().height() : geometry().width()) / d->gridSize();
    return d->normalSectionCount() - visible;
}

/*!
  \reimp
 */
qreal QtGraphicsGridView::maximumHorizontalOffset() const
{
    Q_D(const QtGraphicsGridView);
    if (d->orientation == Qt::Vertical)
        return 0;
    qreal content = d->normalSectionCount() * d->gridSize();
    return content - geometry().width();
}

/*!
  \reimp
 */
qreal QtGraphicsGridView::maximumVerticalOffset() const
{
    Q_D(const QtGraphicsGridView);
    if (d->orientation == Qt::Horizontal)
        return 0;
    qreal content = d->normalSectionCount() * d->gridSize();
    return content - geometry().height();
}

/*!
  \reimp
 */
void QtGraphicsGridView::doLayout()
{
    Q_D(QtGraphicsGridView);
    QStyleOptionViewItemV4 option;
    initStyleOption(&option);

    //const int current = d->currentItem();
    //const int previous = d->prev;
    const int count = d->model ? d->model->count() : 0;
    const qreal gridSize =  d->gridSize();

    //const QPointF currentPos = d->gridPosition(current);
    //const QPointF previousPos = d->gridPosition(previous);

    // ### FIXME: for now
    const qreal verticalOffset = (orientation() == Qt::Vertical ? offset() : 0);
    const qreal horizontalOffset = (orientation() == Qt::Horizontal ? offset() : 0);

    // update item geometry
    const QRectF viewport(QPointF(horizontalOffset, verticalOffset), size());
    const int firstVisibleIndex = d->firstVisibleIndex(viewport);
    const int lastVisibleIndex = qMin(d->lastVisibleIndex(viewport), count - 1);
    //d->scrollItems(firstVisibleIndex); // ### FIXME
    int index = firstVisibleIndex;
    for (; index <= lastVisibleIndex; ++index) {
        //initStyleOption(&option, index);
        const int i = index - firstVisibleIndex;
        if (i >= d->items.count())
            d->items.append(QPair<int,QGraphicsObject*>(index, d->creator->create(index, this)));
        QGraphicsObject *item = d->items.at(i).second;
        const QPointF pos = d->gridPosition(index);
        const QSizeF size = (item->isWidget()
                             ? static_cast<QGraphicsWidget*>(item)->effectiveSizeHint(Qt::PreferredSize, QSizeF(gridSize, gridSize))
                             : item->boundingRect().size());
        const QRectF grid(pos.x(), pos.y(), gridSize, gridSize);
        const QRectF aligned = QStyle::alignedRect(option.direction,
                                                    Qt::AlignCenter,
                                                    size.toSize(),
                                                    grid.toRect());
        if (item->isWidget()) // ### FIXME
            static_cast<QGraphicsWidget*>(item)->setGeometry(aligned);
        else
            item->setPos(aligned.topLeft());
    }
    // remove unused items
    const int from = index - firstVisibleIndex;
    while (from < d->items.count())
        d->creator->recycle(d->items.takeLast().second);
}

/*!
  \reimp
 */
QRectF QtGraphicsGridView::itemGeometry(int index) const
{
    Q_D(const QtGraphicsGridView);
    const QPointF position = d->gridPosition(index);
    const qreal size = d->gridSize();
    return QRectF(position, QSizeF(size, size));
}

/*!
  \reimp
 */
int QtGraphicsGridView::itemAt(const QPointF &position) const
{
    Q_D(const QtGraphicsGridView);
    qreal size = d->gridSize();
    qreal x = position.x();
    qreal y = position.y();
    int index = -1;
    if (d->orientation == Qt::Vertical)
        index = int((offset() + y + d->firstIndex * size) / size) * d->sectionCount + int(x / size);
    else
        index = int((offset() + x + d->firstIndex * size) / size) * d->sectionCount + int(y / size);
    if (index < (d->model ? d->model->count() : 0))
        return index;
    return -1;
}

/*!
  \reimp
 */
void QtGraphicsGridView::initStyleOption(QStyleOptionViewItemV4 *option) const
{
    QtGraphicsListView::initStyleOption(option);
    QStyle *style = option->widget ? option->widget->style() : QApplication::style();
    int pm = style->pixelMetric(QStyle::PM_IconViewIconSize);
    option->decorationSize = QSize(pm, pm);
    option->decorationPosition = QStyleOptionViewItem::Top;
    option->displayAlignment = Qt::AlignCenter;
    option->widget = 0;
}

/*!
  \reimp
 */
/*
void QtGraphicsGridView::initStyleOption(QStyleOptionViewItemV4 *option, int index) const
{
    QtGraphicsListView::initStyleOption(option, index);
    option->state &= ~QStyle::State_HasFocus;
    option->state &= ~QStyle::State_Selected;
}
*/
QT_END_NAMESPACE

#include "moc_qgraphicsgridview.cpp"
