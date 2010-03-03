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

#include "qtreecontroller.h"
#include "qtreecontroller_p.h"
#include "qgraphicsheader.h"
#include "qtreemodelinterface.h"
#include "qtreeselectionmanager.h"
#include "qgraphicstreeview.h"

#include <qevent.h>
#include <qgraphicssceneevent.h>
#include <qtransform.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

// QTreeHeaderDataProvider

class QTreeHeaderDataProvider : public QtGraphicsHeaderDataProvider
{
public:
    QTreeHeaderDataProvider() : model(0) {}
    QHash<QByteArray,QVariant> data(int logicalIndex, const QList<QByteArray> &roles) const
    {
        // ### call the model - temporary implementation
        QHash<QByteArray,QVariant> hash;
        for (int i = 0; i < roles.count(); ++i)
            if (roles.at(i) == "DisplayRole") // ### FIXME
                hash.insert(roles.at(i), logicalIndex + 1);
        return hash;
    }
    QtTreeModelBase *model;
};

// QtTreeControllerPrivate

QtTreeControllerPrivate::QtTreeControllerPrivate()
    : view(0),
      model(0),
      selectionManager(0),
      header(0),
      headerDataProvider(new QTreeHeaderDataProvider),
      scrollPerItem(true),
      scrollPerSection(false),
      currentIndex(-1),
      anchorIndex(-1)
{
}

QtTreeControllerPrivate::~QtTreeControllerPrivate()
{
    delete headerDataProvider;
}

void QtTreeControllerPrivate::_q_viewDestroyed()
{
    view = 0;
}

void QtTreeControllerPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtTreeControllerPrivate::_q_selectionsDestroyed()
{
    selectionManager = 0;
}

void QtTreeControllerPrivate::_q_headerDestroyed()
{
    header = 0;
}

void QtTreeControllerPrivate::_q_firstIndexChanged(int index)
{
    Q_Q(QtTreeController);
    if (scrollPerItem)
        emit q->verticalScrollValueChanged(static_cast<qreal>(index));
}

void QtTreeControllerPrivate::_q_firstSectionChanged(int section)
{
    Q_Q(QtTreeController);
    if (scrollPerSection)
        emit q->horizontalScrollValueChanged(static_cast<qreal>(section));
}

void QtTreeControllerPrivate::_q_verticalOffsetChanged(qreal offset)
{
    Q_Q(QtTreeController);
    if (!scrollPerItem)
        emit q->verticalScrollValueChanged(offset);

}

void QtTreeControllerPrivate::_q_horizontalOffsetChanged(qreal offset)
{
    Q_Q(QtTreeController);
    if (scrollPerSection)
        emit q->horizontalScrollValueChanged(offset);
}

QtTreeSelection QtTreeControllerPrivate::anchoredSelection() const
{
    QtTreeSelection selection;
    if (view && selectionManager && currentIndex != -1 && anchorIndex != -1) {
        QtTreeModelBase::iterator_base it = currentIndex < anchorIndex ? selectionManager->currentItem() : selectionManager->anchorItem();
        QtTreeModelBase::iterator_base end = currentIndex > anchorIndex ? selectionManager->currentItem() : selectionManager->anchorItem();
        while (it.isValid() && end.isValid() && it != end) {
            selection << it;
            view->nextItem(it);
        }
        selection << end;
    }
    return selection;
}

// QtTreeController

/*!
    \class QtTreeController
    \brief provides a controller for tree model and view objects.

    QtTreeController represents a controller in the model-view-controller (MFC)
    design-pattern.

    It is used together with a sub-class of QtTreeModelInterface and QtGraphicsTreeView.

    QtTreeController is responsible for handling user input events and make the requested
    changes to the view, model, or selections.

    For example, when the user clicks an item in the view, it is QtTreeController's
    responsibility to change selection state of that item.

    QtTreeWidgetNG contains a QtTreeController, as well as a QtTreeDefaultModel
    and a QtGraphicsTreeView.
*/

/*!
 */
