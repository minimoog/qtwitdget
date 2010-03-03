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

#include "qtablewidgetng.h"
#include "qtablewidgetng_p.h"

#include "qtablecontroller.h"
#include "qtableselectionmanager.h"
#include "qtabledefaultmodel.h"
#include "qgraphicstableview.h"
#include "qgraphicsheader.h"

#include <qdebug.h>
#include <qscrollbar.h>

QT_BEGIN_NAMESPACE

QtTableWidgetNGPrivate::QtTableWidgetNGPrivate()
    : controller(0), q_ptr(0)
{
}

void QtTableWidgetNGPrivate::_q_update(const QRegion &region)
{
    Q_UNUSED(region);
}

void QtTableWidgetNGPrivate::_q_rowsInserted(int row, int count)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
}

void QtTableWidgetNGPrivate::_q_rowsRemoved(int row, int count)
{
    Q_UNUSED(row);
    Q_UNUSED(count);
}

void QtTableWidgetNGPrivate::_q_rowsMoved(int from, int to, int count)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(count);
}

void QtTableWidgetNGPrivate::_q_columnsInserted(int column, int count)
{
    Q_UNUSED(column);
    Q_UNUSED(count);
}

void QtTableWidgetNGPrivate::_q_columnsRemoved(int column, int count)
{
    Q_UNUSED(column);
    Q_UNUSED(count);
}

void QtTableWidgetNGPrivate::_q_columnsMoved(int from, int to, int count)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(count);
}

void QtTableWidgetNGPrivate::_q_showView()
{
    controller->view()->show();
    controller->view()->grabKeyboard();
}

void QtTableWidgetNGPrivate::_q_hideView()
{
    controller->view()->ungrabKeyboard();
    controller->view()->hide();
}

void QtTableWidgetNGPrivate::_q_updateGeometries()
{
    Q_Q(QtTableWidgetNG);

    QScrollBar *vertical = q->verticalScrollBar();
    QScrollBar *horizontal = q->horizontalScrollBar();

    qreal maximum = 0;

    vertical->setPageStep(controller->verticalPageStepValue(&maximum));
    vertical->setMaximum(maximum);

    horizontal->setPageStep(controller->horizontalPageStepValue(&maximum));
    horizontal->setMaximum(maximum);

    QRect rect = q->viewport()->geometry();
    controller->view()->setGeometry(QRect(0, 0, rect.width(), rect.height()));
    controller->view()->doLayout();
}

void QtTableWidgetNGPrivate::_q_modelChanged(QtTableModelInterface *current, QtTableModelInterface *previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

void QtTableWidgetNGPrivate::_q_selectionManagerChanged(QtTableSelectionManager *current, QtTableSelectionManager *previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

void QtTableWidgetNGPrivate::_q_viewChanged(QtGraphicsTableView *current, QtGraphicsTableView *previous)
{
    Q_Q(QtTableWidgetNG);
    if (previous)
        q->scene()->removeItem(previous);
    if (current)
        q->scene()->addItem(current);
}

void QtTableWidgetNGPrivate::_q_controllerChanged(QtTableController *current, QtTableController *previous)
{
    Q_Q(QtTableWidgetNG);
    if (previous) {
        QObject::disconnect(previous, SIGNAL(modelChanged(QtTableModelInterface*,QtTableModelInterface*)),
                            q, SLOT(_q_modelChanged(QtTableModelInterface*,QtTableModelInterface*)));
        QObject::disconnect(previous, SIGNAL(selectionManagerChanged(QtTableSelectionManager*,QtTableSelectionManager*)),
                            q, SLOT(_q_selectionManagerChanged(QtTableSelectionManager*,QtTableSelectionManager*)));
        QObject::disconnect(q->verticalScrollBar(), SIGNAL(valueChanged(int)),
                            previous, SLOT(setVerticalScrollValue(int)));
        QObject::disconnect(q->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                            previous, SLOT(setHorizontalScrollValue(int)));
    }
    if (current) {
        QObject::connect(current, SIGNAL(modelChanged(QtTableModelInterface*,QtTableModelInterface*)),
                         q, SLOT(_q_modelChanged(QtTableModelInterface*,QtTableModelInterface*)));
        QObject::connect(current, SIGNAL(selectionManagerChanged(QtTableSelectionManager*,QtTableSelectionManager*)),
                         q, SLOT(_q_selectionManagerChanged(QtTableSelectionManager*,QtTableSelectionManager*)));
        QObject::connect(q->verticalScrollBar(), SIGNAL(valueChanged(int)),
                         current, SLOT(setVerticalScrollValue(int)));
        QObject::connect(q->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                         current, SLOT(setHorizontalScrollValue(int)));
    }
    _q_modelChanged(current ? current->model() : 0, previous ? previous->model() : 0);
    _q_selectionManagerChanged(current ? current->selectionManager() : 0, previous ? previous->selectionManager() : 0);
    _q_viewChanged(current ? current->view() : 0, previous ? previous->view() : 0);
}

void QtTableWidgetNGPrivate::initialize(int rows, int columns)
{
    Q_Q(QtTableWidgetNG);
    q->setScene(new QGraphicsScene(q));
    // other parts
    if (!controller)
        controller = new QtTableController(q);
    if (!controller->model())
        controller->setModel(new QtTableDefaultModel(rows, columns, controller));
    if (!controller->selectionManager())
        controller->setSelectionManager(new QtTableSelectionManager(controller));
    if (!controller->view()) {
        controller->setView(new QtGraphicsTableView);
        controller->view()->setParent(controller);
        controller->view()->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    }
    _q_controllerChanged(controller, 0);
}

// QtTableWidgetNG

/*!
 */
QtTableWidgetNG::QtTableWidgetNG(QWidget *parent, int rows, int columns)
    : QGraphicsView(parent), d_ptr(new QtTableWidgetNGPrivate)
{
    Q_D(QtTableWidgetNG);
    d->q_ptr = this;
    d->initialize(rows, columns);
}

/*!
 */
QtTableWidgetNG::QtTableWidgetNG(QtTableController *controller, QWidget *parent)
    : QGraphicsView(parent), d_ptr(new QtTableWidgetNGPrivate)
{
    Q_D(QtTableWidgetNG);
    d->q_ptr = this;
    d->controller = controller;
    d->initialize();
}

/*!
 */
QtTableWidgetNG::~QtTableWidgetNG()
{
    delete d_ptr;
}

/*!
 */
QtTableController *QtTableWidgetNG::controller() const
{
    Q_D(const QtTableWidgetNG);
    return d->controller;
}

/*!
 */
QtTableDefaultModel *QtTableWidgetNG::defaultModel() const
{
    Q_D(const QtTableWidgetNG);
    return qobject_cast<QtTableDefaultModel*>(d->controller->model());
}

/*!
 */
void QtTableWidgetNG::showEvent(QShowEvent *event)
{
    Q_D(QtTableWidgetNG);
    d->_q_updateGeometries();
    QGraphicsView::showEvent(event);
}

/*!
 */
void QtTableWidgetNG::resizeEvent(QResizeEvent *event)
{
    Q_D(QtTableWidgetNG);
    d->_q_updateGeometries();
    QAbstractScrollArea::resizeEvent(event);
}

/*!
 */
void QtTableWidgetNG::scrollContentsBy(int dx, int dy)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);
    // do nothing
}

QT_END_NAMESPACE

#include "moc_qtablewidgetng.cpp"
