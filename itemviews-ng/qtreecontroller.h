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

#ifndef QTTREECONTROLLER_H
#define QTTREECONTROLLER_H

#include <QtCore/qobject.h>
#include "qtreemodelinterface.h"

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
class QTransform;

class QtTreeControllerPrivate;
class QtTreeSelectionManager;
class QtTreeModelBase;
class QtGraphicsTreeView;
class QtGraphicsHeader;


class Q_ITEMVIEWSNG_EXPORT QtTreeController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QtTreeModelBase* model READ model WRITE setModel) // ### 
public:
    QtTreeController(QObject *parent = 0);
    virtual ~QtTreeController();

    QtGraphicsTreeView *view() const;
    void setView(QtGraphicsTreeView *view);

    QtTreeModelBase *model() const;
    void setModel(QtTreeModelBase *model);

    QtTreeSelectionManager *selectionManager() const;
    void setSelectionManager(QtTreeSelectionManager *selections);

    QtGraphicsHeader *header() const;
    void setHeader(QtGraphicsHeader *header);

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
    void itemClicked(QtTreeModelIterator &it, Qt::MouseButton button);
    void viewChanged(QtGraphicsTreeView *current, QtGraphicsTreeView *previous);
    void modelChanged(QtTreeModelBase *current, QtTreeModelBase *previous);
    void selectionManagerChanged(QtTreeSelectionManager *current, QtTreeSelectionManager *previous);
    void verticalScrollValueChanged(qreal value);
    void horizontalScrollValueChanged(qreal value);

public Q_SLOTS:
    void setVerticalScrollValue(qreal value);
    void setHorizontalScrollValue(qreal value);
    void setVerticalScrollValue(int value);
    void setHorizontalScrollValue(int value);

protected:
    QtTreeController(QtTreeControllerPrivate &, QObject *parent = 0);
    QtTreeControllerPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtTreeController)
    Q_DISABLE_COPY(QtTreeController)
    Q_PRIVATE_SLOT(d_func(), void _q_viewDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_headerDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_firstIndexChanged(int index))
    Q_PRIVATE_SLOT(d_func(), void _q_firstSectionChanged(int section))
    Q_PRIVATE_SLOT(d_func(), void _q_verticalOffsetChanged(qreal offset))
    Q_PRIVATE_SLOT(d_func(), void _q_horizontalOffsetChanged(qreal offset))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTREECONTROLLER_H
