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

#include "qkineticlistcontroller.h"
#include "qkineticlistcontroller_p.h"
#include <qgraphicssceneevent.h>
#include <qdatetime.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

QtKineticListControllerPrivate::QtKineticListControllerPrivate()
    : timerId(0), timerInterval(50), timestamp(0),
      scrollVelocity(0), maximumScrollVelocity(50),
      frictionFactor(0.85), velocityThreshold(0.05),
      movementThreshold(5),
      overshoot(false), centerOnItem(false)
{
}

QtKineticListControllerPrivate::~QtKineticListControllerPrivate()
{
}

bool QtKineticListControllerPrivate::updateOffset()
{
    if (!view)
        return false;
    qreal offset = view->offset();
    qreal max = view->maximumOffset();
    bool cont = kineticUpdateOffset(&offset, max);
    qreal value = overshoot ? offset : qBound<qreal>(0, offset, max);
    view->setOffset(value);
    return cont;
}

/*!
  \internal
  Update the offset and the velocity.
  
  return true if the timer should be continued
 */

bool QtKineticListControllerPrivate::kineticUpdateOffset(qreal *offset, qreal max)
{
    static const qreal k = -0.5; // overshoot spring constant
    static const qreal l = -0.02; // center spring constant

    *offset += scrollVelocity;
    
    // check for overshoot at the top
    if (*offset <= 0) {
        if (overshoot) {
            scrollVelocity = qMin(k * *offset, scrollVelocity + k * *offset);
        } else {
            *offset = 0;
            scrollVelocity = 0;
            return false;
        }
    }

    // check for overshoot at the bottom
    if (*offset >= max) {
        if (overshoot) {
            scrollVelocity = qMax(k * (*offset - max), scrollVelocity + k * (*offset - max));
        } else {
            scrollVelocity = 0;
            *offset = max;
            return false;
        }
    }

    // ### FIXME: this code may not work with small items
    // make sure that an item is at the center when we stop
    if (centerOnItem) {
        const QSizeF viewSize = view->size();
        const QPointF viewCenter(viewSize.width() / 2, viewSize.height() / 2);
        const int centerIndex = view->itemAt(viewCenter);
        const QPointF itemCenter = view->itemGeometry(centerIndex).center();
        const QPointF delta = viewCenter - itemCenter;
        if (view->orientation() == Qt::Horizontal)
            scrollVelocity += l * delta.x();
        else
            scrollVelocity += l * delta.y();
        scrollVelocity *= frictionFactor;
        return true;
    }

    // apply friction
    scrollVelocity *= frictionFactor;
    if (qAbs<qreal>(scrollVelocity) < velocityThreshold) {
        scrollVelocity = 0;
        return false;
    }
    return true;
}

void QtKineticListControllerPrivate::startTimer()
{
    Q_Q(QtKineticListController);
    if (timerId == 0)
        timerId = q->startTimer(timerInterval);
}

int QtKineticListControllerPrivate::currentTime() const
{
    QTime t = QTime::currentTime();
    return t.hour() * 3600000 + t.minute() * 60000 + t.second() * 1000 + t.msec();
}

/*!
    \class QtKineticListController
    \brief this is a list controller that implements kinetic scrolling

    \sa QtListWidgetNG
*/
QtKineticListController::QtKineticListController(QObject *parent)
    : QtListController(*(new QtKineticListControllerPrivate), parent)
{
    Q_D(QtKineticListController);
    d->startTimer();
}

/*!
 */
QtKineticListController::~QtKineticListController()
{
}

/*!
  Sets the scroll velocity to the given \a velocity.
  Note that setting this value wil also start the scrolling animation.
 */
void QtKineticListController::setScrollVelocity(qreal velocity)
{
    Q_D(QtKineticListController);
    d->scrollVelocity = qMin(velocity, d->maximumScrollVelocity);
    d->startTimer();
}

/*!
  Returns the scroll velocity.
 */
qreal QtKineticListController::scrollVelocity() const
{
    Q_D(const QtKineticListController);
    return d->scrollVelocity;
}

/*!
  Sets the maximum velocity allowed when scrolling is set to
  the given \a velocity.
 */
void QtKineticListController::setMaximumScrollVelocity(qreal velocity)
{
    Q_D(QtKineticListController);
    d->maximumScrollVelocity = velocity;
}

/*!
  Returns the maximum velocity allowed when scrolling.
 */
qreal QtKineticListController::maximumScrollVelocity() const
{
    Q_D(const QtKineticListController);
    return d->maximumScrollVelocity;
}