QtTreeController::QtTreeController(QObject *parent)
    : QObject(parent), d_ptr(new QtTreeControllerPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTreeController::QtTreeController(QtTreeControllerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTreeController::~QtTreeController()
{
    delete d_ptr;
}

/*!
 */
QtGraphicsTreeView *QtTreeController::view() const
{
    Q_D(const QtTreeController);
    return d->view;
}

/*!
 */
void QtTreeController::setView(QtGraphicsTreeView *view)
{
    Q_D(QtTreeController);
    if (d->view == view)
        return;
    QtGraphicsTreeView *old = d->view;
    if (d->view) {
        disconnect(d->view, SIGNAL(destroyed()), this, SLOT(_q_viewDestroyed()));
        disconnect(d->view, SIGNAL(firstIndexChanged(int)), this, SLOT(_q_firstIndexChanged(int)));
        //disconnect(d->view, SIGNAL(firstSectionChanged(int)), this, SLOT(_q_firstSectionChanged(int)));
        disconnect(d->view, SIGNAL(verticalOffsetChanged(qreal)), this, SLOT(_q_verticalOffsetChanged(qreal)));
        disconnect(d->view, SIGNAL(horizontalOffsetChanged(qreal)), this, SLOT(_q_horizontalOffsetChanged(qreal)));
    }
    d->view = view;
    if (d->view) {
        connect(d->view, SIGNAL(destroyed()), this, SLOT(_q_viewDestroyed()));
        connect(d->view, SIGNAL(firstIndexChanged(int)), this, SLOT(_q_firstIndexChanged(int)));
        //disconnect(d->view, SIGNAL(firstSectionChanged(int)), this, SLOT(_q_firstSectionChanged(int)));
        connect(d->view, SIGNAL(verticalOffsetChanged(qreal)), this, SLOT(_q_verticalOffsetChanged(qreal)));
        connect(d->view, SIGNAL(horizontalOffsetChanged(qreal)), this, SLOT(_q_horizontalOffsetChanged(qreal)));
        d->view->setModel(d->model);
        d->view->setSelectionManager(d->selectionManager);
        d->view->setController(this);
    }
    emit viewChanged(view, old);
}

/*!
 */
QtTreeModelBase *QtTreeController::model() const
{
    Q_D(const QtTreeController);
    return d->model;
}

/*!
 */
void QtTreeController::setModel(QtTreeModelBase *model)
{
    Q_D(QtTreeController);
    if (d->model == model)
        return;

    QtTreeModelBase *old = d->model;

    if (d->model)
        disconnect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));

    d->model = model;
    d->headerDataProvider->model = model;

    if (d->model)
        connect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));

    if (d->view)
        d->view->setModel(d->model);

    emit modelChanged(model, old);
}

/*!
 */
QtTreeSelectionManager *QtTreeController::selectionManager() const
{
    Q_D(const QtTreeController);
    return d->selectionManager;
}

/*!
 */
void QtTreeController::setSelectionManager(QtTreeSelectionManager *selectionManager)
{
    Q_D(QtTreeController);
    if (d->selectionManager == selectionManager)
        return;

    QtTreeSelectionManager *old = d->selectionManager;

    if (d->selectionManager)
        disconnect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));

    d->selectionManager = selectionManager;

    if (d->selectionManager)
        connect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));

    if (d->view)
        d->view->setSelectionManager(d->selectionManager);


    emit selectionManagerChanged(selectionManager, old);
}

/*!
 */
QtGraphicsHeader *QtTreeController::header() const
{
    Q_D(const QtTreeController);
    return d->header;
}

/*!
 */
void QtTreeController::setHeader(QtGraphicsHeader *header)
{
    Q_D(QtTreeController);
    if (d->header == header)
        return;
    if (d->header) {
        disconnect(d->header, SIGNAL(destroyed()), this, SLOT(_q_headerDestroyed()));
    }
    d->header = header;
    if (d->header) {
        connect(d->header, SIGNAL(destroyed()), this, SLOT(_q_headerDestroyed()));
        d->header->setOrientation(Qt::Horizontal);
        d->header->setSectionCount(d->model ? d->model->begin().columnCount() : 1);
        d->header->setDataProvider(d->headerDataProvider);
    }
}

/*!
 */
qreal QtTreeController::verticalScrollValue() const
{
    Q_D(const QtTreeController);
    if (!d->view)
        return 0;
    if (d->scrollPerItem)
        return d->view->firstIndex();
    return d->view->verticalOffset();
}

/*!
 */
void QtTreeController::setVerticalScrollValue(qreal value)
{
    Q_D(QtTreeController);
    if (!d->view)
        return;
    if (d->scrollPerItem)
        d->view->setFirstIndex(value);
    else
        d->view->setVerticalOffset(value);
}

