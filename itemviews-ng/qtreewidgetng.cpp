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

#include "qtreewidgetng.h"
#include "qtreewidgetng_p.h"
#include "qgraphicstreeview.h"
#include "qtreedefaultmodel.h"
#include "qtreeselectionmanager.h"
#include "qtreecontroller.h"
#include "qgraphicsheader.h"

#include <qscrollbar.h>

QT_BEGIN_NAMESPACE

/*!
  \internal
 */
QtTreeWidgetNGPrivate::QtTreeWidgetNGPrivate()
    : controller(0), header(0), q_ptr(0)
{
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_itemExpanded(QtTreeModelBase::iterator_base &it, int count)
{
    Q_Q(QtTreeWidgetNG);
    Q_UNUSED(it);
    Q_UNUSED(count);
    if (!timer.isActive())
        timer.start(0, q);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_itemCollapsed(QtTreeModelBase::iterator_base &it, int count)
{
    Q_Q(QtTreeWidgetNG);
    Q_UNUSED(it);
    Q_UNUSED(count);
    if (!timer.isActive())
        timer.start(0, q);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_itemsInserted(QtTreeModelBase::iterator_base &it, int count)
{
    Q_Q(QtTreeWidgetNG);
    Q_UNUSED(it);
    Q_UNUSED(count);
    if (!timer.isActive())
        timer.start(0, q);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_itemsRemoved(QtTreeModelBase::iterator_base &it, int count)
{
    Q_Q(QtTreeWidgetNG);
    Q_UNUSED(it);
    Q_UNUSED(count);
    if (!timer.isActive())
        timer.start(0, q);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_itemsMoved(QtTreeModelBase::iterator_base &from, QtTreeModelBase::iterator_base &to, int count)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(count);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_showView()
{
    Q_Q(QtTreeWidgetNG);
    Q_ASSERT(q->scene());
    Q_UNUSED(q);
    controller->view()->show();
    controller->view()->grabKeyboard();
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_hideView()
{
    Q_Q(QtTreeWidgetNG);
    Q_ASSERT(q->scene());
    Q_UNUSED(q);
    controller->view()->ungrabKeyboard();
    controller->view()->hide();
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_updateGeometries()
{
    Q_Q(QtTreeWidgetNG);

    // ### take header into account
    QRect rect = q->viewport()->geometry();
    q->setSceneRect(QRect(0, 0, rect.width(), rect.height()));

    qreal y = header ? /*header->preferredHeight()*/22 : 0;
    if (header)
        header->setGeometry(QRect(0, 0, rect.width(), y));

    controller->view()->setGeometry(QRect(0, y, rect.width(), rect.height() - y));
    controller->view()->doLayout();

    // ### check scrollbar policy
    QScrollBar *vertical = q->verticalScrollBar();
    QScrollBar *horizontal = q->horizontalScrollBar();

    int maximum = 0;
    int maximumFirst = controller->view()->maximumFirstIndex(&maximum);
    vertical->setMaximum(maximum);
    vertical->setPageStep(maximum - maximumFirst);

    maximum = controller->header() ? controller->header()->sectionCount() : 0;
    maximumFirst = controller->header() ? controller->header()->maximumFirstSection() : 0;
    horizontal->setMaximum(maximum);
    horizontal->setPageStep(maximum - maximumFirst);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_firstIndexChanged(int index)
{
    Q_Q(QtTreeWidgetNG);
    if (q->verticalScrollBar()->value() != index)
        q->verticalScrollBar()->setValue(index);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_firstSectionChanged(int section)
{
    Q_Q(QtTreeWidgetNG);
    if (q->horizontalScrollBar()->value() != section)
        q->horizontalScrollBar()->setValue(section);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_offsetChanged(qreal offset)
{
    Q_Q(QtTreeWidgetNG);
    int value = int(offset);
    if (q->verticalScrollBar()->value() != value)
        q->verticalScrollBar()->setValue(value);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_modelChanged(QtTreeModelBase *current, QtTreeModelBase *previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_selectionManagerChanged(QtTreeSelectionManager *current, QtTreeSelectionManager *previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_viewChanged(QtGraphicsTreeView *current, QtGraphicsTreeView *previous)
{
    Q_Q(QtTreeWidgetNG);
    if (previous) {
         QObject::disconnect(previous, SIGNAL(itemExpanded(QtTreeModelIterator&,int)),
                   q, SLOT(_q_itemExpanded(QtTreeModelIterator&,int)));
        QObject::disconnect(previous, SIGNAL(itemCollapsed(QtTreeModelIterator&,int)),
                   q, SLOT(_q_itemCollapsed(QtTreeModelIterator&,int)));
        q->scene()->removeItem(previous);
    }
    if (current) {
        QObject::connect(current, SIGNAL(itemExpanded(QtTreeModelIterator&,int)),
                   q, SLOT(_q_itemExpanded(QtTreeModelIterator&,int)));
        QObject::connect(current, SIGNAL(itemCollapsed(QtTreeModelIterator&,int)),
                   q, SLOT(_q_itemCollapsed(QtTreeModelIterator&,int)));
        q->scene()->addItem(current);
        current->grabKeyboard();
    }
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_controllerChanged(QtTreeController *current, QtTreeController *previous)
{
    Q_Q(QtTreeWidgetNG);
    if (previous) {
        QObject::disconnect(previous, SIGNAL(modelChanged(QtTreeModelBase*,QtTreeModelBase*)),
                            q, SLOT(_q_modelChanged(QtTreeModelBase*,QtTreeModelBase*)));
        QObject::disconnect(previous, SIGNAL(selectionManagerChanged(QtTreeSelectionManager*,QtTreeSelectionManager)),
                            q, SLOT(_q_selectionManagerChanged(QtTreeSelectionManager*, QtTreeSelectionManager*)));
        QObject::disconnect(previous, SIGNAL(viewChanged(QtGraphicsTreeView*, QtGraphicsTreeView*)),
                            q, SLOT(_q_viewChanged(QtGraphicsTreeView*, QtGraphicsTreeView*)));
        QObject::disconnect(q->verticalScrollBar(), SIGNAL(valueChanged(int)),
                            previous, SLOT(setVerticalScrollValue(int)));
        QObject::disconnect(q->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                            previous, SLOT(setHorizontalScrollValue(int)));
    }
    if (current) {
        QObject::connect(current, SIGNAL(modelChanged(QtTreeModelBase*,QtTreeModelBase*)),
                         q, SLOT(_q_modelChanged(QtTreeModelBase*,QtTreeModelBase*)));
        QObject::connect(current, SIGNAL(selectionManagerChanged(QtTreeSelectionManager*,QtTreeSelectionManager*)),
                         q, SLOT(_q_selectionManagerChanged(QtTreeSelectionManager*, QtTreeSelectionManager*)));
        QObject::connect(current, SIGNAL(viewChanged(QtGraphicsTreeView*, QtGraphicsTreeView*)),
                         q, SLOT(_q_viewChanged(QtGraphicsTreeView*, QtGraphicsTreeView*)));
        QObject::connect(q->verticalScrollBar(), SIGNAL(valueChanged(int)),
                         current, SLOT(setVerticalScrollValue(int)));
        QObject::connect(q->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                         current, SLOT(setHorizontalScrollValue(int)));
    }
    _q_viewChanged(current ? current->view() : 0, previous ? previous->view() : 0);
    _q_headerChanged(current ? current->header() : 0, previous ? previous->header() : 0);
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::_q_headerChanged(QtGraphicsHeader *current, QtGraphicsHeader *previous)
{
    Q_Q(QtTreeWidgetNG);
    if (previous) {
        q->scene()->removeItem(previous);
    }
    if (current) {
        q->scene()->addItem(current);
    }
}

/*!
  \internal
 */
void QtTreeWidgetNGPrivate::initialize()
{
    Q_Q(QtTreeWidgetNG);
    q->setViewportMargins(0, 0, 0, 0);
    q->setScene(new QGraphicsScene(q));
    if (!controller)
        controller = new QtTreeController;
    if (!controller->model())
        controller->setModel(new QtTreeDefaultModel(controller));
    if (!controller->selectionManager())
        controller->setSelectionManager(new QtTreeSelectionManager(controller));
    if (!controller->view()) {
        controller->setView(new QtGraphicsTreeView);
        controller->view()->setParent(controller);
    }
    if (!header) {
        header = new QtGraphicsHeader;
        controller->setHeader(header);
    }
    //controller->setWheelEnabled(false);
    _q_controllerChanged(controller, 0);
}

// QtTreeWidgetNG

/*!
 */
QtTreeWidgetNG::QtTreeWidgetNG(QWidget *parent)
    : QGraphicsView(parent), d_ptr(new QtTreeWidgetNGPrivate)
{
    Q_D(QtTreeWidgetNG);
    d->q_ptr = this;
    d->initialize();
}

/*!
 */
QtTreeWidgetNG::QtTreeWidgetNG(QtTreeController *controller, QWidget *parent)
    : QGraphicsView(parent), d_ptr(new QtTreeWidgetNGPrivate)
{
    Q_D(QtTreeWidgetNG);
    Q_ASSERT(controller);
    d->q_ptr = this;
    d->controller = controller;
    controller->setParent(this);
    d->initialize();
}

/*!
 */
QtTreeWidgetNG::~QtTreeWidgetNG()
{
    delete d_ptr;
}

/*!
  Returns the pointer to the tree controller used by
  the tree widget.
 */
QtTreeController *QtTreeWidgetNG::controller() const
{
    Q_D(const QtTreeWidgetNG);
    return d->controller;
}

/*!
  Returns the pointer to the default model used by
  the tree widget.
 */
QtTreeDefaultModel *QtTreeWidgetNG::defaultModel() const
{
    Q_D(const QtTreeWidgetNG);
    return qobject_cast<QtTreeDefaultModel*>(d->controller->model());
}

/*!
 */
void QtTreeWidgetNG::showEvent(QShowEvent *event)
{
    Q_D(QtTreeWidgetNG);
    d->_q_updateGeometries();
    QGraphicsView::showEvent(event);
}

/*!
  \reimp
 */
void QtTreeWidgetNG::resizeEvent(QResizeEvent *event)
{
    Q_D(QtTreeWidgetNG);
    d->_q_updateGeometries();
    QAbstractScrollArea::resizeEvent(event);
}

/*!
  \reimp
 */
void QtTreeWidgetNG::timerEvent(QTimerEvent *event)
{
    Q_D(QtTreeWidgetNG);
    if (event->timerId() == d->timer.timerId()) {
        //d->updateScrollRanges();
        d->controller->view()->update();
        d->timer.stop();
    }
    QGraphicsView::timerEvent(event);
}

/*!
 \reimp
 */
void QtTreeWidgetNG::scrollContentsBy(int dx, int dy)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);
    // do nothing
}

QT_END_NAMESPACE

#include "moc_qtreewidgetng.cpp"
