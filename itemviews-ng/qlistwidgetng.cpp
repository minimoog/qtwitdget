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

#include "qlistwidgetng.h"
#include "qlistwidgetng_p.h"

#include "qlistdefaultmodel.h"
#include "qlistcontroller.h"
#include "qlistselectionmanager.h"
#include "qgraphicslistview.h"

#include <qscrollbar.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

class QtGraphicsViewport : public QGraphicsView
{
public:
    QtGraphicsViewport(QGraphicsScene *scene)
        : QGraphicsView(scene)
    {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setViewportMargins(0, 0, 0, 0);
        setFrameShape(QFrame::NoFrame);
    }

    void scrollContentsBy(int dx, int dy)
    {
        Q_UNUSED(dx);
        Q_UNUSED(dy);
        // do nothing
    }
};

QtListWidgetNGPrivate::QtListWidgetNGPrivate()
    : scene(0), controller(0), q_ptr(0)
{
}

void QtListWidgetNGPrivate::_q_update(const QRegion &region)
{
    Q_UNUSED(region);
}

void QtListWidgetNGPrivate::_q_itemsInserted(int index, int count)
{
    Q_UNUSED(index);
    Q_UNUSED(count);
    //_q_updateGeometries(); // ### FIXME
}

void QtListWidgetNGPrivate::_q_itemsRemoved(int index, int count)
{
    Q_UNUSED(index);
    Q_UNUSED(count);
}

void QtListWidgetNGPrivate::_q_itemsMoved(int from, int to, int count)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(count);
}

void QtListWidgetNGPrivate::_q_itemsChanged(int from, int count, const QList<int> &roles)
{
    Q_UNUSED(from);
    Q_UNUSED(count);
    Q_UNUSED(roles);
    //_q_updateGeometries(); // ### FIXME
}

void QtListWidgetNGPrivate::_q_showView()
{
    if (controller && controller->view()) {
        controller->view()->show();
        controller->view()->grabKeyboard();
    }
}

void QtListWidgetNGPrivate::_q_hideView()
{
    if (controller && controller->view()) {
        controller->view()->ungrabKeyboard();
        controller->view()->hide();
    }
}

void QtListWidgetNGPrivate::_q_controllerChanged(QtListController *current, QtListController *previous)
{
    Q_Q(QtListWidgetNG);
    if (previous) {
        QObject::disconnect(previous, SIGNAL(modelChanged(QtListModelInterface*,QtListModelInterface*)),
                            q, SLOT(_q_modelChanged(QtListModelInterface*,QtListModelInterface*)));
        QObject::disconnect(previous, SIGNAL(selectionManagerChanged(QtListSelectionManager*,QtListSelectionManager*)),
                            q, SLOT(_q_selectionManagerChanged(QtListSelectionManager*,QtListSelectionManager*)));
        QObject::disconnect(previous, SIGNAL(viewChanged(QtGraphicsListView*,QtGraphicsListView*)),
                            q, SLOT(_q_viewChanged(QtGraphicsListView*,QtGraphicsListView*)));
        QObject::disconnect(q->verticalScrollBar(), SIGNAL(valueChanged(int)),
                             previous, SLOT(setScrollValue(int)));

    }
    if (current) {
        QObject::connect(current, SIGNAL(modelChanged(QtListModelInterface*,QtListModelInterface*)),
                         q, SLOT(_q_modelChanged(QtListModelInterface*,QtListModelInterface*)));
        QObject::connect(current, SIGNAL(selectionManagerChanged(QtListSelectionManager*,QtListSelectionManager*)),
                         q, SLOT(_q_selectionManagerChanged(QtListSelectionManager*,QtListSelectionManager*)));
        QObject::connect(current, SIGNAL(viewChanged(QtGraphicsListView*,QtGraphicsListView*)),
                         q, SLOT(_q_viewChanged(QtGraphicsListView*,QtGraphicsListView*)));
        QObject::connect(q->verticalScrollBar(), SIGNAL(valueChanged(int)),
                          current, SLOT(setScrollValue(int)));
    }
    // everything changed
    _q_modelChanged(current ? current->model() : 0, previous ? previous->model() : 0);
    _q_selectionManagerChanged(current ? current->selectionManager() : 0, previous ? previous->selectionManager() : 0);
    _q_viewChanged(current ? current->view() : 0, previous ? previous->view() : 0);
}

void QtListWidgetNGPrivate::_q_modelChanged(QtListModelInterface *current, QtListModelInterface *previous)
{
    Q_Q(QtListWidgetNG);
    if (previous) {
        QObject::disconnect(previous, SIGNAL(itemsInserted(int,int)),
                            q, SLOT(_q_itemsInserted(int,int)));
        QObject::disconnect(previous, SIGNAL(itemsRemoved(int,int)),
                            q, SLOT(_q_itemsRemoved(int,int)));
        QObject::disconnect(previous, SIGNAL(itemsMoved(int,int,int)),
                            q, SLOT(_q_itemsMoved(int,int,int)));
    }
    if (current) {
        QObject::connect(current, SIGNAL(itemsInserted(int,int)),
                         q, SLOT(_q_itemsInserted(int,int)));
        QObject::connect(current, SIGNAL(itemsRemoved(int,int)),
                         q, SLOT(_q_itemsRemoved(int,int)));
        QObject::connect(current, SIGNAL(itemsMoved(int,int,int)),
                         q, SLOT(_q_itemsMoved(int,int,int)));
    }
}

