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

#include "qlistcontroller.h"
#include "qlistcontroller_p.h"
#include "qlistmodelinterface.h"
#include "qlistselectionmanager.h"
#include "qgraphicslistview.h"

#include <qtransform.h>
#include <qevent.h>
#include <qgraphicssceneevent.h>
#include <qpropertyanimation.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

// QtListControllerPrivate

/*!
  \internal
 */
QtListControllerPrivate::QtListControllerPrivate()
    : q_ptr(0),
    model(0),
    selectionManager(0),
    view(0),
    behavior(QtListController::SingleSelection),
    animation(0),
    firstIndex(0),
    scrollPerItem(false), // ### for now
    wheelEnabled(true)
{
}

/*!
  \internal
 */
QtListControllerPrivate::~QtListControllerPrivate()
{
}

/*!
  \internal
 */
void QtListControllerPrivate::_q_modelDestroyed()
{
    model = 0;
}

/*!
  \internal
 */
void QtListControllerPrivate::_q_viewDestroyed()
{
    view = 0;
}

/*!
 \internal
*/
void QtListControllerPrivate::_q_selectionsDestroyed()
{
    selectionManager = 0;
}

/*!
 \internal
*/
void QtListControllerPrivate::_q_firstIndexChanged(int index)
{
    Q_Q(QtListController);
    if (scrollPerItem && view)
        emit q->scrollValueChanged(static_cast<qreal>(index));
}

/*!
 \internal
*/
void QtListControllerPrivate::_q_offsetChanged(qreal offset)
{
    Q_Q(QtListController);
    if (!scrollPerItem && view)
        emit q->scrollValueChanged(offset);
}

/*!
  \internal
 */
void QtListControllerPrivate::_q_currentChanged(int current, int previous)
{
    Q_UNUSED(previous);
    if (!view || !model)
        return;
    // ensure current is visible
    if (scrollPerItem)
        view->setFirstIndex(view->firstIndexToEnsureIndexIsVisible(current));
    else
        view->setOffset(view->offsetToEnsureIndexIsVisible(current));
}

/*!
 \internal
*/
void QtListControllerPrivate::_q_animationFinished()
{
    view->setOffset(0);
    view->setFirstIndex(firstIndex);
}

/*!
  \internal
*/
void QtListControllerPrivate::animateFirstIndex(int index)
{
    // animate the offset when scrolling per item
    int previous = view->firstIndex();
    if (animation && index != previous) {
        animation->stop();
        if (index > previous) { // scrolling down
            qreal offset = 0;
            for (int i = previous; i < index; ++i)
                offset += view->itemGeometry(i).height();
            animation->setStartValue(view->offset());
            animation->setEndValue(offset);
        } else { // scrolling up
            qreal offset = view->offset();
            view->setFirstIndex(index);
            for (int i = index; i < previous; ++i)
                offset += view->itemGeometry(i).height();
            view->setOffset(offset);
            animation->setStartValue(offset);
            animation->setEndValue(0);
        }
        firstIndex = index;
        animation->start();
    }
}

// QtListController

/*!
    \class QtListController
    \brief provides a controller for list model and view objects.

    QtListController represents a controller in the model-view-controller (MFC)
    design-pattern.

    It is used together with a sub-class of QtListModelInterface and QtGraphicsListView.

    QtListController is responsible for handling user input events and make the requested
    changes to the view, model, or selections.

    For example, when the user clicks an item in the view, it is QtListController's
    responsibility to change selection state of that item.

    QtListWidgetNG contains a QtListController, as well as a QtListDefaultModel
    and a QtGraphicsListView.
*/

/*!
    \enum SelectionBehavior
    \brief controls how selection is done in the list
*/

/*!
    Constructs a list controller with the given \a parent.
*/
QtListController::QtListController(QObject *parent)
    : QObject(parent), d_ptr(new QtListControllerPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
    \internal
*/
QtListController::QtListController(QtListControllerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys the list controller.
*/
QtListController::~QtListController()
{
    delete d_ptr;
}

/*!
    Returns a pointer to the model used by the controller. If no model was set
    on the controller, the returned value will be 0.

    \sa setModel()
*/
QtListModelInterface *QtListController::model() const
{
    Q_D(const QtListController);
    return d->model;
}

/*!
  Sets the model used by the controller to be the given \a model.

  \sa model()
 */
void QtListController::setModel(QtListModelInterface *model)
{
    Q_D(QtListController);
    if (d->model == model)
        return;

    QtListModelInterface *old = d->model;

    if (d->model)
        disconnect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));

    d->model = model;

    if (d->model)
        connect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));

    if (d->selectionManager)
        d->selectionManager->setModel(d->model);

    if (d->view)
        d->view->setModel(d->model);

    emit modelChanged(model, old);
}

