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

#include "qgraphicsheader.h"
#include "qgraphicsheader_p.h"

#include <qdebug.h>
#include <qvarlengtharray.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qapplication.h>
#include <qevent.h>

#include <qgraphicssceneevent.h>

QT_BEGIN_NAMESPACE

// QtGraphicsHeaderPrivate

/*!
  \internal
*/
QtGraphicsHeaderPrivate::QtGraphicsHeaderPrivate()
    : dataProvider(0),
      cachedDataIndex(-1),
      orientation(Qt::Horizontal),
      offset(0),
      firstVisualIndex(0),
      sectionCount(0),
      state(Idle),
      mousePressPosition(0),
      resizingVisualIndex(-1),
      resizingOriginalSize(0),
      movingVisualIndex(-1),
      movingSectionSize(0),
      movingCursorPosition(0),
      movingCursorOffset(0),
      movingCursorVisualIndex(-1),
      selectingVisualIndex(-1),
      q_ptr(0)
{
}

/*!
  \internal
*/
QtGraphicsHeaderPrivate::~QtGraphicsHeaderPrivate()
{
}

/*!
  \internal
  Maps \a pos to the internal coordinate system after applying
  the \a transform.

  Note that the difference between the external and internal
  coordinates is the offset and firstVisualIndex.
 */
qreal QtGraphicsHeaderPrivate::mapToInternal(const QPointF &pos, const QTransform &transform) const
{
    return ((orientation == Qt::Horizontal ? transform.map(pos).x() : transform.map(pos).y())
            + (firstVisualIndex != 0 ? sectionGeometry(firstVisualIndex).first : 0)
            + offset);
}

/*!
  \internal

  Handle the \a event. The event position is mapped
  using the \a transform to the internal coordinate system.
*/
bool QtGraphicsHeaderPrivate::mousePressEvent(QMouseEvent *event, const QTransform &transform)
{
    Q_Q(QtGraphicsHeader);
    if (event->buttons() & Qt::LeftButton) {
        q->update(); // ###
        const qreal pos = mapToInternal(event->pos(), transform);
        const int visualIndex = sectionAt(pos);
        const int handleVisualIndex = sectionHandleAt(pos, visualIndex);
        if (handleVisualIndex == -1) {
            if (isSectionClickable(visualIndex)) { // ### isSectionSelectable ?
                emit q->sectionPressed(visualIndex, event->modifiers());
                state = SelectingSections;
                selectingVisualIndex = visualIndex;
            } else {
                //qDebug() << "pressing section" << handleVisualIndex;
                state = PressingSection;
            }
        } else if (true /*sectionMode(handleVisualIndex) == QtGraphicsHeader::Interactive*/) {
            state = ResizingSection;
            resizingVisualIndex = handleVisualIndex;
            resizingOriginalSize = sectionSize(handleVisualIndex);
            q->setCursor(orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);
        }
        mousePressPosition = pos;
        return true;
    }
    return false;
}

bool QtGraphicsHeaderPrivate::mouseMoveEvent(QMouseEvent *event, const QTransform &transform)
{
    Q_Q(QtGraphicsHeader);
    if (event->buttons() & Qt::LeftButton) {
        q->update(); // ###
        const qreal pos = mapToInternal(event->pos(), transform);
        switch (state) {
        case ResizingSection: {
            const qreal delta = pos - mousePressPosition;
            const qreal size = resizingOriginalSize + delta;
            const qreal minimum = 0; // ###
            setSectionSize(resizingVisualIndex, qMax(size, minimum));
            return true; }
        case MovingSection: {
            const qreal cursorPosition = (orientation == Qt::Horizontal
                                          ? transform.map(event->pos()).x() - q->geometry().x()
                                          : transform.map(event->pos()).y() - q->geometry().y());
            movingCursorPosition = cursorPosition;
            movingCursorVisualIndex = sectionAt(pos);
            return true; }
        case SelectingSections: {
            const int visualIndex = sectionAt(pos);
            if (visualIndex != selectingVisualIndex)
                emit q->sectionDragSelected(visualIndex, event->modifiers());
            selectingVisualIndex = visualIndex;
            return true; }
        case PressingSection: {
#ifndef QT_NO_CURSOR
            const int sectionHandle = sectionHandleAt(pos);
            q->setCursor(orientation ? Qt::SplitHCursor : Qt::SplitVCursor);
            Q_UNUSED(sectionHandle);
            if (sectionHandle != -1)
                return true;
#endif//QT_NO_CURSOR
            // this is where state can change to MovingSection
            const int visualIndex = sectionAt(pos);
            const qreal cursorPosition = (orientation == Qt::Horizontal
                                          ? transform.map(event->pos()).x() - q->geometry().x()
                                          : transform.map(event->pos()).y() - q->geometry().y());
            const qreal distanceMoved = qAbs<qreal>(cursorPosition - mousePressPosition);
            if (distanceMoved >= QApplication::startDragDistance() && isSectionMovable(visualIndex)) {
                state = MovingSection;
                movingCursorPosition = cursorPosition;
                movingVisualIndex = visualIndex;
                const QPair<qreal, qreal> geometry = sectionGeometry(visualIndex);
                movingSectionSize = geometry.second;
                movingCursorOffset = pos - geometry.first;
                movingCursorVisualIndex = visualIndex;
            }
            return true; }
        case Idle:
        default:
            break;
        }
    }
    return false;
}

bool QtGraphicsHeaderPrivate::mouseReleaseEvent(QMouseEvent *event, const QTransform &transform)
{
    Q_Q(QtGraphicsHeader);
    if (event->button() == Qt::LeftButton) {
        q->update(); // ###
        qreal pos = mapToInternal(event->pos(), transform);
        const int visualIndex = sectionAt(pos);
        // ### TODO: make sure it is the same as pressed item
        switch (state) {
        case PressingSection:
        case Idle: {
            if (isSectionClickable(visualIndex))
                emit q->sectionClicked(visualIndex, event->modifiers());
            return true; }
        case ResizingSection: {
            resizingVisualIndex = -1;
            resizingOriginalSize = 0;
            state = Idle;
            q->unsetCursor();
            // if we are scrolling by setting the firstVisualIndex we may
            // need to recalculate the maxFirstVisualIndex after a resize
            return true; }
        case MovingSection: {
            q->moveSection(movingVisualIndex, visualIndex);
            movingVisualIndex = -1;
            movingCursorPosition = 0;
            movingCursorOffset = 0;
            movingCursorVisualIndex = -1;
            state = Idle;
            return true; }
        case SelectingSections: {
            if (isSectionClickable(visualIndex)) // ### isSectionSelectable
                emit q->sectionReleased(visualIndex, event->modifiers());
            selectingVisualIndex = -1;
            state = Idle;
            return true; }
        }
    }
    return false;
}

