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

#ifndef QTTABLECONTROLLER_H
#define QTTABLECONTROLLER_H

#include "qitemviewsglobal.h"

#include <QtCore/qobject.h>
#include <QtGui/qtransform.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QEvent;
class QKeyEvent;
class QInputMethodEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneDragDropEvent;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneResizeEvent;

class QtGraphicsHeader;
class QtGraphicsTableView;
class QtTableModelInterface;
class QtTableSelectionManager;
class QtTableControllerPrivate;

class Q_ITEMVIEWSNG_EXPORT QtTableController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QtTableModelInterface* model READ model WRITE setModel)
    Q_PROPERTY(QtTableSelectionManager* selectionManager READ selectionManager WRITE setSelectionManager)
    Q_PROPERTY(QtGraphicsTableView* view READ view WRITE setView)
    Q_PROPERTY(QtGraphicsHeader* horizontalHeader READ horizontalHeader WRITE setHorizontalHeader)
    Q_PROPERTY(QtGraphicsHeader* verticalHeader READ verticalHeader WRITE setVerticalHeader)

public:
    explicit QtTableController(QObject *parent = 0);
    virtual ~QtTableController();

    QtGraphicsTableView *view() const;
    void setView(QtGraphicsTableView *view);

    QtTableModelInterface *model() const;
    void setModel(QtTableModelInterface *model);

    QtTableSelectionManager *selectionManager() const;
    void setSelectionManager(QtTableSelectionManager *selectionManager);

    QtGraphicsHeader *verticalHeader() const;
    void setVerticalHeader(QtGraphicsHeader *header);

    QtGraphicsHeader *horizontalHeader() const;
    void setHorizontalHeader(QtGraphicsHeader *header);

    virtual qreal verticalScrollValue() const;
    virtual qreal horizontalScrollValue() const;

    virtual qreal verticalPageStepValue(qreal *maximumVerticalScrollValue = 0) const;
    virtual qreal horizontalPageStepValue(qreal *maximumHorizontalScrollValue = 0) const;

    virtual qreal maximumVerticalScrollValue() const;
    virtual qreal maximumHorizontalScrollValue() const;

    virtual bool keyPressEvent(QKeyEvent *event);
    virtual bool inputMethodEvent(QInputMethodEvent *event);
    virtual bool mousePressEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform);
    virtual bool mouseMoveEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform);
    virtual bool mouseReleaseEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform);
    virtual bool mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform);
    virtual bool dragEnterEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform);
    virtual bool dragLeaveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform);
    virtual bool dragMoveEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform);
    virtual bool dropEvent(QGraphicsSceneDragDropEvent *event, const QTransform &transform);
    virtual bool hoverEnterEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform);
    virtual bool hoverMoveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform);
    virtual bool hoverLeaveEvent(QGraphicsSceneHoverEvent *event, const QTransform &transform);
    virtual bool wheelEvent(QGraphicsSceneWheelEvent *event, const QTransform &transform);
    virtual bool resizeEvent(QGraphicsSceneResizeEvent *event, const QTransform &transform);
    virtual bool processEvent(QEvent *event, const QTransform &transform);

Q_SIGNALS:
    void cellClicked(int row, int column, Qt::MouseButton button);
    void viewChanged(QtGraphicsTableView *current, QtGraphicsTableView *previous);
    void modelChanged(QtTableModelInterface *current, QtTableModelInterface *previous);
    void selectionManagerChanged(QtTableSelectionManager *current, QtTableSelectionManager *previous);
    void verticalHeaderChanged(QtGraphicsHeader *current, QtGraphicsHeader *previous);
    void horizontalHeaderChanged(QtGraphicsHeader *current, QtGraphicsHeader *previous);
    void verticalScrollValueChanged(qreal value);
    void horizontalScrollValueChanged(qreal value);

public Q_SLOTS:
    void setVerticalScrollValue(qreal value);
    void setHorizontalScrollValue(qreal value);

protected:
    QtTableController(QtTableControllerPrivate&, QObject *parent = 0);
    QtTableControllerPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtTableController)
    Q_DISABLE_COPY(QtTableController)
    Q_PRIVATE_SLOT(d_func(), void _q_viewDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_verticalHeaderDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_horizontalHeaderDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_rowPressed(int row, Qt::KeyboardModifiers modifiers))
    Q_PRIVATE_SLOT(d_func(), void _q_rowReleased(int row, Qt::KeyboardModifiers modifiers))
    Q_PRIVATE_SLOT(d_func(), void _q_rowDragSelected(int row, Qt::KeyboardModifiers modifiers))
    Q_PRIVATE_SLOT(d_func(), void _q_columnPressed(int column, Qt::KeyboardModifiers modifiers))
    Q_PRIVATE_SLOT(d_func(), void _q_columnReleased(int column, Qt::KeyboardModifiers modifiers))
    Q_PRIVATE_SLOT(d_func(), void _q_columnDragSelected(int column, Qt::KeyboardModifiers modifiers))
    Q_PRIVATE_SLOT(d_func(), void _q_firstRowChanged(int row))
    Q_PRIVATE_SLOT(d_func(), void _q_firstColumnChanged(int column))
    Q_PRIVATE_SLOT(d_func(), void _q_verticalOffsetChanged(qreal offset))
    Q_PRIVATE_SLOT(d_func(), void _q_horizontalOffsetChanged(qreal offset))
    Q_PRIVATE_SLOT(d_func(), void _q_setHorizontalHeader(QtGraphicsHeader*))
    Q_PRIVATE_SLOT(d_func(), void _q_setVerticalHeader(QtGraphicsHeader*))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTABLECONTROLLER_H