/*!
    Returns a pointer to the selection manager used by the contoller. If no
    selection manager is set, the returned value will be 0.
*/
QtListSelectionManager *QtListController::selectionManager() const
{
    Q_D(const QtListController);
    return d->selectionManager;
}

/*!
    Sets the selection manager used by the controller to the given
    \a selectionManager.
*/
void QtListController::setSelectionManager(QtListSelectionManager *selectionManager)
{
    Q_D(QtListController);
    if (d->selectionManager == selectionManager)
        return;
    if (d->selectionManager) {
        disconnect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));
        disconnect(d->selectionManager, SIGNAL(currentChanged(int,int)), this, SLOT(_q_currentChanged(int,int)));
    }

    QtListSelectionManager *old = d->selectionManager;
    d->selectionManager = selectionManager;

    if (d->selectionManager) {
        d->selectionManager->setModel(d->model);
        connect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));
        connect(d->selectionManager, SIGNAL(currentChanged(int,int)), this, SLOT(_q_currentChanged(int,int)));
    }

    if (d->view)
        d->view->setSelectionManager(d->selectionManager);

    emit selectionManagerChanged(selectionManager, old);
}

/*!
    Returns a pointer the list view. If no view is set, the returned value
    will be 0.

    \sa setView()
*/
QtGraphicsListView * QtListController::view() const
{
    Q_D(const QtListController);
    return d->view;
}

/*!
    Sets the view used by the controller to the given \a view.

    \sa view()
*/
void QtListController::setView(QtGraphicsListView *view)
{
    Q_D(QtListController);
    if (d->view == view)
        return;

    QtGraphicsListView *old = d->view;

    if (!d->animation) {
        d->animation = new QPropertyAnimation(0, "offset", this);
        d->animation->setDuration(100);
        connect(d->animation, SIGNAL(finished()), this, SLOT(_q_animationFinished()));
    }
    d->animation->setTargetObject(view);

    if (d->view) {
        disconnect(d->view, SIGNAL(destroyed()), this, SLOT(_q_viewDestroyed()));
        disconnect(d->view, SIGNAL(firstIndexChanged(int)), this, SLOT(_q_firstIndexChanged(int)));
        disconnect(d->view, SIGNAL(offsetChanged(qreal)), this, SLOT(_q_offsetChanged(qreal)));
    }

    d->view = view;

    if (d->view) {
        connect(d->view, SIGNAL(destroyed()), this, SLOT(_q_viewDestroyed()));
        connect(d->view, SIGNAL(firstIndexChanged(int)), this, SLOT(_q_firstIndexChanged(int)));
        connect(d->view, SIGNAL(offsetChanged(qreal)), this, SLOT(_q_offsetChanged(qreal)));
        d->view->setModel(d->model);
        d->view->setSelectionManager(d->selectionManager);
        d->view->setController(this);
    }

    emit viewChanged(view, old);
}

/*!
 */
QtListController::SelectionBehavior QtListController::selectionBehavior() const
{
    Q_D(const QtListController);
    return d->behavior;
}

/*!
 */
void QtListController::setSelectionBehavior(QtListController::SelectionBehavior behavior)
{
    Q_D(QtListController);
    d->behavior = behavior;
}

/*!
 */
bool QtListController::isWheelEnabled() const
{
    Q_D(const QtListController);
    return d->wheelEnabled;
}

/*!
 */
void QtListController::setWheelEnabled(bool enable)
{
    Q_D(QtListController);
    d->wheelEnabled = enable;
}


/*!
  Returns the scroll value of the view.
  This value depends on the controller scroll mode and the
  view orientation.

  \sa view()
 */
qreal QtListController::scrollValue() const
{
    Q_D(const QtListController);
    if (!d->view)
        return 0;
    if (d->scrollPerItem)
        return d->view->firstIndex();
    return d->view->offset();
}

/*!
  Sets the scroll value on the view.
  The property this value is set on depends on the controller
  scroll mode and the view orientation.

  \sa view()
 */
void QtListController::setScrollValue(qreal value)
{
    Q_D(QtListController);
    if (!d->view)
        return;
    if (d->scrollPerItem) {
        int index = static_cast<int>(value);
        if (d->view->firstIndex() == index)
            return;
        //d->animateFirstIndex(index);
        d->view->setFirstIndex(qMax(0, index)); // ### workaround for QScrollBar bug
    } else {
        if (d->view->offset() == value)
            return;
        d->view->setOffset(qMax(0., value)); // ### workaround for QScrollBar bug
    }
    emit scrollValueChanged(value);
}

/*!
 */
void QtListController::setScrollValue(int value)
{
    setScrollValue(qreal(value));
}


/*!
  Returns the page step value of the view.
  This value depends on the controller scroll mode and the
  view orientation.

  \sa view()
 */
