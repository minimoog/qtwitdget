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

#ifndef QTKINETICLISTCONTROLLER_H
#define QTKINETICLISTCONTROLLER_H

#include "../qlistcontroller.h"
#include <QtCore/qmetatype.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtKineticListControllerPrivate;
class Q_ITEMVIEWSNG_EXPORT QtKineticListController : public QtListController
{
    Q_OBJECT
    Q_PROPERTY(qreal scrollVelocity READ scrollVelocity WRITE setScrollVelocity)
    Q_PROPERTY(qreal maximumScrollVelocity READ maximumScrollVelocity WRITE setMaximumScrollVelocity)
    Q_PROPERTY(bool overshootEnabled READ isOvershootEnabled WRITE setOvershootEnabled)
    Q_PROPERTY(bool centerOnItemEnabled READ isCenterOnItemEnabled WRITE setCenterOnItemEnabled)

public:
    QtKineticListController(QObject *parent = 0);
    virtual ~QtKineticListController();

    void setScrollVelocity(qreal velocity);
    qreal scrollVelocity() const;

    void setMaximumScrollVelocity(qreal velocity);
    qreal maximumScrollVelocity() const;

    void setOvershootEnabled(bool enable);
    bool isOvershootEnabled() const;

    void setCenterOnItemEnabled(bool enable);
    bool isCenterOnItemEnabled() const;

public Q_SLOTS:
    void stop();

protected:
    bool hideEvent(QHideEvent *event);
    bool mouseMoveEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform);
    bool mousePressEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform);
    bool mouseReleaseEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform);
    bool wheelEvent(QGraphicsSceneWheelEvent *event, const QTransform &transform);
    void timerEvent(QTimerEvent *event);

private:
    Q_DISABLE_COPY(QtKineticListController)
    Q_DECLARE_PRIVATE(QtKineticListController)
};

QT_END_HEADER

QT_END_NAMESPACE

#endif // QTKINETICLISTCONTROLLER_H
