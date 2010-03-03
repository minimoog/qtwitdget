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

#include "qtablecontroller.h"
#include "qtablecontroller_p.h"

#include "qtablemodelinterface.h"
#include "qtableselectionmanager.h"
#include "qgraphicstableview.h"
#include "qgraphicsheader.h"

#include <qdebug.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <qevent.h>

#include <qgraphicssceneevent.h>

QT_BEGIN_NAMESPACE

// QTableHeaderDataProvider

class QTableHeaderDataProvider : public QtGraphicsHeaderDataProvider
{
public:
    QTableHeaderDataProvider() : model(0) {}
    QHash<QByteArray,QVariant> data(int logicalIndex, const QList<QByteArray> &roles) const
    {
        // ### call the model - temporary implementation
        QHash<QByteArray,QVariant> hash;
        for (int i = 0; i < roles.count(); ++i)
            if (roles.at(i) == "DisplayRole")
                hash.insert(roles.at(i), logicalIndex + 1);
        return hash;
    }
    QtTableModelInterface *model;
};

// QtTableControllerPrivate

QtTableControllerPrivate::QtTableControllerPrivate()
    : q_ptr(0),
      view(0),
      model(0),
      horizontalHeader(0),
      verticalHeader(0),
      horizontalHeaderDataProvider(new QTableHeaderDataProvider),
      verticalHeaderDataProvider(new QTableHeaderDataProvider),
      selectionManager(0),
      cachedEditorWidget(0),
      cachedEditorRow(-1),
      cachedEditorColumn(-1),
      editorFactory(0),
      scrollPerRow(true),
      scrollPerColumn(true)
{
}

QtTableControllerPrivate::~QtTableControllerPrivate()
{
}

void QtTableControllerPrivate::_q_viewDestroyed()
{
    view = 0;
}

void QtTableControllerPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtTableControllerPrivate::_q_selectionsDestroyed()
{
    selectionManager = 0;
}

void QtTableControllerPrivate::_q_verticalHeaderDestroyed()
{
    verticalHeader = 0;
}

void QtTableControllerPrivate::_q_horizontalHeaderDestroyed()
{
    horizontalHeader = 0;
}

void QtTableControllerPrivate::_q_rowPressed(int row, Qt::KeyboardModifiers modifiers)
{
    if (model && selectionManager) {
        if (modifiers & Qt::ControlModifier) {
            selectionManager->beginAnchoredSelection(row, 0, QtTableSelectionManager::Toggle);
            selectionManager->setCurrentCell(row, model->columnCount() - 1);
        } else if (modifiers & Qt::ShiftModifier) {
            selectionManager->clearSelections();
            selectionManager->beginAnchoredSelection(selectionManager->anchorRow(), 0);
            selectionManager->setCurrentCell(row, model->columnCount() - 1);
        } else { // no modifiers
            selectionManager->clearSelections();
            selectionManager->beginAnchoredSelection(row, 0);
            selectionManager->setCurrentCell(row, model->columnCount() - 1);
        }
    }
}

void QtTableControllerPrivate::_q_rowReleased(int row, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers);
    if (selectionManager) {
        if (selectionManager->currentRow() != row)
            selectionManager->setCurrentCell(row, selectionManager->currentColumn());
        selectionManager->endAnchoredSelection();
        //emit rowClicked(row, Qt::LeftButton);
    }
}

void QtTableControllerPrivate::_q_rowDragSelected(int row, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers);
    if (selectionManager && selectionManager->currentRow() != row)
        selectionManager->setCurrentCell(row, selectionManager->currentColumn());
}

void QtTableControllerPrivate::_q_columnPressed(int column, Qt::KeyboardModifiers modifiers)
{
    if (model && selectionManager) {
        if (modifiers & Qt::ControlModifier) {
            selectionManager->beginAnchoredSelection(0, column, QtTableSelectionManager::Toggle);
            selectionManager->setCurrentCell(model->rowCount() - 1, column);
        } else if (modifiers & Qt::ShiftModifier) {
            selectionManager->clearSelections();
            selectionManager->beginAnchoredSelection(0, selectionManager->anchorColumn());
            selectionManager->setCurrentCell(model->rowCount() - 1, column);
        } else { // no modifiers
            selectionManager->clearSelections();
            selectionManager->beginAnchoredSelection(0, column);
            selectionManager->setCurrentCell(model->rowCount() - 1, column);
        }
    }
}