qreal QtListController::pageStepValue(qreal *maximumScrollValue) const
{
    Q_D(const QtListController);
    if (!d->view)
        return 0;
    if (d->scrollPerItem) {
        const int count = d->model ? d->model->count() : 0;
        const int maximum = d->view->maximumFirstIndex();
        if (maximumScrollValue)
            *maximumScrollValue = maximum;
        return count - maximum;
    }
    if (maximumScrollValue)
        *maximumScrollValue = d->view->maximumOffset();
    const QSizeF size = d->view->size();
    return (d->view->orientation() == Qt::Vertical ? size.height() : size.width());
}


/*!
  Returns the maximum scroll value of the view.
  This value depends on the controller scroll mode and the
  view orientation.

  \sa view()
 */
qreal QtListController::maximumScrollValue() const
{
    Q_D(const QtListController);
    if (!d->view)
        return 0;
    if (d->scrollPerItem)
        return d->view->maximumFirstIndex();
    return d->view->maximumOffset();
}

/*!
 */
bool QtListController::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    return false;
}

/*!
 */
bool QtListController::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    return false;
}

/*!
 */
bool QtListController::keyPressEvent(QKeyEvent *event)
{
    Q_D(QtListController);
    if (d->model && d->selectionManager) {
        int index = d->selectionManager->currentItem();
        switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_Up:
            if (--index < 0)
                index = d->model->count() - 1;
            break;
        case Qt::Key_Right:
        case Qt::Key_Down:
            if (++index >= d->model->count())
                index = 0;
            break;
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            break;
        case Qt::Key_Space:
            if (index >= 0 && index < d->model->count())
                emit itemActivated(index);
            return true;
        default:
            break;
        }
        if (d->selectionManager->currentItem() != index) {
            if (event->modifiers() & Qt::ControlModifier) {
                d->selectionManager->setCurrentItem(index);
                switch (d->behavior) {
                case NoSelection:
                    return true;
                case SingleSelection:
                    if (d->selectionManager->isAnchoredSelectionActive())
                        d->selectionManager->setAnchorItem(index);
                    else
                        d->selectionManager->beginAnchoredSelection(index, QtListSelectionManager::Toggle);
                    return true;
                case MultiSelection:
                    d->selectionManager->beginAnchoredSelection(d->selectionManager->anchorItem(), QtListSelectionManager::Toggle);
                    return true;
                }
            } else if (event->modifiers() & Qt::ShiftModifier) {
                d->selectionManager->setCurrentItem(index);
                switch (d->behavior) {
                    case NoSelection:
                        return true;
                    case SingleSelection:
                        if (d->selectionManager->isAnchoredSelectionActive())
                            d->selectionManager->setAnchorItem(index);
                        else
                            d->selectionManager->beginAnchoredSelection(index, QtListSelectionManager::Select);
                        return true;
                    case MultiSelection:
                        d->selectionManager->beginAnchoredSelection(d->selectionManager->anchorItem(), QtListSelectionManager::Select);
                        return true;
                }
            } else { // no modifier
                d->selectionManager->endAnchoredSelection();
                d->selectionManager->clearSelections();
                d->selectionManager->setCurrentItem(index);
                switch (d->behavior) {
                    case NoSelection:
                        return true;
                    case SingleSelection:
                    case MultiSelection:
                        d->selectionManager->beginAnchoredSelection(index, QtListSelectionManager::Select);
                        return true;
                }
            }
        }
    }
    return false;
}

/*!
 */
bool QtListController::inputMethodEvent(QInputMethodEvent *event)
{
    Q_UNUSED(event);
    return false;
}

/*!
 */
bool QtListController::mousePressEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtListController);
    if (d->selectionManager && d->view && event->buttons() & Qt::LeftButton) {
        const int index = d->view->itemAt(transform.map(event->pos()));
        d->selectionManager->setCurrentItem(index);
        if (event->modifiers() & Qt::ControlModifier) {
            switch (d->behavior) {
            case NoSelection:
                return true;
            case SingleSelection:
            case MultiSelection:
                d->selectionManager->beginAnchoredSelection(index, QtListSelectionManager::Toggle);
                return true;
            }
        } else if (event->modifiers() & Qt::ShiftModifier) {
            d->selectionManager->clearSelections();
            switch (d->behavior) {
            case NoSelection:
                return true;
            case SingleSelection:
                d->selectionManager->beginAnchoredSelection(index);
                return true;
            case MultiSelection:
                d->selectionManager->beginAnchoredSelection(d->selectionManager->anchorItem());
                return true;
            }
        } else { // no modifiers
            d->selectionManager->clearSelections();
            switch (d->behavior) {
            case NoSelection:
                return true;
            case SingleSelection:
            case MultiSelection:
                d->selectionManager->beginAnchoredSelection(index);
                return true;
            }
        }
    }
    return false;
}

