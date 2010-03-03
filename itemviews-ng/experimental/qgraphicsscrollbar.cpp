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

#include "qgraphicsscrollbar.h"
#include "qgraphicsscrollbar_p.h"

#include <qpainter.h>
#include <qgraphicsscene.h>
#include <qgraphicssceneevent.h>

#include <qstyleoption.h>
#include <qapplication.h>

#include <qdebug.h>

QT_BEGIN_NAMESPACE

// QtGraphicsScrollBarPrivate

QtGraphicsScrollBarPrivate::QtGraphicsScrollBarPrivate()
    : value(0), minimum(0), maximum(1), singleStep(1), pageStep(1), orientation(Qt::Vertical),
      pressedValue(0), hoveredControl(QStyle::SC_None), pressedControl(QStyle::SC_None),
      initialDelay(250), repeatFrequency(50), q_ptr(0)
{
}

QtGraphicsScrollBarPrivate::~QtGraphicsScrollBarPrivate()
{
}

/*!
  \internal
 */
void QtGraphicsScrollBarPrivate::performSubControlAction(QStyle::SubControl control)
{
    Q_Q(QtGraphicsScrollBar);
    switch (control) {
    case QStyle::SC_ScrollBarSubLine:
        q->setValue(value - singleStep);
        break;
    case QStyle::SC_ScrollBarAddLine:
        q->setValue(value + singleStep);
        break;
    case QStyle::SC_ScrollBarSubPage:
        q->setValue(value - pageStep);
        break;
    case QStyle::SC_ScrollBarAddPage:
        q->setValue(value + pageStep);
        break;
    case QStyle::SC_ScrollBarSlider:
        pressedValue = value;
        q->update();
        break;
    default:
        pressedValue = -1;
        q->update();
    }
}

/*!
    \class QtGraphicsScrollBar
    \brief The QtGraphicsScrollBar provides a vertical or horizontal scroll bar.

*/

/*!
 */
QtGraphicsScrollBar::QtGraphicsScrollBar(Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(new QtGraphicsScrollBarPrivate)
{
    d_ptr->q_ptr = this;
    setOrientation(orientation);
    setAcceptHoverEvents(true);
}
/*
QtGraphicsScrollBar::QtGraphicsScrollBar(QtGraphicsScrollBarPrivate &dd, QGraphicsWidget *parent, QGraphicsScene *scene, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
    setAcceptHoverEvents(true);
}
*/

/*!
 */
QtGraphicsScrollBar::~QtGraphicsScrollBar()
{
    delete d_ptr;
}

/*!
  Returns the value of the scrollbar.
  \sa setValue()
 */
qreal QtGraphicsScrollBar::value() const
{
    Q_D(const QtGraphicsScrollBar);
    return d->value;
}

/*!
  Sets the value of the scrollbar to be the given \a value.
  \sa value()
 */
void QtGraphicsScrollBar::setValue(qreal value)
{
    Q_D(QtGraphicsScrollBar);
    value = qBound(d->minimum, value, d->maximum);
    if (value != d->value) {
        d->value = value;
        emit valueChanged(value);
        update();
    }
}

/*!
  Returns the minimum value of the scrollbar.
  \sa setMinimum()
 */
qreal QtGraphicsScrollBar::minimum() const
{
    Q_D(const QtGraphicsScrollBar);
    return d->minimum;
}

/*!
  Sets the minimum value of the scrollbar to the given \a minimum.
  \sa minimum()
 */
void QtGraphicsScrollBar::setMinimum(qreal minimum)
{
    Q_D(QtGraphicsScrollBar);
    if (minimum != d->minimum) {
        d->minimum = minimum;
        emit minimumChanged(minimum);
        setValue(d->value);
    }
}

/*!
  Returns the maximum value of the scrollbar.
  \sa setMaximum()
 */
qreal QtGraphicsScrollBar::maximum() const
{
    Q_D(const QtGraphicsScrollBar);
    return d->maximum;
}

/*!
  Sets the maximum value of the scrollbar to the given \a maximum.
  \sa maximum()
 */
void QtGraphicsScrollBar::setMaximum(qreal maximum)
{
    Q_D(QtGraphicsScrollBar);
    if (maximum != d->maximum) {
        d->maximum = maximum;
        emit maximumChanged(maximum);
        setValue(d->value);
    }
}

/*!
  Returns the single step of the scrollbar.
 */
qreal QtGraphicsScrollBar::singleStep() const
{
    Q_D(const QtGraphicsScrollBar);
    return d->singleStep;
}

/*!
  Sets the single step of the scrollbar to the given \a step.
  \sa singleStep()
 */
void QtGraphicsScrollBar::setSingleStep(qreal step)
{
    Q_D(QtGraphicsScrollBar);
    d->singleStep = step;
}

/*!
  Returns the page step of the scrollbar.
  \sa setPageStep()
 */
qreal QtGraphicsScrollBar::pageStep() const
{
    Q_D(const QtGraphicsScrollBar);
    return d->pageStep;
}

/*!
  Sets the page step of the scrollbar to be the given \a step.
  \sa setPageStep
 */
void QtGraphicsScrollBar::setPageStep(qreal step)
{
    Q_D(QtGraphicsScrollBar);
    d->pageStep = step;
}

/*!
  Returns the orientation of the scrollbar.
  \sa setOrientation()
 */
Qt::Orientation QtGraphicsScrollBar::orientation() const
{
    Q_D(const QtGraphicsScrollBar);
    return d->orientation;
}

/*!
  Sets the orientation of the scrollbar to the given \a orientation.
  \sa orientation()
 */
void QtGraphicsScrollBar::setOrientation(Qt::Orientation orientation)
{
    Q_D(QtGraphicsScrollBar);
    d->orientation = orientation;
}

/*!
  \reimp
 */
QSizeF QtGraphicsScrollBar::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if (which == Qt::MinimumSize || which == Qt::PreferredSize) {
        QStyleOptionSlider option;
        initStyleOption(&option);
        
        int scrollBarExtent = style()->pixelMetric(QStyle::PM_ScrollBarExtent, &option, 0);
        int scrollBarSliderMin = style()->pixelMetric(QStyle::PM_ScrollBarSliderMin, &option, 0);
        
        QSize contents;
        if (orientation() == Qt::Vertical)
            contents = QSize(scrollBarExtent, scrollBarExtent * 2 + scrollBarSliderMin);
        else
            contents = QSize(scrollBarExtent * 2 + scrollBarSliderMin, scrollBarExtent);
        
        return style()->sizeFromContents(QStyle::CT_ScrollBar, &option, contents, 0);
    }
    return QGraphicsWidget::sizeHint(which, constraint);
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *widget)
{
    Q_D(QtGraphicsScrollBar);
    QStyleOptionSlider option;
    initStyleOption(&option);
    option.subControls = QStyle::SC_All;
    if (d->pressedControl != QStyle::SC_None) {
        option.activeSubControls = d->pressedControl;
        option.state |= QStyle::State_Sunken;
    } else {
        option.activeSubControls = d->hoveredControl;
    }
    style()->drawComplexControl(QStyle::CC_ScrollBar, &option, painter, widget);
}