/*!
 */
void QtTreeController::setVerticalScrollValue(int value)
{
    setVerticalScrollValue(qreal(value));
}

/*!
 */
qreal QtTreeController::horizontalScrollValue() const
{
    Q_D(const QtTreeController);
    if (!d->view)
        return 0;
    if (d->scrollPerSection)
        return d->view->firstSection();
    return d->view->horizontalOffset();
}

/*!
 */
void QtTreeController::setHorizontalScrollValue(int value)
{
    setHorizontalScrollValue(qreal(value));
}

/*!
 */
void QtTreeController::setHorizontalScrollValue(qreal value)
{
    Q_D(QtTreeController);
    if (!d->view)
        return;
    if (d->scrollPerSection)
        if (d->view->header())
            d->view->header()->setFirstSection(value);
    else
        d->view->setHorizontalOffset(value);
}

/*!
 */
qreal QtTreeController::verticalPageStepValue(qreal *maximumVerticalScrollValue) const
{
    Q_D(const QtTreeController);
    if (!d->view)
        return 0;
    if (d->scrollPerItem) {
        int count = 0;
        int maximum = d->view->maximumFirstIndex(&count);
        if (maximumVerticalScrollValue)
            *maximumVerticalScrollValue = static_cast<qreal>(maximum);
        return count - maximum;
    }
    if (maximumVerticalScrollValue)
        *maximumVerticalScrollValue = d->view->maximumVerticalOffset();
    return d->view->size().height();
}

/*!
 */
qreal QtTreeController::horizontalPageStepValue(qreal *maximumHorizontalScrollValue) const
{
    Q_D(const QtTreeController);
    if (!d->view)
        return 0;
    if (d->scrollPerSection) {
        int count = d->view->header() ? d->view->header()->sectionCount() : 1;
        int maximum = maximum > 0 ? maximum : d->view->maximumFirstSection();
        if (maximumHorizontalScrollValue)
            *maximumHorizontalScrollValue = static_cast<qreal>(maximum);
        return count - maximum;
    }
    if (maximumHorizontalScrollValue)
        *maximumHorizontalScrollValue = d->view->maximumHorizontalOffset();
    return d->view->size().height();
}

/*!
 */
qreal QtTreeController::maximumVerticalScrollValue() const
{
    Q_D(const QtTreeController);
    if (!d->view)
        return 0;
    if (d->scrollPerItem)
        return d->view->maximumFirstIndex();
    return d->view->maximumVerticalOffset();
}

/*!
 */
qreal QtTreeController::maximumHorizontalScrollValue() const
{
    Q_D(const QtTreeController);
    if (!d->view)
        return 0;
    if (d->scrollPerSection)
        return d->view->maximumFirstSection();
    return d->view->maximumHorizontalOffset();
}

/*!
 */
bool QtTreeController::keyPressEvent(QKeyEvent *event)
{
    Q_D(QtTreeController);
    if (d->view && d->selectionManager) {
        QtTreeModelBase::iterator_base it = d->selectionManager->currentItem();
        switch (event->key()) {
        case Qt::Key_Left:
            d->view->setItemExpanded(it, false);
            return true;
        case Qt::Key_Right:
            d->view->setItemExpanded(it, true);
            return true;
        case Qt::Key_Down:
            d->view->nextItem(it);
            break;
        case Qt::Key_Up:
            d->view->previousItem(it);
            break;
        default:
            break;
        }
        if (d->selectionManager->currentItem() != it) {
            if (event->modifiers() & Qt::ShiftModifier) {
                //d->selectionManager->beginAnchoredSelection(d->selectionManager->anchorItem());
                d->selectionManager->setCurrentItem(it);
                d->currentIndex = d->view->indexOf(it);
                d->selectionManager->setSelected(d->anchoredSelection());
            } else {
                //d->selectionManager->endAnchoredSelection();
                d->selectionManager->clearSelections();
                d->selectionManager->setCurrentItem(it);
                d->selectionManager->setAnchorItem(it);
                d->currentIndex = d->view->indexOf(it);
                d->selectionManager->setSelected(QtTreeSelection() << it);
            }
            return true;
        }
    }
    return false;
}

/*!
 */
bool QtTreeController::inputMethodEvent(QInputMethodEvent *event)
{
    Q_UNUSED(event);
    return false;
}