/*!
 */
bool QtListController::mouseMoveEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtListController);
    if (d->selectionManager && d->view && event->buttons() & Qt::LeftButton) {
        const int index = d->view->itemAt(transform.map(event->pos()));
        if (d->selectionManager->currentItem() != index) {
            d->selectionManager->setCurrentItem(index);
            switch (d->behavior) {
            case NoSelection:
                return true;
            case SingleSelection:
                d->selectionManager->setAnchorItem(index);
                return true;
            case MultiSelection:
                return true;
            }
        }
    }
    return false;
}

/*!
 */
bool QtListController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtListController);
    if (d->selectionManager && d->view && event->button() == Qt::LeftButton) {
        const int index = d->view->itemAt(transform.map(event->pos()));
        if (d->selectionManager->currentItem() != index)
            d->selectionManager->setCurrentItem(index);
        switch (d->behavior) {
        case NoSelection:
            break;
        case SingleSelection:
            d->selectionManager->setAnchorItem(index);
        case MultiSelection:
            d->selectionManager->endAnchoredSelection();
            break;
        }
        emit itemClicked(index, Qt::LeftButton);
        emit itemActivated(index); // activates on single-click
        return true;
    }
    return false;
}

/*!
 */
bool QtListController::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::dragEnterEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::dragLeaveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::dragMoveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::dropEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::hoverEnterEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::hoverMoveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::hoverLeaveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtListController::wheelEvent(QGraphicsSceneWheelEvent *event, const QTransform &transform)
{
    Q_UNUSED(transform);
    Q_D(QtListController);
    return false;
    if (!d->view || !d->wheelEnabled)
        return false;
    if (d->scrollPerItem) {
        const int index = d->view->firstIndex() - event->delta() / 120;
        d->view->setFirstIndex(qBound<qreal>(0, index, d->view->maximumFirstIndex()));
    } else {
        const qreal offset = d->view->offset() - event->delta() / 120;
        d->view->setOffset(qBound<qreal>(.0, offset, d->view->maximumOffset()));
    }
    return true;
}

/*!
 */
bool QtListController::resizeEvent(QGraphicsSceneResizeEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    Q_D(QtListController);
    if (!d->view)
        return false;
    d->view->updateLayout();
    return true;
}

/*!
  Processes the given \a event, performing the appropriate changes to the view,
  selections or model. The given \a transform is used to transform coordinates
  and geometries if provided by the event.
  Returns true if the event was recognized and processed, otherwise returns false.
 */
bool QtListController::processEvent(QEvent *event, const QTransform &transform)
{
    if (!event)
        return false;
    switch(event->type()) {        
//  case QEvent::FocusIn:
//  case QEvent::FocusOut:
//      return focusEvent(static_cast<QFocusEvent*>(event));
    case QEvent::KeyPress:
        return keyPressEvent(static_cast<QKeyEvent*>(event));
    case QEvent::InputMethod:
        return inputMethodEvent(static_cast<QInputMethodEvent*>(event));
    case QEvent::GraphicsSceneMousePress:
        return mousePressEvent(static_cast<QGraphicsSceneMouseEvent*>(event), QTransform());
    case QEvent::GraphicsSceneMouseMove:
        return mouseMoveEvent(static_cast<QGraphicsSceneMouseEvent*>(event), QTransform());
    case QEvent::GraphicsSceneMouseRelease:
        return mouseReleaseEvent(static_cast<QGraphicsSceneMouseEvent*>(event), QTransform());
#ifndef QT_NO_WHEELEVENT
    case QEvent::GraphicsSceneWheel:
         return wheelEvent(static_cast<QGraphicsSceneWheelEvent*>(event), QTransform());
#endif
    case QEvent::GraphicsSceneDragEnter:
        return dragEnterEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), QTransform());
    case QEvent::GraphicsSceneDragLeave:
        return dragLeaveEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), QTransform());
    case QEvent::GraphicsSceneDragMove:
        return dragMoveEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), QTransform());
    case QEvent::GraphicsSceneDrop:
        return dropEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), QTransform());
    case QEvent::GraphicsSceneHoverEnter:
        return hoverEnterEvent(static_cast<QGraphicsSceneHoverEvent*>(event), QTransform());
    case QEvent::GraphicsSceneHoverMove:
        return hoverMoveEvent(static_cast<QGraphicsSceneHoverEvent*>(event), QTransform());
    case QEvent::GraphicsSceneHoverLeave:
        return hoverLeaveEvent(static_cast<QGraphicsSceneHoverEvent*>(event), QTransform());
    case QEvent::GraphicsSceneResize:
        return resizeEvent(static_cast<QGraphicsSceneResizeEvent*>(event), transform);
    default:
        break;
    }
    return false;
}

QT_END_NAMESPACE

#include "moc_qlistcontroller.cpp"
