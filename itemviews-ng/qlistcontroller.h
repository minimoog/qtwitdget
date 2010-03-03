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

#ifndef QTLISTCONTROLLER_H
#define QTLISTCONTROLLER_H

#include "qitemviewsglobal.h"

#include <QtCore/qobject.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QEvent;
class QKeyEvent;
class QShowEvent;
class QHideEvent;
class QInputMethodEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneDragDropEvent;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneResizeEvent;
class QTransform;

class QtListModelInterface;
class QtListSelectionManager;
class QtGraphicsListView;

class QtListControllerPrivate;

class Q_ITEMVIEWSNG_EXPORT QtListController : public QObject
{
    Q_OBJECT
    Q_ENUMS(SelectionBehavior)
    Q_PROPERTY(QtListModelInterface* model READ model WRITE setModel)
    Q_PROPERTY(QtListSelectionManager* selectionManager READ selectionManager WRITE setSelectionManager)
    Q_PROPERTY(QtGraphicsListView *view READ view WRITE setView)
    Q_PROPERTY(SelectionBehavior selectionBehavior READ selectionBehavior WRITE setSelectionBehavior)
    Q_PROPERTY(bool isWheelEnabled READ isWheelEnabled WRITE setWheelEnabled)

public:
    enum SelectionBehavior {
        NoSelection,
        SingleSelection,
        MultiSelection
    };

    QtListController(QObject *parent = 0);
    virtual ~QtListController();

    QtListModelInterface *model() const;
    void setModel(QtListModelInterface *model);

    QtListSelectionManager *selectionManager() const;
    void setSelectionManager(QtListSelectionManager *selections);

    QtGraphicsListView *view() const;
    void setView(QtGraphicsListView *view);

    SelectionBehavior selectionBehavior() const;
    void setSelectionBehavior(SelectionBehavior behavior);

    bool isWheelEnabled() const;
    void setWheelEnabled(bool enable);

    virtual qreal scrollValue() const;
    virtual qreal pageStepValue(qreal *maximumScrollValue = 0) const;
    virtual qreal maximumScrollValue() const;

    virtual bool showEvent(QShowEvent *event);
    virtual bool hideEvent(QHideEvent *event);
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
    void itemActivated(int index);
    void itemClicked(int index, Qt::MouseButton button);
    // ### add more signals for user actions
    void modelChanged(QtListModelInterface *current, QtListModelInterface *previous);
    void selectionManagerChanged(QtListSelectionManager *current, QtListSelectionManager *previous);
    void viewChanged(QtGraphicsListView *current, QtGraphicsListView *previous);
    void scrollValueChanged(qreal value);

public Q_SLOTS:
    void setScrollValue(qreal value);
    void setScrollValue(int value); // ###

protected:
    QtListController(QtListControllerPrivate &, QObject *parent);
    QtListControllerPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtListController)
    Q_DISABLE_COPY(QtListController)
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_viewDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_firstIndexChanged(int))
    Q_PRIVATE_SLOT(d_func(), void _q_offsetChanged(qreal))
    Q_PRIVATE_SLOT(d_func(), void _q_currentChanged(int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_animationFinished())
};

QT_END_NAMESPACE

QT_END_HEADER

#endif //QTLISTCONTROLLER_H