void QtTableControllerPrivate::_q_columnReleased(int column, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers);
    if (selectionManager) {
        if (selectionManager->currentColumn() != column)
            selectionManager->setCurrentCell(selectionManager->currentRow(), column);
        selectionManager->endAnchoredSelection();
        //emit columnClicked(column, Qt::LeftButton);
    }
}

void QtTableControllerPrivate::_q_columnDragSelected(int column, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers);
    if (selectionManager && selectionManager->currentColumn() != column)
        selectionManager->setCurrentCell(selectionManager->currentRow(), column);
}

void QtTableControllerPrivate::_q_firstRowChanged(int row)
{
    Q_Q(QtTableController);
    if (scrollPerRow)
        emit q->verticalScrollValueChanged(static_cast<qreal>(row));
}

void QtTableControllerPrivate::_q_firstColumnChanged(int column)
{
    Q_Q(QtTableController);
    if (scrollPerColumn)
        emit q->horizontalScrollValueChanged(static_cast<qreal>(column));
}

void QtTableControllerPrivate::_q_verticalOffsetChanged(qreal offset)
{
    Q_Q(QtTableController);
    if (!scrollPerRow)
        emit q->verticalScrollValueChanged(offset);
}

void QtTableControllerPrivate::_q_horizontalOffsetChanged(qreal offset)
{
    Q_Q(QtTableController);
    if (scrollPerRow)
        emit q->horizontalScrollValueChanged(offset);
}

void QtTableControllerPrivate::_q_setHorizontalHeader(QtGraphicsHeader* header)
{
    Q_Q(QtTableController);
    q->setHorizontalHeader(header);
}

void QtTableControllerPrivate::_q_setVerticalHeader(QtGraphicsHeader* header)
{
    Q_Q(QtTableController);
    q->setVerticalHeader(header);
}

QWidget *QtTableControllerPrivate::cellEditor(int row, int column) const
{
    if (editorHash.isEmpty())
        return 0;
    if (row == cachedEditorRow && column == cachedEditorColumn)
        return cachedEditorWidget;
    const quint64 key = (quint64(row) << 32)|quint64(column);
    QWidget *widget = editorHash.value(key, 0);
    if (widget) {
        cachedEditorWidget = widget;
        cachedEditorRow = row;
        cachedEditorColumn = column;
    }
    return widget;
}

void QtTableControllerPrivate::setCellEditor(QWidget *editor, int row, int column)
{
    Q_Q(QtTableController);
    const quint64 key = (quint64(row) << 32)|quint64(column);
    if (editor) {
        const QSize size = editor->size();
        editor->setGeometry(-size.width(), -size.height(), size.width(), size.height());
        editor->setAttribute(Qt::WA_DontShowOnScreen);
        editorHash.insert(key, editor);
        editor->window()->installEventFilter(q);
        setCellEditorData(editor, row, column);
    } else if ((editor = editorHash.take(key))) {
        editor->window()->removeEventFilter(q);
        editor = 0;
    }
    cachedEditorWidget = editor;
    cachedEditorRow = row;
    cachedEditorColumn = column;
}

void QtTableControllerPrivate::setCellEditorData(QWidget *editor, int row, int column)
{
    QVariant value;// = cachedData(row, column, Qt::EditRole); // ### FIXME
    QByteArray userPropertyName = editor->metaObject()->userProperty().name();

    qDebug() << "setCellEditorData" << row << column << value;

    // ### Qt 5: remove
    // A work-around for missing "USER true" in qdatetimeedit.h for
    // QTimeEdit's time property and QDateEdit's date property.
    // It only triggers if the default user property "dateTime" is
    // reported for QTimeEdit and QDateEdit.
    if (userPropertyName == "dateTime") {
        if (editor->inherits("QTimeEdit"))
            userPropertyName = "time";
        else if (editor->inherits("QDateEdit"))
            userPropertyName = "date";
    }

    if (userPropertyName.isEmpty()) // #### check editorFactory
        userPropertyName = editorFactory->valuePropertyName(static_cast<QVariant::Type>(value.userType()));
    if (!userPropertyName.isEmpty()) {
        if (!value.isValid())
            value = QVariant(editor->property(userPropertyName).userType(), (const void *)0);
        editor->setProperty(userPropertyName, value);
    }
}