/*!
 */
void QtGraphicsScrollBar::increase()
{
    setValue(value() + singleStep());
}

/*!
 */
void QtGraphicsScrollBar::decrease()
{
    setValue(value() - singleStep());
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::timerEvent(QTimerEvent *event)
{
    Q_D(QtGraphicsScrollBar);
    if (d->initialDelayTimer.timerId() == event->timerId()) {
        d->repeatActionTimer.start(d->repeatFrequency, this);
    } else if (d->repeatActionTimer.timerId() == event->timerId()) {
        d->performSubControlAction(d->pressedControl);
    }   
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(QtGraphicsScrollBar);
    QStyleOptionSlider option;
    initStyleOption(&option);
    QStyle::SubControl control = style()->hitTestComplexControl(QStyle::CC_ScrollBar, &option, event->pos().toPoint(), 0);
    d->pressedControl = control;
    d->performSubControlAction(control);
    if ((control != QStyle::SC_ScrollBarSlider) &&
        !(d->initialDelayTimer.isActive() || d->repeatActionTimer.isActive()))
        d->initialDelayTimer.start(d->initialDelay, this);
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(QtGraphicsScrollBar);

    if (d->pressedValue == -1 || d->initialDelayTimer.isActive() || d->repeatActionTimer.isActive())
        return;
 
    QStyleOptionSlider option;
    initStyleOption(&option);

    QRect rect = style()->subControlRect(QStyle::CC_ScrollBar, &option, QStyle::SC_ScrollBarGroove, 0);
    QPointF delta = event->pos() - event->buttonDownPos(Qt::LeftButton);
    qreal range = maximum() - minimum();

    if (orientation() == Qt::Vertical) {
        qreal scale = rect.height() / (range + pageStep());
        setValue((delta.y() / scale) + d->pressedValue);
    } else {
        qreal scale = rect.width() / (range + pageStep());
        setValue((delta.x() / scale) + d->pressedValue);
    }
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    Q_D(QtGraphicsScrollBar);
    d->pressedControl = QStyle::SC_None;
    if (d->initialDelayTimer.isActive())
        d->initialDelayTimer.stop();
    if (d->repeatActionTimer.isActive())
        d->repeatActionTimer.stop();
    update();
}

#ifndef QT_NO_WHEELEVENT
/*!
  \reimp
 */
void QtGraphicsScrollBar::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    setValue(value() - event->delta() / 120);
}
#endif

/*!
  \reimp
 */
void QtGraphicsScrollBar::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_D(QtGraphicsScrollBar);
    QStyleOptionSlider option;
    initStyleOption(&option);
    QStyle::SubControl control = style()->hitTestComplexControl(QStyle::CC_ScrollBar, &option, event->pos().toPoint(), 0);
    d->hoveredControl = control;
    update();
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_D(QtGraphicsScrollBar);
    QStyleOptionSlider option;
    initStyleOption(&option);
    QStyle::SubControl control = style()->hitTestComplexControl(QStyle::CC_ScrollBar, &option, event->pos().toPoint(), 0);
    d->hoveredControl = control;
    update();
}

/*!
  \reimp
 */
void QtGraphicsScrollBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    Q_D(QtGraphicsScrollBar);
    d->hoveredControl = QStyle::SC_None;
    update();
}

/*!
 */
void QtGraphicsScrollBar::initStyleOption(QStyleOptionSlider *option) const
{
    QGraphicsWidget::initStyleOption(option);
    Q_D(const QtGraphicsScrollBar);
    option->rect = QRect(QPoint(0, 0), size().toSize());
    option->subControls = QStyle::SC_None;
    option->activeSubControls = QStyle::SC_None;
    option->orientation = d->orientation;
    option->minimum = d->minimum;
    option->maximum = d->maximum;
    option->sliderPosition = d->value;//d->position;
    option->sliderValue = d->value;
    option->singleStep = d->singleStep;
    option->pageStep = d->pageStep;
    //option->upsideDown = d->invertedAppearance;
    if (d->orientation == Qt::Horizontal)
        option->state |= QStyle::State_Horizontal;
    if (isActiveWindow())
        option->state |= QStyle::State_Active;
}

QT_END_NAMESPACE

#include "moc_qgraphicsscrollbar.cpp"