/*!
  Depending on the given \a enabled value, the controller will scroll past
  the first and last item.
 */
void QtKineticListController::setOvershootEnabled(bool enabled)
{
    Q_D(QtKineticListController);
    d->overshoot = enabled;
}

/*!
  Returns true if the controller is allowed to scroll past
  the first and last item, otherwise returns false.
 */
bool QtKineticListController::isOvershootEnabled() const
{
    Q_D(const QtKineticListController);
    return d->overshoot;
}

/*!
  Depending on the given \a enable value, the controller will ensure
  that when the scrolling stops, an item will be in the center of the view.
 */
void QtKineticListController::setCenterOnItemEnabled(bool enable)
{
    Q_D(QtKineticListController);
    d->centerOnItem = enable;
}

/*!
  Returns true if the controller ensures that an item is in the center
  of the view when the scrolling stops, otherwise returns false.
 */
bool QtKineticListController::isCenterOnItemEnabled() const
{
    Q_D(const QtKineticListController);
    return d->centerOnItem;
}

/*!
 */
void QtKineticListController::stop()
{
    Q_D(QtKineticListController);

    killTimer(d->timerId);
    d->timerId = 0;

    d->timestamp = d->currentTime();
    d->timeDelta = 0;
    d->movement = 0.;

    d->scrollVelocity = 0;
}

/*!
  \reimp
 */
bool QtKineticListController::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    Q_D(QtKineticListController);
    killTimer(d->timerId);
    d->timerId = 0;
    return true;
}

/*!
  \reimp
 */
bool QtKineticListController::mouseMoveEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    Q_D(QtKineticListController);
    if (!d->view)
        return false;

    int oldTimestamp = d->timestamp;
    d->timestamp = d->currentTime();
    d->timeDelta = d->timestamp - oldTimestamp;

    if (d->view->orientation() == Qt::Horizontal)
        d->movement = event->lastPos().x() - event->pos().x();
    else
        d->movement = event->lastPos().y() - event->pos().y();

    qreal offset = d->view->offset() + d->movement;
    qreal max = d->view->maximumOffset();
    d->view->setOffset(d->overshoot ? offset : qBound<qreal>(0, offset, max));

    return true;
}

/*!
  \reimp
 */
bool QtKineticListController::mousePressEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    Q_D(QtKineticListController);

    if (!d->centerOnItem)
        d->scrollVelocity = 0;

    killTimer(d->timerId);
    d->timerId = 0;

    d->timestamp = d->currentTime();
    d->timeDelta = 0;
    d->movement = 0.;

    stop();

    return true;
}

/*!
  \reimp
 */
bool QtKineticListController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, const QTransform &transform)
{
    Q_D(QtKineticListController);
    if (!d->view)
        return false;

    qreal dt = qreal(d->timeDelta) / qreal(d->timerInterval);
    QPointF dp = event->pos() - event->buttonDownPos(event->button());
    qreal totalMovement = (d->view->orientation() == Qt::Horizontal ? dp.x() : dp.y());

    if (qAbs<qreal>(totalMovement) > d->movementThreshold) {
        setScrollVelocity(d->movement / dt); // start kinetic scrolling
    } else {
        const qreal offset = d->view->offset();
        const qreal maximumOffset = d->view->maximumOffset();
        if (offset < 0 || offset > maximumOffset)
            d->startTimer(); // start bounce-back
        else if (d->centerOnItem && qAbs(d->scrollVelocity) > d->velocityThreshold)
            d->startTimer(); // continue scrolling
    }

    if (qAbs<qreal>(totalMovement) < d->movementThreshold)
        return QtListController::mouseReleaseEvent(event, transform);

    return true;
}

/*!
  \reimp
 */
bool QtKineticListController::wheelEvent(QGraphicsSceneWheelEvent *event, const QTransform &transform)
{
    Q_ASSERT(event);
    Q_UNUSED(transform);
    Q_D(QtKineticListController);
    if (d->view && d->view->orientation() == event->orientation())
        setScrollVelocity(d->scrollVelocity - event->delta() / 8.);
    return true;
}

/*!
  \reimp
 */
void QtKineticListController::timerEvent(QTimerEvent *event)
{
    Q_D(QtKineticListController);
    if (event->timerId() == d->timerId) {
        if (!d->updateOffset()) {
            killTimer(d->timerId);
            d->timerId = 0;
        }
    }
}

QT_END_NAMESPACE