// QtTableController

/*!
    \class QtTableController
    \brief provides a controller for table model and view objects.

    QtTableController represents a controller in the model-view-controller (MFC)
    design-pattern.

    It is used together with a sub-class of QtTableModelInterface and QtGraphicsTableView.

    QtTableController is responsible for handling user input events and make the requested
    changes to the view, model, or selections.

    For example, when the user clicks a cell in the view, it is QtTableController's
    responsibility to change selection state of that cell.

    QtTableWidgetNG contains a QtTableController, as well as a QtTableDefaultModel
    and a QtGraphicsTableView.
*/

/*!
 */
QtTableController::QtTableController(QObject *parent)
    : QObject(parent), d_ptr(new QtTableControllerPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTableController::QtTableController(QtTableControllerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTableController::~QtTableController()
{
    delete d_ptr;
}

/*!
 */
QtGraphicsTableView *QtTableController::view() const
{
    Q_D(const QtTableController);
    return d->view;
}

/*!
 */
void QtTableController::setView(QtGraphicsTableView *view)
{
    Q_D(QtTableController);
    if (d->view == view)
        return;
    QtGraphicsTableView *old = d->view;
    if (d->view) {
        disconnect(d->view, SIGNAL(destroyed()), this, SLOT(_q_viewDestroyed()));
        disconnect(d->view, SIGNAL(firstRowChanged(int)), this, SLOT(_q_firstRowChanged(int)));
        disconnect(d->view, SIGNAL(firstColumnChanged(int)), this, SLOT(_q_firstColumnChanged(int)));
        disconnect(d->view, SIGNAL(verticalOffsetChanged(qreal)), this, SLOT(_q_verticalOffsetChanged(qreal)));
        disconnect(d->view, SIGNAL(horizontalOffsetChanged(qreal)), this, SLOT(_q_horizontalOffsetChanged(qreal)));
        disconnect(d->view, SIGNAL(horizontalHeaderChanged(QtGraphicsHeader*)), this, SLOT(_q_setHorizontalHeader(QtGraphicsHeader*)));
        disconnect(d->view, SIGNAL(verticalHeaderChanged(QtGraphicsHeader*)), this, SLOT(_q_setVerticalHeader(QtGraphicsHeader*)));
    }
    d->view = view;
    if (d->view) {
        if (d->view->horizontalHeader() != d->horizontalHeader && d->view->horizontalHeader()) // different
            d->horizontalHeader->deleteLater();
        setHorizontalHeader(d->view->horizontalHeader()); // view leads
        if (d->horizontalHeader == 0)
            setHorizontalHeader(new QtGraphicsHeader(Qt::Horizontal));
        if (d->view->verticalHeader() != d->verticalHeader && d->view->verticalHeader()) // different
            d->verticalHeader->deleteLater();
        setVerticalHeader(d->view->verticalHeader()); // view leads
        if (d->verticalHeader == 0)
            setVerticalHeader(new QtGraphicsHeader(Qt::Vertical));

        connect(d->view, SIGNAL(destroyed()), this, SLOT(_q_viewDestroyed()));
        connect(d->view, SIGNAL(firstRowChanged(int)), this, SLOT(_q_firstRowChanged(int)));
        connect(d->view, SIGNAL(firstColumnChanged(int)), this, SLOT(_q_firstColumnChanged(int)));
        connect(d->view, SIGNAL(verticalOffsetChanged(qreal)), this, SLOT(_q_verticalOffsetChanged(qreal)));
        connect(d->view, SIGNAL(horizontalOffsetChanged(qreal)), this, SLOT(_q_horizontalOffsetChanged(qreal)));
        connect(d->view, SIGNAL(horizontalHeaderChanged(QtGraphicsHeader*)), this, SLOT(_q_setHorizontalHeader(QtGraphicsHeader*)));
        connect(d->view, SIGNAL(verticalHeaderChanged(QtGraphicsHeader*)), this, SLOT(_q_setVerticalHeader(QtGraphicsHeader*)));

        d->view->setModel(d->model);
        d->view->setSelectionManager(d->selectionManager);
        d->view->setController(this);
    }
    emit viewChanged(view, old);
}

/*!
 */
QtTableModelInterface *QtTableController::model() const
{
    Q_D(const QtTableController);
    return d->model;
}

/*!
 */
void QtTableController::setModel(QtTableModelInterface *model)
{
    Q_D(QtTableController);
    if (d->model == model)
        return;
    QtTableModelInterface *old = d->model;
    if (d->model) {
        QObject::disconnect(d->model, SIGNAL(destroyed()),
                            this, SLOT(_q_modelDestroyed()));
    }
    d->model = model;
    if (d->model) {
        QObject::connect(d->model, SIGNAL(destroyed()),
                         this, SLOT(_q_modelDestroyed()));
        if (d->horizontalHeader)
            d->horizontalHeader->setSectionCount(d->model->columnCount());
        if (d->verticalHeader)
            d->verticalHeader->setSectionCount(d->model->rowCount());
    }
    if (d->selectionManager)
        d->selectionManager->setModel(d->model);
    if (d->verticalHeaderDataProvider)
        d->verticalHeaderDataProvider->model = model;
    if (d->horizontalHeaderDataProvider)
        d->horizontalHeaderDataProvider->model = model;
    emit modelChanged(model, old);
}

/*!
 */
QtTableSelectionManager *QtTableController::selectionManager() const
{
    Q_D(const QtTableController);
    return d->selectionManager;
}

/*!
 */
void QtTableController::setSelectionManager(QtTableSelectionManager *selectionManager)
{
    Q_D(QtTableController);
    if (d->selectionManager == selectionManager)
        return;
    QtTableSelectionManager *old = d->selectionManager;
    if (d->selectionManager) {
        disconnect(d->selectionManager, SIGNAL(destroyed()),
                   this, SLOT(_q_selectionsDestroyed()));
    }
    d->selectionManager = selectionManager;
    if (d->selectionManager) {
        connect(d->selectionManager, SIGNAL(destroyed()),
                this, SLOT(_q_selectionsDestroyed()));
        if (d->selectionManager->model() != d->model)
            d->selectionManager->setModel(d->model);
    }
    emit selectionManagerChanged(selectionManager, old);
}

/*!
 */
QtGraphicsHeader *QtTableController::verticalHeader() const
{
    Q_D(const QtTableController);
    return d->verticalHeader;
}

/*!
 */
void QtTableController::setVerticalHeader(QtGraphicsHeader *header)
{
    Q_D(QtTableController);
    if (d->verticalHeader == header)
        return;
    QtGraphicsHeader *old = d->verticalHeader;
    if (d->verticalHeader) {
        disconnect(d->verticalHeader, SIGNAL(destroyed()),
                   this, SLOT(_q_verticalHeaderDestroyed()));
        disconnect(d->verticalHeader, SIGNAL(sectionPressed(int, Qt::KeyboardModifiers)),
                   this, SLOT(_q_rowPressed(int, Qt::KeyboardModifiers)));
        disconnect(d->verticalHeader, SIGNAL(sectionReleased(int, Qt::KeyboardModifiers)),
                   this, SLOT(_q_rowReleased(int, Qt::KeyboardModifiers)));
        disconnect(d->verticalHeader, SIGNAL(sectionDragSelected(int, Qt::KeyboardModifiers)),
                   this, SLOT(_q_rowDragSelected(int, Qt::KeyboardModifiers)));
    }
    d->verticalHeader = header;
    if (d->verticalHeader) {
        d->verticalHeader->setOrientation(Qt::Vertical);
        d->verticalHeader->setSectionCount(d->model ? d->model->rowCount() : 0);
        d->verticalHeader->setDataProvider(d->verticalHeaderDataProvider);
        connect(d->verticalHeader, SIGNAL(destroyed()),
                this, SLOT(_q_verticalHeaderDestroyed()));
        connect(d->verticalHeader, SIGNAL(sectionPressed(int, Qt::KeyboardModifiers)),
                this, SLOT(_q_rowPressed(int, Qt::KeyboardModifiers)));
        connect(d->verticalHeader, SIGNAL(sectionReleased(int, Qt::KeyboardModifiers)),
                this, SLOT(_q_rowReleased(int, Qt::KeyboardModifiers)));
        connect(d->verticalHeader, SIGNAL(sectionDragSelected(int, Qt::KeyboardModifiers)),
                this, SLOT(_q_rowDragSelected(int, Qt::KeyboardModifiers)));
        if (d->view)
            d->view->setVerticalHeader(d->verticalHeader);
    }
    emit verticalHeaderChanged(d->verticalHeader, old);
}

/*!
 */
QtGraphicsHeader *QtTableController::horizontalHeader() const
{
    Q_D(const QtTableController);
    return d->horizontalHeader;
}

/*!
 */
void QtTableController::setHorizontalHeader(QtGraphicsHeader *header)
{
    Q_D(QtTableController);
    if (d->horizontalHeader == header)
        return;
    QtGraphicsHeader *old = d->horizontalHeader;
    if (d->horizontalHeader) {
        disconnect(d->horizontalHeader, SIGNAL(destroyed()),
                   this, SLOT(_q_horizontalHeaderDestroyed()));
        disconnect(d->horizontalHeader, SIGNAL(sectionPressed(int, Qt::KeyboardModifiers)),
                   this, SLOT(_q_columnPressed(int, Qt::KeyboardModifiers)));
        disconnect(d->horizontalHeader, SIGNAL(sectionReleased(int, Qt::KeyboardModifiers)),
                   this, SLOT(_q_columnReleased(int, Qt::KeyboardModifiers)));
        disconnect(d->horizontalHeader, SIGNAL(sectionDragSelected(int, Qt::KeyboardModifiers)),
                   this, SLOT(_q_columnDragSelected(int, Qt::KeyboardModifiers)));
    }
    d->horizontalHeader = header;
    if (d->horizontalHeader) {
        d->horizontalHeader->setOrientation(Qt::Horizontal);
        d->horizontalHeader->setSectionCount(d->model ? d->model->columnCount() : 0);
        d->horizontalHeader->setDataProvider(d->horizontalHeaderDataProvider);
        connect(d->horizontalHeader, SIGNAL(destroyed()),
                this, SLOT(_q_horizontalHeaderDestroyed()));
        connect(d->horizontalHeader, SIGNAL(sectionPressed(int, Qt::KeyboardModifiers)),
                this, SLOT(_q_columnPressed(int, Qt::KeyboardModifiers)));
        connect(d->horizontalHeader, SIGNAL(sectionReleased(int, Qt::KeyboardModifiers)),
                this, SLOT(_q_columnReleased(int, Qt::KeyboardModifiers)));
        connect(d->horizontalHeader, SIGNAL(sectionDragSelected(int, Qt::KeyboardModifiers)),
                this, SLOT(_q_columnDragSelected(int, Qt::KeyboardModifiers)));
        if (d->view)
            d->view->setHorizontalHeader(d->horizontalHeader);
    }
    emit horizontalHeaderChanged(d->horizontalHeader, old);
}
/*
QWidget *QtTableController::createEditor(int row, int column, QWidget *contextWidget) const
{
    Q_D(const QtTableController);
    const QVariant::Type type = static_cast<QVariant::Type>(d->data(row, column, Qt::EditRole).userType());
    const QItemEditorFactory *factory = d->editorFactory ? d->editorFactory : QItemEditorFactory::defaultFactory();
    return factory->createEditor(type, contextWidget);
}
*/

/*!
 */
qreal QtTableController::verticalScrollValue() const
{
    Q_D(const QtTableController);
    if (d->view)
        return d->scrollPerRow ? d->view->firstRow() : d->view->verticalOffset();
    return 0;
}

/*!
 */
void QtTableController::setVerticalScrollValue(qreal value)
{
    Q_D(QtTableController);
    if (d->view) {
        if (d->scrollPerRow)
            d->view->setFirstRow(value);
        else
            d->view->setVerticalOffset(value);
    }
}

/*!
 */
qreal QtTableController::horizontalScrollValue() const
{
    Q_D(const QtTableController);
    if (d->view)
        return d->scrollPerColumn ? d->view->firstColumn() : d->view->horizontalOffset();
    return 0;
}

/*!
 */
void QtTableController::setHorizontalScrollValue(qreal value)
{
    Q_D(QtTableController);
    if (d->view) {
        if (d->scrollPerColumn)
            d->view->setFirstColumn(value);
        else
            d->view->setHorizontalOffset(value);
    }
}

/*!
 */
qreal QtTableController::verticalPageStepValue(qreal *maximumVerticalScrollValue) const
{
    Q_D(const QtTableController);
    if (d->view) {
        if (d->scrollPerRow) {
            const int count = d->view->rowCount();
            const int maximum = d->view->maximumFirstRow();
            if (maximumVerticalScrollValue)
                *maximumVerticalScrollValue = maximum;;
            return count - maximum;
        } else {
            if (maximumVerticalScrollValue)
                *maximumVerticalScrollValue = d->view->maximumVerticalOffset();
            return d->view->size().height();
        }
    }
    return 0;
}

/*!
 */
qreal QtTableController::horizontalPageStepValue(qreal *maximumHorizontalScrollValue) const
{
    Q_D(const QtTableController);
    if (d->view) {
        if (d->scrollPerRow) {
            const int count = d->view->columnCount();
            const int maximum = d->view->maximumFirstColumn();
            if (maximumHorizontalScrollValue)
                *maximumHorizontalScrollValue = maximum;
            return count - maximum;
        } else {
            return d->view->size().width();
        }
    }
    return 0;
}

/*!
 */
qreal QtTableController::maximumVerticalScrollValue() const
{
    Q_D(const QtTableController);
    if (d->view) {
        if (d->scrollPerRow)
            return d->view->maximumFirstRow();
        else
            return d->view->maximumVerticalOffset();
    }
    return 0;
}

/*!
 */
qreal QtTableController::maximumHorizontalScrollValue() const
{
    Q_D(const QtTableController);
    if (d->view) {
        if (d->scrollPerRow)
            return d->view->maximumFirstColumn();
        else
            return d->view->maximumHorizontalOffset();
    }
    return 0;
}
/*!
 */
bool QtTableController::keyPressEvent(QKeyEvent *event)
{
    Q_D(QtTableController);
    if (d->model && d->selectionManager) {
        int row = d->selectionManager->currentRow();
        int column = d->selectionManager->currentColumn();
        switch (event->key()) {
        case Qt::Key_Left:
            column = qBound(0, column - 1, d->model->columnCount() - 1);
            break;
        case Qt::Key_Right:
            column = qBound(0, column + 1, d->model->columnCount() - 1);
            break;
        case Qt::Key_Up:
            row = qBound(0, row - 1, d->model->rowCount() - 1);
            break;
        case Qt::Key_Down:
            row = qBound(0, row + 1, d->model->rowCount() - 1);
            break;
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
        case Qt::Key_Space:
            // ### FIXME: implement
        default:
            break;
        }
        if (d->selectionManager->currentRow() != row || d->selectionManager->currentColumn() != column) {
            if (event->modifiers() & Qt::ShiftModifier) {
                d->selectionManager->beginAnchoredSelection(d->selectionManager->anchorRow(), d->selectionManager->anchorColumn());
                d->selectionManager->setCurrentCell(row, column);
            } else {
                d->selectionManager->endAnchoredSelection();
                d->selectionManager->clearSelections();
                d->selectionManager->setCurrentCell(row, column);
            }
            return true;
        }

    }
    return false;
}

/*!
 */
bool QtTableController::inputMethodEvent(QInputMethodEvent *event)
{
    Q_UNUSED(event);
    return false;
}

/*!
 */
bool QtTableController::mousePressEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtTableController);
    if (d->view && d->selectionManager && event->buttons() & Qt::LeftButton) {
        int row = d->view->rowAt(event->pos(), transform);
        int column = d->view->columnAt(event->pos(), transform);
        if (event->modifiers() & Qt::ControlModifier) {
            d->selectionManager->beginAnchoredSelection(row, column, QtTableSelectionManager::Toggle);
            d->selectionManager->setCurrentCell(row, column);
        } else if (event->modifiers() & Qt::ShiftModifier) {
            d->selectionManager->clearSelections();
            d->selectionManager->beginAnchoredSelection(d->selectionManager->anchorRow(), d->selectionManager->anchorColumn());
            d->selectionManager->setCurrentCell(row, column);
        } else { // no modifiers
            d->selectionManager->clearSelections();
            d->selectionManager->beginAnchoredSelection(row, column);
            d->selectionManager->setCurrentCell(row, column);
        }
        return true;
    }
    return false;
}