void QtListWidgetNGPrivate::_q_selectionManagerChanged(QtListSelectionManager *current, QtListSelectionManager *previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
}

void QtListWidgetNGPrivate::_q_viewChanged(QtGraphicsListView *current, QtGraphicsListView *previous)
{
    Q_Q(QtListWidgetNG);
    if (previous) {
        if (scene)
            scene->removeItem(previous);
        QObject::disconnect(previous, SIGNAL(firstIndexChanged(int)), q, SLOT(_q_firstIndexChanged(int)));
    }
    if (current) {
        if (scene) {
            scene->addItem(current);
            scene->setActiveWindow(current);
        }
        current->grabKeyboard();
        QObject::connect(current, SIGNAL(firstIndexChanged(int)), q, SLOT(_q_firstIndexChanged(int)));
    }
    //_q_updateGeometries();
}

void QtListWidgetNGPrivate::_q_firstIndexChanged(int index)
{
    Q_Q(QtListWidgetNG);
    Q_ASSERT(false);
    if (q->verticalScrollBar()->value() != index)
        q->verticalScrollBar()->setValue(index);
}

void QtListWidgetNGPrivate::_q_updateGeometries()
{
    Q_Q(QtListWidgetNG);

    QRect rect = q->viewport()->geometry();

    controller->view()->setGeometry(QRect(0, 0, rect.width(), rect.height()));
    controller->view()->doLayout();

    static_cast<QtGraphicsViewport*>(q->viewport())->scene()->setSceneRect(0, 0, rect.width(), rect.height());
    static_cast<QtGraphicsViewport*>(q->viewport())->viewport()->setGeometry(QRect(0, 0, rect.width(), rect.height()));

    QScrollBar *scrollbar = q->verticalScrollBar(); // ### check scrollbar policy

    qreal maximum = 0;
    qreal page = controller->pageStepValue(&maximum);

    scrollbar->setPageStep(page);
    scrollbar->setMaximum(maximum);
    //qDebug() << "scrollbar" << scrollbar->value() << scrollbar->maximum() << scrollbar->pageStep();
}

/*!
  \internal
 */
void QtListWidgetNGPrivate::initialize()
{
    Q_Q(QtListWidgetNG);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (!scene)
        scene = new QGraphicsScene(q);

    q->setViewport(new QtGraphicsViewport(scene));

    if (!controller)
        controller = new QtListController(q);
    if (!controller->model())
        controller->setModel(new QtListDefaultModel(controller));
    if (!controller->selectionManager())
        controller->setSelectionManager(new QtListSelectionManager(controller));
    if (!controller->view()) {
        controller->setView(new QtGraphicsListView);
        controller->view()->setParent(controller);
    }
    controller->setWheelEnabled(false); // the widget will handle it
    _q_controllerChanged(controller, 0);
}

// QtListWidgetNG

/*!
  \class QtListWidgetNG
  \brief The QtListWidgetNG class provides an item-based list widget.

  QtListWidgetNG is a QWidget sub-class that uses default constructed
  QtListController, QtListDefaultModel and QtGraphicsListView instances
  to show a list of items.

  A QtListWidgetNG can be also be constructed with a custom controller,
  view and/or model.
*/

/*!
    Creates a new QtListWidgetNG with the given \a parent.
 */
QtListWidgetNG::QtListWidgetNG(QWidget *parent)
    : QAbstractScrollArea(parent), d_ptr(new QtListWidgetNGPrivate)
{
    Q_D(QtListWidgetNG);
    d->q_ptr = this;
    d->initialize();
}

/*!
  Creates a new QtListWidgetNG with the given \a controller and \a parent.
 */
QtListWidgetNG::QtListWidgetNG(QtListController *controller, QWidget *parent)
    : QAbstractScrollArea(parent), d_ptr(new QtListWidgetNGPrivate)
{
    Q_D(QtListWidgetNG);
    Q_ASSERT(controller);
    d->q_ptr = this;
    d->controller = controller;
    controller->setParent(this);
    d->initialize();
}

/*!
  Destructor.
 */
QtListWidgetNG::~QtListWidgetNG()
{
    delete d_ptr;
}

/*!
   Returns the controller used by the widget.

  \sa defaultModel()
 */
QtListController *QtListWidgetNG::controller() const
{
    Q_D(const QtListWidgetNG);
    return d->controller;
}

/*!
  Returns the default model.

  The QtListWidgetNG is created with a default item model to hold the data
  shown by this widget. If this list is created with a custom controller
  with a QtListDefaultModel, the pointer to the custom model is returned.
  Otherwise null is returned.

  \sa controller()
 */
QtListDefaultModel *QtListWidgetNG::defaultModel() const
{
    Q_D(const QtListWidgetNG);
    return qobject_cast<QtListDefaultModel*>(d->controller->model());
}

/*!
  \reimp
 */
bool QtListWidgetNG::viewportEvent(QEvent *event)
{
    Q_D(QtListWidgetNG);
    bool b = QAbstractScrollArea::viewportEvent(event);
    switch (event->type()) {
    case QEvent::Resize:
        d->_q_updateGeometries();
        break;
    default:
        break;
    }
    return b;
}

/*!
  \reimp
*/
void QtListWidgetNG::scrollContentsBy(int dx, int dy)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);
    // do nothing
}

QT_END_NAMESPACE

#include "moc_qlistwidgetng.cpp"