/*!
 */
bool QtTreeController::mousePressEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtTreeController);
    const QPointF pos = transform.map(event->pos());
    if (d->view && event->buttons() & Qt::LeftButton) {
        int index = -1;
        QRectF rect; // optimization; let itemAt get the rect and index
        QtTreeModelBase::iterator_base it = d->view->itemAt(pos, &index, &rect);
        if (it.hasChildren() && d->view->disclosureRect(rect).contains(pos)) {
            if (d->view->isItemExpanded(it))
                d->view->setItemExpanded(it, false);
            else
                d->view->setItemExpanded(it, true);
            return true;
        }
        if (d->selectionManager) {
            if (event->modifiers() & Qt::ControlModifier) {
                //d->selectionManager->beginAnchoredSelection(it, QtTreeSelectionManager::Toggle);
                d->selectionManager->setAnchorSelectionMode(QtTreeSelectionManager::Toggle);
                d->selectionManager->setAnchorItem(it);
                d->selectionManager->setCurrentItem(it);
                d->anchorIndex = index;
                d->currentIndex = index;
                d->selectionManager->setAnchorSelection(QtTreeSelection() << it);
            } else if (event->modifiers() & Qt::ShiftModifier) {
                d->selectionManager->clearSelections();
                //d->selectionManager->beginAnchoredSelection(d->selectionManager->anchorItem());
                d->selectionManager->setCurrentItem(it);
                d->currentIndex = index;
                d->selectionManager->setAnchorSelection(d->anchoredSelection());
            } else { // no modifiers
                d->selectionManager->clearSelections();
                //d->selectionManager->beginAnchoredSelection(it);
                d->selectionManager->setAnchorItem(it);
                d->selectionManager->setCurrentItem(it);
                d->anchorIndex = index;
                d->currentIndex = index;
                d->selectionManager->setAnchorSelection(QtTreeSelection() << it);
            }
            return true;
        }
    }
    return false;
}

/*!
 */
bool QtTreeController::mouseMoveEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtTreeController);
    QPointF pos = transform.map(event->pos());
    if (d->view && d->selectionManager && event->buttons() & Qt::LeftButton) {
        int index = -1;
        QtTreeModelBase::iterator_base it = d->view->itemAt(pos, &index);
        if (d->selectionManager->currentItem() != it) {
            d->selectionManager->setCurrentItem(it);
            d->currentIndex = index;
            d->selectionManager->setAnchorSelection(d->anchoredSelection());
            return true;
        }
    }
    return false;
}

/*!
 */
bool QtTreeController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtTreeController);
    QPointF pos = transform.map(event->pos());
    if (d->view && event->button() == Qt::LeftButton) {
        int index = -1;
        QtTreeModelBase::iterator_base it = d->view->itemAt(pos, &index);
        if (d->selectionManager->currentItem() != it) {
            d->selectionManager->setCurrentItem(it);
            d->currentIndex = index;
        }
        QtTreeSelection selection = d->anchoredSelection();
        d->selectionManager->setAnchorSelection(QtTreeSelection());
        d->selectionManager->setSelected(selection, d->selectionManager->anchorSelectionMode());
        //d->selectionManager->endAnchoredSelection();
        emit itemClicked(it, Qt::LeftButton);
        return true;
    }
    return false;
}

/*!
 */
bool QtTreeController::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::dragEnterEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::dragLeaveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::dragMoveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::dropEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::hoverEnterEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::hoverMoveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::hoverLeaveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTreeController::wheelEvent(QGraphicsSceneWheelEvent *event, const QTransform &transform)
{
    Q_UNUSED(transform);
    Q_D(QtTreeController);
    if (!d->view)
        return false;
    // ### FIXME: only works for scroll per item
    const int index = d->view->firstIndex() - event->delta() / 120;
    d->view->setFirstIndex(qBound(0, index, d->view->maximumFirstIndex()));
    return true;
}

/*!
 */
bool QtTreeController::resizeEvent(QGraphicsSceneResizeEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    Q_D(QtTreeController);
    if (d->view) {
        d->view->updateLayout();
        return true;
    }
    return false;
}

/*!
 */
bool QtTreeController::processEvent(QEvent *event, const QTransform &transform)
{
    Q_ASSERT(event);
    Q_ASSERT(event);
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

#include "moc_qtreecontroller.cpp"