bool QtGraphicsHeaderPrivate::mouseDoubleClickEvent(QMouseEvent *event, const QTransform &transform)
{
    Q_Q(QtGraphicsHeader);
    if (event->button() == Qt::LeftButton) {
        // ### TODO: make sure it is the same as pressed item
        const qreal pos = mapToInternal(event->pos(), transform);
        const int index = sectionAt(pos);
        emit q->sectionDoubleClicked(index, event->modifiers());
        return true;
    }
    return false;
}

bool QtGraphicsHeaderPrivate::dragEnterEvent(QDragEnterEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

bool QtGraphicsHeaderPrivate::dragLeaveEvent(QDragLeaveEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

bool QtGraphicsHeaderPrivate::dragMoveEvent(QDragMoveEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

bool QtGraphicsHeaderPrivate::dropEvent(QDropEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

bool QtGraphicsHeaderPrivate::hoverEnterEvent(QHoverEvent *event, const QTransform &transform)
{
    Q_Q(QtGraphicsHeader);
    const qreal pos = mapToInternal(event->pos(), transform);
    const int visualIndex = sectionAt(pos);
    const int handleVisualIndex = sectionHandleAt(pos, visualIndex);
    if (handleVisualIndex == -1)
        q->unsetCursor();
    else
        q->setCursor(orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);
    return false;
}

bool QtGraphicsHeaderPrivate::hoverMoveEvent(QHoverEvent *event, const QTransform &transform)
{
    Q_Q(QtGraphicsHeader);
    const qreal pos = mapToInternal(event->pos(), transform);
    const int visualIndex = sectionAt(pos);
    const int handleVisualIndex = sectionHandleAt(pos, visualIndex);
    if (handleVisualIndex == -1)
        q->unsetCursor();
    else
        q->setCursor(orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);
    return false;
}

bool QtGraphicsHeaderPrivate::hoverLeaveEvent(QHoverEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    Q_Q(QtGraphicsHeader);
    q->unsetCursor();
    return false;
}

bool QtGraphicsHeaderPrivate::resizeEvent(QResizeEvent *event, const QTransform &transform)
{
    Q_UNUSED(event);
    Q_UNUSED(transform);
    return false;
}

void QtGraphicsHeaderPrivate::insertSections(int visualIndex, int count)
{
    const qreal defaultSectionSize = (orientation == Qt::Horizontal ? 100 : 30); // ###
    sectionSizeSpans.insertSectionsAt(defaultSectionSize, visualIndex, count);
    sectionModeSpans.insertSectionsAt(0/*defaultMode*/, visualIndex, count);
    sectionMovableSpans.insertSectionsAt(true, visualIndex, count);
    sectionClickableSpans.insertSectionsAt(true, visualIndex, count);
    sectionSelectedSpans.insertSectionsAt(false, visualIndex, count);
    sectionCount += count;
}

void QtGraphicsHeaderPrivate::removeSections(int visualIndex, int count)
{
    sectionSizeSpans.removeSectionsAt(visualIndex, count);
    sectionModeSpans.removeSectionsAt(visualIndex, count);
    sectionMovableSpans.removeSectionsAt(visualIndex, count);
    sectionClickableSpans.removeSectionsAt(visualIndex, count);
    sectionSelectedSpans.removeSectionsAt(visualIndex, count);
    sectionCount -= count;
}

bool QtGraphicsHeaderPrivate::isSectionCurrent(int visualIndex) const
{
    Q_UNUSED(visualIndex);
    return false;
}

bool QtGraphicsHeaderPrivate::isSectionSelected(int visualIndex) const
{
    Q_UNUSED(visualIndex);
    return sectionSelectedSpans.sectionValue(visualIndex, false);
}

bool QtGraphicsHeaderPrivate::isSectionHighlighted(int visualIndex) const
{
    Q_UNUSED(visualIndex);
    return false;
}

bool QtGraphicsHeaderPrivate::isSectionUnderMouse(int visualIndex) const
{
    Q_UNUSED(visualIndex);
    return false;
}

bool QtGraphicsHeaderPrivate::isSectionPressed(int visualIndex) const
{
    Q_UNUSED(visualIndex);
    return false;
}

bool QtGraphicsHeaderPrivate::isSectionHidden(int visualIndex) const
{
    return sectionSize(visualIndex) == 0;
}

bool QtGraphicsHeaderPrivate::isSectionClickable(int visualIndex) const
{
    return sectionClickableSpans.sectionValue(visualIndex, false);
}

bool QtGraphicsHeaderPrivate::isSectionMovable(int visualIndex) const
{
    return sectionMovableSpans.sectionValue(visualIndex, false);
}

bool QtGraphicsHeaderPrivate::isSortIndicatorShown(int visualIndex) const
{
    Q_UNUSED(visualIndex);
    return false;
}

/*!
  \internal

  Returns the sort indicator order set on the section at \a visualIndex.
  The default value is Qt::AscendingOrder.

  \sa isSortIndicatorShown
 */

Qt::SortOrder QtGraphicsHeaderPrivate::sortIndicatorOrder(int visualIndex) const
{
    Q_UNUSED(visualIndex);
    return Qt::AscendingOrder;
}

/*!
  \internal

  Returns the total size of all the sections.
 */
qreal QtGraphicsHeaderPrivate::contentSize() const
{
    qreal length = 0;
    for (int i = 0; i < sectionSizeSpans.count(); ++i) {
        const QtSectionSpans<qreal>::Span &span = sectionSizeSpans.at(i);
        const qreal sectionSize = span.value;
        uint spanSectionCount = span.count;
        length += (sectionSize * spanSectionCount);
    }
    return length;
}

/*!
  \internal

  Returns the section at the given \a position.
  If \a geometry pointer is given, this will be set to the position
  and size of the section found.

  Note that the position should already be mapped
  to the internal coordinate system.

  \sa mapToInternal
*/
int QtGraphicsHeaderPrivate::sectionAt(qreal position, QPair<qreal,qreal> *geometry) const
{
    int visualIndex = 0;
    qreal coordinate = 0;
    // find the section at position
    for (int i = 0; i < sectionSizeSpans.count(); ++i) {
        const QtSectionSpans<qreal>::Span &span = sectionSizeSpans.at(i);
        const qreal spanSize = (span.value * span.count);
        if (spanSize > 0 && position >= coordinate && position < coordinate + spanSize) {
            const qreal localPosition = position - coordinate;
            const qreal sectionSize = span.value;
            const int localIndex = int(localPosition / sectionSize);
            const qreal sectionPosition = localIndex * sectionSize;
            if (geometry)
                *geometry = QPair<qreal,qreal>(sectionPosition, sectionSize);
            return visualIndex + localIndex;
        }
        coordinate += spanSize;
        visualIndex += span.count; 
    }
    return -1;
}

/*!
  \internal

  Returns the section associated with the section resize
  handle at the given \a position.

  As an optimization, you can provide the \a visualIndex for
  the given \a position.

  Note that the position should already be mapped
  to the internal coordinate system.

  \sa mapToInternal
*/
int QtGraphicsHeaderPrivate::sectionHandleAt(qreal position, int visualIndex) const
{
    Q_Q(const QtGraphicsHeader);
    if (visualIndex == -1)
        visualIndex = sectionAt(position);
    const QPair<qreal, qreal> geometry = sectionGeometry(visualIndex);
    const qreal sectionPos = geometry.first;
    const qreal sectionHandlePos = geometry.first + geometry.second;
    QWidget *widget = 0;
    const qreal gripMargin = q->style()->pixelMetric(QStyle::PM_HeaderGripMargin, 0, widget);
    // are we at the beginning of the section ?
    if (position < sectionPos + gripMargin && visualIndex > 0) {
        while (isSectionHidden(visualIndex - 1))
            --visualIndex;
        return visualIndex - 1;
    }
    // are we at the end of the section ?
    if (position > sectionHandlePos - gripMargin) {
        return visualIndex;
    }
    return -1;
}

/*!
  \internal

  Returns a pair containing the position and size of the
  section at \a visualIndex.

  The position is in the internal coordinat system.

  \sa sectionSize()
*/
QPair<qreal, qreal> QtGraphicsHeaderPrivate::sectionGeometry(int visualIndex) const
{
    int sectionStart = 0;
    qreal spanPosition = 0;
    for (int i = 0; i < sectionSizeSpans.count(); ++i) {
        const QtSectionSpans<qreal>::Span &span = sectionSizeSpans.at(i);
        const int sectionEnd = sectionStart + span.count - 1;
        if (visualIndex >= sectionStart && visualIndex <= sectionEnd) {
            const qreal sectionSize = span.value;
            const qreal sectionPosition =  (spanPosition + (visualIndex - sectionStart) * sectionSize);
            return QPair<qreal, qreal>(sectionPosition, sectionSize);
        }
        sectionStart = sectionEnd + 1;
        spanPosition += (span.value * span.count);
    }
    qWarning() << "no section geometry found for visual index" << visualIndex;
    return QPair<qreal, qreal>(-1, -1);
}

/*!
  \internal

  Returns the size of the section at \a visualIndex.

  \sa sectionGeometry()
*/
qreal QtGraphicsHeaderPrivate::sectionSize(int visualIndex) const
{
    return sectionSizeSpans.sectionValue(visualIndex, 0);
}

/*!
  \internal

  Returns the resize mode assigned to the section at \a visualIndex.

  \sa sectionSize  
 */
uint QtGraphicsHeaderPrivate::sectionMode(int visualIndex) const // ###
{
    return sectionModeSpans.sectionValue(visualIndex, 0);
}

/*!
  \internal

  Sets the section count to be \a count.
  Also ensures that all the internal structures are updated.
*/
void QtGraphicsHeaderPrivate::setSectionCount(int count)
{
    const int difference = count - sectionCount;
    if (difference == 0)
        return;

    sectionCount = count;

    if (count == 0) {
        sectionSizeSpans.clear();
        sectionModeSpans.clear();
        sectionMovableSpans.clear();
        sectionClickableSpans.clear();
        sectionSelectedSpans.clear();
        visualToLogical.clear();
        logicalToVisual.clear();
        return;
    }

    // do we need to add sections ?
    if (difference > 0) {
        qreal defaultSectionSize = (orientation == Qt::Horizontal ? 100 : 30); // ###
        sectionSizeSpans.addSections(defaultSectionSize, difference);
        sectionModeSpans.addSections(0 /*defaultMode*/, difference); // ###
        sectionMovableSpans.addSections(true, difference);
        sectionClickableSpans.addSections(true, difference);
        sectionSelectedSpans.addSections(false, difference);
    } else { // we need to remove sections
        sectionSizeSpans.removeSections(-difference);
        sectionModeSpans.removeSections(-difference);
        sectionMovableSpans.removeSections(-difference);
        sectionClickableSpans.removeSections(-difference);
        sectionSelectedSpans.removeSections(-difference);
    }

    // update the index mapping
    if (!visualToLogical.isEmpty() || !logicalToVisual.isEmpty()) {
        visualToLogical.resize(count);
        logicalToVisual.resize(count);
        for (int i = count - difference; i < sectionCount; ++i) {
            visualToLogical[i] = i;
            logicalToVisual[i] = i;
        }
    }
}

/*!
  \internal
  Sets the size of the section at \a visualIndex to \a size.

  This function will insert a new span in the size span list.
  It will try to merge the new span with the adjacent spans
  if possible.

  \sa sectionSize
*/
void QtGraphicsHeaderPrivate::setSectionSize(int visualIndex, qreal size)
{
    sectionSizeSpans.setValue(visualIndex, size);
}

/*!
  \internal
  Sets the mode of the section at \a visualIndex to \a mode.

  This function will insert a new span in the mode span list.
  It will try to merge the new span with the adjacent spans
  if possible.
*/
void QtGraphicsHeaderPrivate::setSectionMode(int visualIndex, uint mode) // ####
{
    sectionModeSpans.setValue(visualIndex, mode);
}

QVariant QtGraphicsHeaderPrivate::data(int logicalIndex, const QByteArray &role) const
{
    // ### structure this list so the most used items are added first
    static const QList<QByteArray> roles = (QList<QByteArray>()
                                     //<< "FontRole"
                                     //<< "TextAlignmentRole"
                                     //<< "ForegroundRole"
                                     //<< "CheckStateRole"
                                     << "DecorationRole"
                                     << "DisplayRole");
    if (!dataProvider)
        return (role == "DisplayRole" ? logicalIndex : QVariant());
    if (logicalIndex != cachedDataIndex) {
        cachedDataHash = dataProvider->data(logicalIndex, roles);
        cachedDataIndex = logicalIndex;
    }
    return cachedDataHash.value(role);
}


// QtGraphicsHeaderDataProvider
QtGraphicsHeaderDataProvider::~QtGraphicsHeaderDataProvider()
{
}

// QtGraphicsHeader

/*!
  \class QtGraphicsHeader
  \brief A header control to be used to decorate either a QTreeControl or a QTableControl
*/

/*!
 */
QtGraphicsHeader::QtGraphicsHeader(Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(new QtGraphicsHeaderPrivate)
{
    d_ptr->q_ptr = this;
    setOrientation(orientation);
    setAcceptHoverEvents(true);
}

/*!
 */
QtGraphicsHeader::QtGraphicsHeader(QtGraphicsHeaderPrivate &dd, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
    setAcceptHoverEvents(true);
}

/*!
 */
QtGraphicsHeader::~QtGraphicsHeader()
{
    delete d_ptr;
}

/*!
 */
void QtGraphicsHeader::setDataProvider(QtGraphicsHeaderDataProvider *provider)
{
    Q_D(QtGraphicsHeader);
    d->dataProvider = provider;
}

/*!
 */
QtGraphicsHeaderDataProvider *QtGraphicsHeader::dataProvider() const
{
    Q_D(const QtGraphicsHeader);
    return d->dataProvider;
}

/*!
  Returns the orientation set on the header.
 */
Qt::Orientation QtGraphicsHeader::orientation() const
{
    Q_D(const QtGraphicsHeader);
    return d->orientation;
}

/*!
  Sets the \a orientation of the header.
 */
void QtGraphicsHeader::setOrientation(Qt::Orientation orientation)
{
    Q_D(QtGraphicsHeader);
    d->orientation = orientation;
}

/*!
  Returns the offset used by the header when drawing its contents.
 */
qreal QtGraphicsHeader::offset() const
{
    Q_D(const QtGraphicsHeader);
    return d->offset;
}

/*!
  Sets the offset used by the header when drawing its contents to \a offset.
 */
void QtGraphicsHeader::setOffset(qreal offset)
{
    Q_D(QtGraphicsHeader);
    d->offset = offset;
    emit offsetChanged(offset);
    update();
}

/*!
  Returns the maximum allowed offset for the header.
 */
qreal QtGraphicsHeader::maximumOffset() const
{
    Q_D(const QtGraphicsHeader);
    return d->contentSize() - (d->orientation == Qt::Horizontal ? geometry().width() : geometry().height());
}

/*!
  Returns the size of the header perpendicular to its orientation.
 */
qreal QtGraphicsHeader::perpendicularSize() const
{
    Q_D(const QtGraphicsHeader);
    QStyleOptionHeader option;
    initStyleOption(&option, 0);
    /*
    qreal width = 0;
    qreal height = 0;
    for (int visualIndex = 0; visualIndex < d->sectionCount; ++visualIndex) {
        int section = mapToLogicalIndex(visualIndex);
        initStyleOption(&option, section);
        QSizeF hint = sectionSizeHint(&option, section);
        width = qMax<qreal>(width, hint.width());
        height = qMax<qreal>(height, hint.height());
    }
    */
    const int section = sectionCount() - 1;
    initStyleOption(&option, section);
    QSizeF hint = sectionSizeHint(&option, section);
    return (d->orientation == Qt::Horizontal ? hint.height() : hint.width());
}

/*!
  Returns the size of the contents in the header.
 */
qreal QtGraphicsHeader::contentSize() const
{
    Q_D(const QtGraphicsHeader);
    return d->contentSize();
}

/*!
  Returns the number of sections in the header.
 */
int QtGraphicsHeader::sectionCount() const
{
    Q_D(const QtGraphicsHeader);
    return d->sectionCount;
}

/*!
  Returns the number of sections in the given visible \a visibleLength of the header.
 */
int QtGraphicsHeader::visibleSectionCount(qreal visibleLength) const
{
    Q_D(const QtGraphicsHeader);
    if (visibleLength <= 0)
        return 0;
    const qreal offset = (d->firstVisualIndex != 0 ? d->sectionGeometry(d->firstVisualIndex).first : 0) + d->offset;
    const int section = d->sectionAt(visibleLength + offset - 1);
    return (section == -1 ? d->sectionCount - 1 : section) - d->firstVisualIndex + 1;
}

/*!
  Sets the number of sections in the header to \a count.
 */
void QtGraphicsHeader::setSectionCount(int count)
{
    Q_D(QtGraphicsHeader);
    const int oldCount = d->sectionCount;
    d->setSectionCount(count);
    emit sectionCountChanged(count, oldCount);
}

/*!
  Returns the visual index of the first section.
 */
int QtGraphicsHeader::firstSection() const
{
    Q_D(const QtGraphicsHeader);
    return d->firstVisualIndex;
}

/*!
  Sets the first section to be drawn to the section indicated
  by the given \a visualIndex.
 */
void QtGraphicsHeader::setFirstSection(int visualIndex)
{
    Q_D(QtGraphicsHeader);
    d->firstVisualIndex = visualIndex;
    emit firstSectionChanged(visualIndex);
    update();
}

/*!
  Returns the maximum allowed value for first section in the header.
 */
int QtGraphicsHeader::maximumFirstSection() const
{
    // ### optimize to use spans, and start from last
    Q_D(const QtGraphicsHeader);
    if (d->sectionCount <= 0)
        return 0;

    qreal size = (d->orientation == Qt::Horizontal ? geometry().width() : geometry().height()) + d->offset;
    int visualIndex = d->sectionCount - 1;
    for (; visualIndex >= 0; --visualIndex) {
        size -= d->sectionSize(visualIndex);
        if (size <= 0) { // overshoot
            ++visualIndex;
            break;
        }
    }
    return visualIndex;
}

/*!
  Returns the visual index that corresponds to the given \a logicalIndex.
 */
int QtGraphicsHeader::mapToVisualIndex(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    if (d->logicalToVisual.isEmpty()) // no mapping
        return logicalIndex;
    if (logicalIndex >= 0 && logicalIndex < d->logicalToVisual.count())
        return d->logicalToVisual.at(logicalIndex);
    return -1;
}

/*!
  Returns the logical index that corresponds to the given \a visualIndex.
 */
int QtGraphicsHeader::mapToLogicalIndex(int visualIndex) const
{
    Q_D(const QtGraphicsHeader);
    if (d->visualToLogical.isEmpty()) // no mapping
        return visualIndex;
    if (visualIndex >= 0 && visualIndex < d->visualToLogical.count())
        return d->visualToLogical.at(visualIndex);
    return -1;
}

/*!
  Sets the mapping between visual indexes and logical indexes to be the
  given \a visualToLogical vector.
 */
void QtGraphicsHeader::setIndexMapping(const QVector<int> &visualToLogical)
{
    Q_D(QtGraphicsHeader);
    // ### ensure that the vector count is the same as sectionCount
    d->visualToLogical = visualToLogical;
    int count = visualToLogical.count();
    QVector<int> logicalToVisual(count);
    for (int i = 0; i < count; ++i)
        logicalToVisual[visualToLogical.at(i)] = i;
    d->logicalToVisual = logicalToVisual;
    //emit indexMappingChanged();
}

/*!
  Returns the currently used mapping between visual indexes and logical indexes.
 */
const QVector<int> &QtGraphicsHeader::indexMapping() const
{
    // ### we only return the visualToLogical mapping
    Q_D(const QtGraphicsHeader);
    return d->visualToLogical;
}

/*!
  Returns the sort order set for the section indicated by the given \a logicalIndex.
 */
Qt::SortOrder QtGraphicsHeader::sortIndicatorOrder(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->sortIndicatorOrder(mapToVisualIndex(logicalIndex));
}

/*!
  Sets the sort order for the section indicated by \a logicalIndex to be
  the given \a order.
 */
void QtGraphicsHeader::setSortIndicatorOrder(int logicalIndex, Qt::SortOrder order)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(order);
}

/*!
  Returns true if the sort indicator is shown for the section indicated by the
  given \a logicalIndex, otherwise returns false.
 */
bool QtGraphicsHeader::isSortIndicatorShown(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->isSortIndicatorShown(mapToVisualIndex(logicalIndex));
}

/*!
  Sets the sort indicator to be \a shown for the section indicated by the
  given \a logicalIndex.
 */
void QtGraphicsHeader::setSortIndicatorShown(int logicalIndex, bool show)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(show);
}

/*!
  Returns the size of the given \a logicalIndex, in the direction
  of the header orientation.

  \sa orientation()
 */
qreal QtGraphicsHeader::sectionSize(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->sectionSize(mapToVisualIndex(logicalIndex));
}

/*!
  Sets the size of the section specified by \a logicalIndex to
  the given \a size.

  \sa sectionResized(), sectionSize()
 */
void QtGraphicsHeader::setSectionSize(int logicalIndex, qreal size)
{
    Q_D(QtGraphicsHeader);
    const int visualIndex = mapToVisualIndex(logicalIndex);
    const qreal oldSize = d->sectionSize(visualIndex);
    if (size != oldSize) {
        d->sectionSizeSpans.setValue(visualIndex, size);
        emit sectionResized(logicalIndex, size, oldSize);
        update();
    }
}

/*!
  Returns the position of the given \a logicalIndex, in the direction
  of the header orientation.

  \sa orientation()
 */
qreal QtGraphicsHeader::sectionPosition(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->sectionGeometry(mapToVisualIndex(logicalIndex)).first;
}

/*!
  Returns the logicalIndex of the section at the given \a position.

  \sa sectionPosition()
 */
int QtGraphicsHeader::sectionAt(const QPointF &position, const QTransform &transform) const
{
    Q_D(const QtGraphicsHeader);
    const qreal pos = d->mapToInternal(position, transform);
    return mapToLogicalIndex(d->sectionAt(pos));
}

/*!
  Returns the position and size of the section at the given \a position.

  \sa sectionAt(), sectionPosition(), sectionSize()
 */
QPair<qreal,qreal> QtGraphicsHeader::sectionGeometryAt(const QPointF &position, const QTransform &transform) const
{
    Q_D(const QtGraphicsHeader);
    const qreal pos = d->mapToInternal(position, transform);
    QPair<qreal,qreal> geometry;
    d->sectionAt(pos, &geometry);
    return geometry;
}

/*!
  Moves the section at \a visualIndexFrom to occupy \a visualIndexTo.

  \sa sectionsMoved()
 */
void QtGraphicsHeader::moveSection(int visualIndexFrom, int visualIndexTo)
{
    Q_D(QtGraphicsHeader);

    if (visualIndexFrom == visualIndexTo)
        return;

    int *visualToLogical = 0;
    int *logicalToVisual = 0;

    // initialize the index mapping
    if (d->visualToLogical.isEmpty() || d->logicalToVisual.isEmpty()) {
        // NOTE: resize might segfault if sectionCount is too large
        // ### find a way to catch this and recover
        d->visualToLogical.resize(d->sectionCount);
        d->logicalToVisual.resize(d->sectionCount);
        visualToLogical = d->visualToLogical.data();
        logicalToVisual = d->logicalToVisual.data();
        for (int i = 0; i < d->sectionCount; ++i) {
            visualToLogical[i] = i;
            logicalToVisual[i] = i;
        }
    } else {
        visualToLogical = d->visualToLogical.data();
        logicalToVisual = d->logicalToVisual.data();
    }

    const int logicalIndexFrom = visualToLogical[visualIndexFrom];
    const int affectedSectionsCount = qAbs(visualIndexTo - visualIndexFrom) + 1;
    QVarLengthArray<qreal> sizes(affectedSectionsCount);
    QVarLengthArray<uint> modes(affectedSectionsCount); // ###

    // update the index mapping
    if (visualIndexTo > visualIndexFrom) {
        sizes[visualIndexTo - visualIndexFrom] = d->sectionSize(visualIndexFrom);
        modes[visualIndexTo - visualIndexFrom] = d->sectionMode(visualIndexFrom);
        int visualIndex = visualIndexFrom;
        while (visualIndex < visualIndexTo) {
            sizes[visualIndex - visualIndexFrom] = d->sectionSize(visualIndex + 1);
            modes[visualIndex - visualIndexFrom] = d->sectionMode(visualIndex + 1);
            int logicalIndex = visualToLogical[visualIndex + 1];
            visualToLogical[visualIndex] = logicalIndex;
            logicalToVisual[logicalIndex] = visualIndex;
            ++visualIndex;
        }
    } else {
        sizes[0] = d->sectionSize(visualIndexFrom);
        modes[0] = d->sectionMode(visualIndexFrom);
        int visualIndex = visualIndexFrom;
        while (visualIndex > visualIndexTo) {
            sizes[visualIndex - visualIndexTo] = d->sectionSize(visualIndex - 1);
            modes[visualIndex - visualIndexTo] = d->sectionMode(visualIndex - 1);
            int logicalIndex = visualToLogical[visualIndex - 1];
            visualToLogical[visualIndex] = logicalIndex;
            logicalToVisual[logicalIndex] = visualIndex;
            --visualIndex;
        }
    }

    visualToLogical[visualIndexTo] = logicalIndexFrom;
    logicalToVisual[logicalIndexFrom] = visualIndexTo;

    // move the section sizes and modes
    if (visualIndexTo > visualIndexFrom) {
        for (int visualIndex = visualIndexFrom; visualIndex <= visualIndexTo; ++visualIndex) {
            d->setSectionSize(visualIndex, sizes[visualIndex - visualIndexFrom]);
            d->setSectionMode(visualIndex, modes[visualIndex - visualIndexFrom]);
        }
    } else {
        for (int visualIndex = visualIndexTo; visualIndex <= visualIndexFrom; ++visualIndex) {
            d->setSectionSize(visualIndex, sizes[visualIndex - visualIndexTo]);
            d->setSectionMode(visualIndex, modes[visualIndex - visualIndexTo]);
        }
    }

    emit sectionMoved(logicalIndexFrom, visualIndexTo, visualIndexFrom);
}

/*!
  Returns true if the section indicated by \a logicalIndex is hidden,
  otherwise returns false.
 */
bool QtGraphicsHeader::isSectionHidden(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->isSectionHidden(mapToVisualIndex(logicalIndex));
}

/*!
  Sets the section indicated by \a logicalIndex to be \a hidden.
 */
void QtGraphicsHeader::setSectionHidden(int logicalIndex, bool hidden)
{
    //Q_D(QtGraphicsHeader);
    Q_UNUSED(logicalIndex);
    Q_UNUSED(hidden);
    // ### if hide save size and set size to 0 else set size to saved size
}

/*!
  Returns true if the section indicated by \a logicalIndex is movable,
  otherwise returns false.
 */
bool QtGraphicsHeader::isSectionMovable(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->isSectionMovable(mapToVisualIndex(logicalIndex));
}

/*!
  Sets the section indicated by \a logicalIndex to be \a movable.
 */
void QtGraphicsHeader::setSectionMovable(int logicalIndex, bool movable)
{
    Q_D(QtGraphicsHeader);
    if (logicalIndex >= 0)
        d->sectionMovableSpans.setValue(mapToVisualIndex(logicalIndex), movable);
    else
        d->sectionMovableSpans.replaceAll(movable);
}

/*!
  Returns true if the section indicated by \a logicalIndex is clickable,
  otherwise returns false.
 */
bool QtGraphicsHeader::isSectionClickable(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->isSectionClickable(mapToVisualIndex(logicalIndex));
}

/*!
  Sets the section indicated by \a logicalIndex to be \a clickable.
 */
void QtGraphicsHeader::setSectionClickable(int logicalIndex, bool clickable)
{
    Q_D(QtGraphicsHeader);
    if (logicalIndex >= 0)
        d->sectionClickableSpans.setValue(mapToVisualIndex(logicalIndex), clickable);
    else
        d->sectionClickableSpans.replaceAll(clickable);
}

/*!
  Returns true if the section indicated by \a logicalIndex is selected,
  otherwise returns false.
 */
bool QtGraphicsHeader::isSectionSelected(int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    return d->isSectionSelected(mapToVisualIndex(logicalIndex));
}

/*!
  Sets the selected state section indicated by \a logicalIndex
  to \a selected.
 */
void QtGraphicsHeader::setSectionSelected(int logicalIndex, bool selected)
{
    Q_D(QtGraphicsHeader);
    if (logicalIndex >= 0)
        d->sectionSelectedSpans.setValue(mapToVisualIndex(logicalIndex), selected);
    else
        d->sectionSelectedSpans.replaceAll(selected);
}

/*!
  Returns true if the header control is not processing
  input events, otherwise returns false.

  This function is typically used to determine if the
  control is in the process of selecting, moving or
  resizing a section.
 */
bool QtGraphicsHeader::isIdle() const
{
    Q_D(const QtGraphicsHeader);
    return d->state == QtGraphicsHeaderPrivate::Idle;
}
/*!
  \reimpl

  \sa paintSection()
 */
void QtGraphicsHeader::paint(QPainter *painter, const QStyleOptionGraphicsItem *graphicsOption, QWidget *widget)
{
    Q_D(const QtGraphicsHeader);

    painter->setClipRect(boundingRect()); // ### use flags to enable clipping

    QStyleOptionHeader option;
    initStyleOption(&option, widget);
    copyStyleOptionState(graphicsOption, &option);
    
    const int first = d->firstVisualIndex;
    const int last = d->sectionCount - 1;
    const bool horizontal = (d->orientation == Qt::Horizontal);
    const qreal length = (horizontal ? geometry().width() : geometry().height());
    const qreal other = (horizontal ? geometry().height() : geometry().width());

    // paint the sections
    qreal pos = -d->offset;
    for (int visualIndex = first; visualIndex <= last && pos <= length; ++visualIndex) {
        int section = mapToLogicalIndex(visualIndex);
        qreal size = sectionSize(section);
        // make room for moving section
        if (visualIndex == d->movingCursorVisualIndex) {
            if (horizontal)
                option.rect = QRectF(pos, 0, d->movingSectionSize, other).toRect();
            else
                option.rect = QRectF(0, pos, other, d->movingSectionSize).toRect();
            paintEmptySection(painter, &option);
            pos += d->movingSectionSize;
        }
        // hide the moving section
        if (visualIndex == d->movingVisualIndex) {
            size = 0;
        }
        // paint the section
        if (size > 0 && pos + size > 0) {
            initStyleOption(&option, section);
            if (horizontal)
                option.rect = QRectF(pos, 0, size, other).toRect();
            else
                option.rect = QRectF(0, pos, other, size).toRect();
            paintSection(painter, &option, section);
            pos += size;
        }
    }

    // fill the remaining area
    const qreal remaining = length - pos;
    if (remaining > 0) {
        if (horizontal)
            option.rect = QRectF(pos, 0, remaining, other).toRect();
        else
            option.rect = QRectF(0, pos, other, remaining).toRect();
        paintEmptySection(painter, &option);
    }

    // paint the section being moved
    if (d->state == QtGraphicsHeaderPrivate::MovingSection) {
        int section = mapToLogicalIndex(d->movingVisualIndex);
        qreal size = sectionSize(section);
        qreal pos = d->movingCursorPosition - d->movingCursorOffset;
        initStyleOption(&option, section);
        if (horizontal)
            option.rect = QRectF(pos, 0, size, other).toRect();
        else
            option.rect = QRectF(0, pos, other, size).toRect();
        paintDragIndicator(painter, &option, section);
    }

    //painter->drawRect(boundingRect().adjusted(0, 0, -1, -1));// ### for debugging
}

/*!
 */
QSizeF QtGraphicsHeader::sectionSizeHint(QStyleOptionHeader *option,  int logicalIndex) const
{
    // ### ignores the SizeHintRole
    Q_D(const QtGraphicsHeader);
    Q_ASSERT(option);
    QSize size = style()->sizeFromContents(QStyle::CT_HeaderSection, option, QSize(), /*option->widget*/0);
    const int visualIndex = mapToVisualIndex(logicalIndex);
    if (d->isSortIndicatorShown(visualIndex)) {
        qreal indicator = size.height(); // ### indicator size
        qreal margin = style()->pixelMetric(QStyle::PM_HeaderMargin, option, /*option->widget*/0);
        if (d->orientation == Qt::Horizontal)
            size.setWidth(int(size.width() + margin + indicator));
        else
            size.setHeight(int(size.height() + margin + indicator));
    }
    return QSizeF(size); // ### cache ?
}

/*!
 */
bool QtGraphicsHeader::processEvent(QEvent *event, const QTransform &transform)
{
    Q_D(QtGraphicsHeader);
    Q_ASSERT(event);

    switch (event->type()) {        
    case QEvent::MouseButtonPress:
        return d->mousePressEvent(static_cast<QMouseEvent*>(event), transform);
    case QEvent::MouseMove:
        return d->mouseMoveEvent(static_cast<QMouseEvent*>(event), transform);
    case QEvent::MouseButtonRelease:
        return d->mouseReleaseEvent(static_cast<QMouseEvent*>(event), transform);
    case QEvent::MouseButtonDblClick:
        return d->mouseDoubleClickEvent(static_cast<QMouseEvent*>(event), transform);
#ifndef QT_NO_DRAGANDDROP
    case QEvent::DragEnter:
        return d->dragEnterEvent(static_cast<QDragEnterEvent*>(event), transform);
    case QEvent::DragLeave:
        return d->dragLeaveEvent(static_cast<QDragLeaveEvent*>(event), transform);
    case QEvent::DragMove:
        return d->dragMoveEvent(static_cast<QDragMoveEvent*>(event), transform);
    case QEvent::Drop:
        return d->dropEvent(static_cast<QDropEvent*>(event), transform);
#endif
    case QEvent::HoverEnter:
        return d->hoverEnterEvent(static_cast<QHoverEvent*>(event), transform);
    case QEvent::HoverMove:
        return d->hoverMoveEvent(static_cast<QHoverEvent*>(event), transform);
    case QEvent::HoverLeave:
        return d->hoverLeaveEvent(static_cast<QHoverEvent*>(event), transform);
    case QEvent::Resize:
        return d->resizeEvent(static_cast<QResizeEvent*>(event), transform);
    // ### converting graphics scene events
    case QEvent::GraphicsSceneMousePress: {
        QGraphicsSceneMouseEvent *ge = static_cast<QGraphicsSceneMouseEvent*>(event);
        QMouseEvent me(QEvent::MouseButtonPress, ge->pos().toPoint(), ge->button(), ge->buttons(), ge->modifiers());
        return d->mousePressEvent(&me, QTransform()); } // ###
    case QEvent::GraphicsSceneMouseMove: {
        QGraphicsSceneMouseEvent *ge = static_cast<QGraphicsSceneMouseEvent*>(event);
        QMouseEvent me(QEvent::MouseMove, ge->pos().toPoint(), ge->button(), ge->buttons(), ge->modifiers());
        return d->mouseMoveEvent(&me, QTransform()); } // ###
    case QEvent::GraphicsSceneMouseRelease: {
        QGraphicsSceneMouseEvent *ge = static_cast<QGraphicsSceneMouseEvent*>(event);
        QMouseEvent me(QEvent::MouseButtonRelease, ge->pos().toPoint(), ge->button(), ge->buttons(), ge->modifiers());
        return d->mouseReleaseEvent(&me, QTransform()); } // ###
// #ifndef QT_NO_WHEELEVENT
//     case QEvent::GraphicsSceneWheel: {
//          QGraphicsSceneWheelEvent *ge = static_cast<QGraphicsSceneWheelEvent*>(event);
//          QWheelEvent we(ge->pos().toPoint(), ge->delta(), ge->buttons(), ge->modifiers(), ge->orientation());
//          return d->wheelEvent(&we, QTransform()); } // ###
// #endif
    case QEvent::GraphicsSceneDragEnter: {
        QGraphicsSceneDragDropEvent *ge = static_cast<QGraphicsSceneDragDropEvent*>(event);
        QDragEnterEvent de(ge->pos().toPoint(), ge->dropAction(), ge->mimeData(), ge->buttons(), ge->modifiers());
        return d->dragEnterEvent(&de, QTransform()); }
    case QEvent::GraphicsSceneDragLeave: {
        //QGraphicsSceneDragDropEvent *ge = static_cast<QGraphicsSceneDragDropEvent*>(event);
        QDragLeaveEvent de;//(ge->pos().toPoint(), ge->dropAction(), ge->mimeData(), ge->buttons(), ge->modifiers());
        return d->dragLeaveEvent(&de, QTransform()); }
    case QEvent::GraphicsSceneDragMove: {
        QGraphicsSceneDragDropEvent *ge = static_cast<QGraphicsSceneDragDropEvent*>(event);
        QDragMoveEvent de(ge->pos().toPoint(), ge->dropAction(), ge->mimeData(), ge->buttons(), ge->modifiers());
        return d->dragMoveEvent(&de, QTransform()); }
    case QEvent::GraphicsSceneDrop: {
        QGraphicsSceneDragDropEvent *ge = static_cast<QGraphicsSceneDragDropEvent*>(event);
        QDropEvent de(ge->pos().toPoint(), ge->dropAction(), ge->mimeData(), ge->buttons(), ge->modifiers());
        return d->dropEvent(&de, QTransform()); }
    case QEvent::GraphicsSceneHoverEnter: {
        QGraphicsSceneHoverEvent *ge = static_cast<QGraphicsSceneHoverEvent*>(event);
        QHoverEvent he(ge->type(), ge->pos().toPoint(), ge->lastPos().toPoint());
        return d->hoverEnterEvent(&he, QTransform()); }
    case QEvent::GraphicsSceneHoverMove: {
        QGraphicsSceneHoverEvent *ge = static_cast<QGraphicsSceneHoverEvent*>(event);
        QHoverEvent he(ge->type(), ge->pos().toPoint(), ge->lastPos().toPoint());
        return d->hoverMoveEvent(&he, QTransform()); }
    case QEvent::GraphicsSceneHoverLeave: {
        QGraphicsSceneHoverEvent *ge = static_cast<QGraphicsSceneHoverEvent*>(event);
        QHoverEvent he(ge->type(), ge->pos().toPoint(), ge->lastPos().toPoint());
        return d->hoverLeaveEvent(&he, QTransform()); }
    case QEvent::GraphicsSceneResize: {
        QGraphicsSceneResizeEvent *ge = static_cast<QGraphicsSceneResizeEvent*>(event);
        QResizeEvent re(ge->newSize().toSize(), ge->oldSize().toSize());
        return d->resizeEvent(&re, QTransform()); }
    default:
        break;
    }
    return false;
}

/*!
 */
void QtGraphicsHeader::updateSections(int logicalIndex, int count)
{
    Q_UNUSED(logicalIndex);
    Q_UNUSED(count);
}

/*!
 */
void QtGraphicsHeader::insertSections(int logicalIndex, int count)
{
    Q_D(QtGraphicsHeader);
    d->insertSections(mapToVisualIndex(logicalIndex), count);
}

/*!
 */
void QtGraphicsHeader::removeSections(int logicalIndex, int count)
{
    Q_D(QtGraphicsHeader);
    d->removeSections(mapToVisualIndex(logicalIndex), count);
}

/*!
  Initialisze the given \a option struct with settings
  for this control.
 */
void QtGraphicsHeader::initStyleOption(QStyleOptionHeader *option, QWidget *widget) const
{
    Q_D(const QtGraphicsHeader);
    Q_ASSERT(option);
    
    if (widget)
        option->init(widget);
    else
        option->state |= QStyle::State_Enabled;

    if (d->orientation == Qt::Horizontal)
        option->state |= QStyle::State_Horizontal;
    else
        option->state &= ~QStyle::State_Horizontal;

    option->orientation = d->orientation;
    option->iconAlignment = Qt::AlignCenter;
    option->textAlignment = d->orientation == Qt::Horizontal
                            ? Qt::AlignHCenter|Qt::AlignVCenter
                            : Qt::AlignHCenter|Qt::AlignVCenter; // ###
}

/*!
  Initialize the given \a option struct with the settings and data
  specific for the section at the given \a logicalIndex.
 */
void QtGraphicsHeader::initStyleOption(QStyleOptionHeader *option, int logicalIndex) const
{
    Q_D(const QtGraphicsHeader);
    Q_ASSERT(option);
    
    option->section = logicalIndex;

    const int visual = d->logicalToVisual.value(logicalIndex, logicalIndex);

    if (d->sectionCount == 1)
        option->position = QStyleOptionHeader::OnlyOneSection;
    else if (visual == 0)
        option->position = QStyleOptionHeader::Beginning;
    else if (visual == d->sectionCount - 1)
        option->position = QStyleOptionHeader::End;
    else
        option->position = QStyleOptionHeader::Middle;

    const bool previousSelected = d->isSectionSelected(visual - 1);
    const bool nextSelected = d->isSectionSelected(visual + 1);

    if (previousSelected && nextSelected)
        option->selectedPosition = QStyleOptionHeader::NextAndPreviousAreSelected;
    else if (previousSelected)
        option->selectedPosition = QStyleOptionHeader::PreviousIsSelected;
    else if (nextSelected)
        option->selectedPosition = QStyleOptionHeader::NextIsSelected;
    else
        option->selectedPosition = QStyleOptionHeader::NotAdjacent;

    if (d->isSortIndicatorShown(visual)) {
        if (d->sortIndicatorOrder(visual) == Qt::AscendingOrder)
            option->sortIndicator = QStyleOptionHeader::SortDown;
        else
            option->sortIndicator = QStyleOptionHeader::SortUp;
    }

    if (d->isSectionCurrent(visual))
        option->state |= QStyle::State_HasFocus;
    else
        option->state &= ~QStyle::State_HasFocus;

    if (d->isSectionSelected(visual))
        option->state |= QStyle::State_Selected;
    else
        option->state &= ~QStyle::State_Selected;

    if (d->isSectionHighlighted(visual))
        option->state |= QStyle::State_On;
    else
        option->state &= ~QStyle::State_On;

    if (d->isSectionUnderMouse(visual))
        option->state |= QStyle::State_MouseOver;
    else
        option->state &= ~QStyle::State_MouseOver;

    if (d->isSectionPressed(visual)) {
        option->state |= QStyle::State_Sunken;
        //option->state &= ~QStyle::State_Raised;
    } else {
        //option->state &= ~QStyle::State_Sunken;
        option->state |= QStyle::State_Raised;
    }

    // section data

    option->icon = qvariant_cast<QIcon>(d->data(logicalIndex, "DecorationRole"));
    option->text = qvariant_cast<QString>(d->data(logicalIndex, "DisplayRole"));
}

/*!
 */
bool QtGraphicsHeader::event(QEvent *event)
{
    if (processEvent(event, transform())) {
        event->accept();
        return true;
    }
    return QGraphicsWidget::event(event);
}

/*!
  Paints the section indicated by the given \a logicalIndex,
  using the given \a painter and \a option.

  \sa paint
 */
void QtGraphicsHeader::paintSection(QPainter *painter, QStyleOptionHeader *option, int logicalIndex) const
{
    Q_UNUSED(logicalIndex);
    Q_ASSERT(option);
    style()->drawControl(QStyle::CE_Header, option, painter, /*option->widget*/0);
}

/*!
  Paints an empty section using the given \a painter and \a option.

  \sa paintSection
 */
void QtGraphicsHeader::paintEmptySection(QPainter *painter, QStyleOptionHeader *option) const
{
    Q_ASSERT(option);
    style()->drawControl(QStyle::CE_HeaderEmptyArea, option, painter, /*option->widget*/0);
}

/*!
  Paints the indicator that shows the moved section, indicated by \a logicalIndex,
  using the given \a painter and \a option.

  The default implementation calls paintSection.

  \sa paintSection
 */
void QtGraphicsHeader::paintDragIndicator(QPainter *painter, QStyleOptionHeader *option, int logicalIndex) const
{
    const qreal opacity = painter->opacity();
    painter->setOpacity(0.75); // ### property ?
    paintSection(painter, option, logicalIndex);
    painter->setOpacity(opacity);
}

/*!
 */
void QtGraphicsHeader::copyStyleOptionState(const QStyleOptionGraphicsItem *source, QStyleOptionHeader *dest)
{
    if (source && dest) {
        dest->state = source->state;
        dest->direction = source->direction;
        dest->rect = source->rect;
        dest->fontMetrics = source->fontMetrics;
        dest->palette = source->palette;
    }
}

/*!
  Copies the header section data structures from the given \a other
  header object.
 */
void QtGraphicsHeader::copySections(const QtGraphicsHeader &other)
{
    Q_D(QtGraphicsHeader);
    d->cachedDataIndex = -1;
    setSectionCount(other.sectionCount());
    d->sectionSizeSpans = other.d_ptr->sectionSizeSpans;
    d->sectionModeSpans = other.d_ptr->sectionModeSpans;
    d->sectionMovableSpans = other.d_ptr->sectionMovableSpans;
    d->sectionClickableSpans = other.d_ptr->sectionClickableSpans;
    d->sectionSelectedSpans = other.d_ptr->sectionSelectedSpans;
}

QT_END_NAMESPACE

#include "moc_qgraphicsheader.cpp"
