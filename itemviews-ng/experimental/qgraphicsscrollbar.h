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

#ifndef QTGRAPHICSSCROLLBAR_H
#define QTGRAPHICSSCROLLBAR_H

#include "../qitemviewsglobal.h"

#include <QtGui/qgraphicswidget.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtGraphicsScrollBarPrivate;
class QStyleOptionSlider;

class Q_ITEMVIEWSNG_EXPORT QtGraphicsScrollBar : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(qreal singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(qreal pageStep READ pageStep WRITE setPageStep)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

public:
    QtGraphicsScrollBar(Qt::Orientation orientation = Qt::Vertical, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~QtGraphicsScrollBar();

    qreal value() const;
    
    qreal minimum() const;
    void setMinimum(qreal minimum);

    qreal maximum() const;
    void setMaximum(qreal maximum);
    
    qreal singleStep() const;
    void setSingleStep(qreal step);

    qreal pageStep() const;
    void setPageStep(qreal step);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);
    
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

public Q_SLOTS:
    void setValue(qreal value);
    void increase();
    void decrease();
    
Q_SIGNALS:
    void valueChanged(qreal value);
    void minimumChanged(qreal minimum);
    void maximumChanged(qreal maximum);

protected:
    //QtGraphicsScrollBar(QtGraphicsScrollBarPrivate &, Qt::Orientation orientation = Qt::Vertical, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);

    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QGraphicsSceneWheelEvent *event);
#endif
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    virtual void initStyleOption(QStyleOptionSlider *option) const;

protected:
    QtGraphicsScrollBarPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtGraphicsScrollBar)
    Q_DISABLE_COPY(QtGraphicsScrollBar)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif// QTGRAPHICSSCROLLBAR_H