/*!
 */
bool QtTableController::mouseMoveEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtTableController);
    if (d->view && d->selectionManager && event->buttons() & Qt::LeftButton) {
        const int row = d->view->rowAt(event->pos(), transform);
        const int column = d->view->columnAt(event->pos(), transform);
        if (d->selectionManager->currentRow() != row || d->selectionManager->currentColumn() != column) {
            d->selectionManager->setCurrentCell(row, column);
            return true;
        }
        return true;
    }
    return false;
}

/*!
 */
bool QtTableController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtTableController);
    if (d->selectionManager && d->view && event->button() == Qt::LeftButton) {
        const int row = d->view->rowAt(event->pos(), transform);
        const int column = d->view->columnAt(event->pos(), transform);
        if (d->selectionManager->currentRow() != row || d->selectionManager->currentColumn() != column)
            d->selectionManager->setCurrentCell(row, column);
        d->selectionManager->endAnchoredSelection();
        emit cellClicked(row, column, Qt::LeftButton);
        return true;
    }
    return false;
}

/*!
 */
bool QtTableController::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::dragEnterEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::dragLeaveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::dragMoveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::dropEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::hoverEnterEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::hoverMoveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::hoverLeaveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::wheelEvent(QGraphicsSceneWheelEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

/*!
 */
bool QtTableController::resizeEvent(QGraphicsSceneResizeEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    Q_D(QtTableController);
    if (!d->view)
        return false;
    d->view->updateLayout();
    return true;
}

/*!
 */
bool QtTableController::processEvent(QEvent *event, const QTransform &transform)
{
    if (!event)
        return false;

    switch (event->type()) {
//     case QEvent::FocusIn:
//     case QEvent::FocusOut:
//             return d->focusEvent(static_cast<QFocusEvent*>(event));
    case QEvent::KeyPress:
        return keyPressEvent(static_cast<QKeyEvent*>(event));
    case QEvent::InputMethod:
        return inputMethodEvent(static_cast<QInputMethodEvent*>(event));
    case QEvent::GraphicsSceneMousePress:
        return mousePressEvent(static_cast<QGraphicsSceneMouseEvent*>(event), transform);
    case QEvent::GraphicsSceneMouseMove:
        return mouseMoveEvent(static_cast<QGraphicsSceneMouseEvent*>(event), transform);
    case QEvent::GraphicsSceneMouseRelease:
        return mouseReleaseEvent(static_cast<QGraphicsSceneMouseEvent*>(event), transform);
#ifndef QT_NO_DRAGANDDROP
    case QEvent::GraphicsSceneDragEnter:
        return dragEnterEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), transform);
    case QEvent::GraphicsSceneDragLeave:
        return dragLeaveEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), transform);
    case QEvent::GraphicsSceneDragMove:
        return dragMoveEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), transform);
    case QEvent::GraphicsSceneDrop:
        return dropEvent(static_cast<QGraphicsSceneDragDropEvent*>(event), transform);
#endif
    case QEvent::GraphicsSceneHoverEnter:
        return hoverEnterEvent(static_cast<QGraphicsSceneHoverEvent*>(event), transform);
    case QEvent::GraphicsSceneHoverMove:
        return hoverMoveEvent(static_cast<QGraphicsSceneHoverEvent*>(event), transform);
    case QEvent::GraphicsSceneHoverLeave:
        return hoverLeaveEvent(static_cast<QGraphicsSceneHoverEvent*>(event), transform);
    case QEvent::GraphicsSceneResize:
        return resizeEvent(static_cast<QGraphicsSceneResizeEvent*>(event), transform);
    default:
        break;
    }
    return false;
}

QT_END_NAMESPACE

#include "moc_qtablecontroller.cpp"
