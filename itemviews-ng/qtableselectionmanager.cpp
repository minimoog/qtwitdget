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

#include "qtableselectionmanager.h"
#include "qtableselectionmanager_p.h"
#include <qdebug.h>
#include <limits.h>

QT_BEGIN_NAMESPACE

class QtTableSelectionRangePrivate
{
public:
    // 0. 'a' is contained by 'b'
    static inline bool containedBy(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.topRow() >= b.topRow() && a.leftColumn() >= b.leftColumn()
                && a.rightColumn() <= b.rightColumn() && a.bottomRow() <= b.bottomRow());
    }
    // 1. 'a' contains 'b'
    static inline bool contains(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.topRow() < b.topRow() && a.leftColumn() < b.leftColumn()
                && a.rightColumn() > b.rightColumn() && a.bottomRow() > b.bottomRow());
    }
    // 2. 'b' clips the entire top side of 'a'
    static inline bool clipsEntireTopSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.topRow(), a.leftColumn()) && b.containsCell(a.topRow(), a.rightColumn())
                && !b.containsCell(a.bottomRow(), a.leftColumn()) && !b.containsCell(a.bottomRow(), a.rightColumn()));
    }
    // 3. 'b' clips the entire left side of 'a'
    static inline bool clipsEntireLeftSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.topRow(), a.leftColumn()) && b.containsCell(a.bottomRow(), a.leftColumn())
                && !b.containsCell(a.topRow(), a.rightColumn()) && !b.containsCell(a.bottomRow(), a.rightColumn()));
    }
    // 4. 'b' clips the entire bottom side of 'a'
    static inline bool clipsEntireBottomSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.bottomRow(), a.leftColumn()) && b.containsCell(a.bottomRow(), a.rightColumn())
                && !b.containsCell(a.topRow(), a.leftColumn()) && !b.containsCell(a.topRow(), a.rightColumn()));
    }
    // 5. 'b' clips the entire right side of 'a'
    static inline bool clipsEntireRightSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.topRow(), a.rightColumn()) && b.containsCell(a.bottomRow(), a.rightColumn())
                && !b.containsCell(a.topRow(), a.leftColumn()) && !b.containsCell(a.bottomRow(), a.leftColumn()));
    }
    // 6. 'b' clips top left corner of 'a'
    static inline bool clipsTopLeftCorner(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.topRow(), a.leftColumn()) && !b.containsCell(a.topRow(), a.rightColumn())
                && !b.containsCell(a.bottomRow(), a.leftColumn()));
    }
    // 7. 'b' clips bottom left corner of 'a'
    static inline bool clipsBottomLeftCorner(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.bottomRow(), a.leftColumn()) && !b.containsCell(a.topRow(), a.leftColumn())
                && !b.containsCell(a.bottomRow(), a.rightColumn()));
    }
    // 8. 'b' clips top right corner of 'a'
    static inline bool clipsTopRightCorner(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.topRow(), a.rightColumn()) && !b.containsCell(a.topRow(), a.leftColumn())
                && !b.containsCell(a.bottomRow(), a.rightColumn()));
    }
    // 9. 'b' clips bottom right corner of 'a'
    static inline bool clipsBottomRightCorner(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (b.containsCell(a.bottomRow(), a.rightColumn()) && !b.containsCell(a.bottomRow(), a.leftColumn())
                && !b.containsCell(a.topRow(), a.rightColumn()));
    }
    // 10. 'b' clips into the top side of 'a'
    static inline bool clipsIntoTopSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.containsCell(b.bottomRow(), b.leftColumn()) && a.containsCell(b.bottomRow(), b.rightColumn())
                && (a.topRow() >= b.topRow() && a.leftColumn() < b.leftColumn()
                    && a.bottomRow() > b.bottomRow() && a.rightColumn() > b.rightColumn()));
    }
    // 11. 'b' clips into the left side of 'a'
    static inline bool clipsIntoLeftSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.containsCell(b.topRow(), b.rightColumn()) && a.containsCell(b.bottomRow(), b.rightColumn())
                && (a.topRow() < b.topRow() && a.leftColumn() >= b.leftColumn()
                    && a.bottomRow() > b.bottomRow() && a.rightColumn() > b.rightColumn()));
    }
    // 12. 'b' clips into the bottom side of 'a'
    static inline bool clipsIntoBottomSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.containsCell(b.topRow(), b.leftColumn()) && a.containsCell(b.topRow(), b.rightColumn())
                && (a.topRow() < b.topRow() && a.leftColumn() < b.leftColumn()
                    && a.bottomRow() <= b.bottomRow() && a.rightColumn() > b.rightColumn()));
    }
    // 13. 'b' clips into the right side of 'a'
    static inline bool clipsIntoRightSide(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.containsCell(b.topRow(), b.leftColumn()) && a.containsCell(b.bottomRow(), b.leftColumn())
                && (a.topRow() < b.topRow() && a.leftColumn() < b.leftColumn()
                    && a.bottomRow() > b.bottomRow() && a.rightColumn() <= b.rightColumn()));
    }
    // 14. 'b' intersects 'a' horizontally
    static inline bool intersectsHorizontally(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.topRow() < b.topRow() && a.bottomRow() > b.bottomRow()
                && a.leftColumn() >= b.leftColumn() && a.rightColumn() <= b.rightColumn());
    }
    // 15. 'b' intersects 'a' vertically
    static inline bool intersectsVertically(const QtTableSelectionRange &a, const QtTableSelectionRange &b)
    {
        return (a.leftColumn() < b.leftColumn() && a.rightColumn() > b.rightColumn()
                && a.topRow() >= b.topRow() && a.bottomRow() <= b.bottomRow());
    }
};

/*!
    \class QtTableSelectionRange
    \brief api docs go here.
*/

// QtTableSelectionRange
/*!
 */
QtTableSelectionRange::QtTableSelectionRange()
    : top(-1), left(-1), bottom(-1), right(-1)
{
}

/*!
 */
QtTableSelectionRange::QtTableSelectionRange(int top, int left, int bottom, int right)
    : top(top), left(left), bottom(bottom), right(right)
{
}

/*!
 */
QtTableSelectionRange::QtTableSelectionRange(const QtTableSelectionRange &other)
    : top(other.top), left(other.left), bottom(other.bottom), right(other.right)
{
}

/*!
 */
QtTableSelectionRange::~QtTableSelectionRange()
{
}

/*!
 */
bool QtTableSelectionRange::intersects(const QtTableSelectionRange &other) const
{
    // ### may not be the most efficient intersection test
    if (QtTableSelectionRangePrivate::containedBy(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::contains(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsEntireTopSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsEntireLeftSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsEntireBottomSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsEntireRightSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsTopLeftCorner(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsBottomLeftCorner(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsTopRightCorner(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsBottomRightCorner(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsIntoTopSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsIntoLeftSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsIntoBottomSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::clipsIntoRightSide(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::intersectsHorizontally(*this, other))
        return true;
    if (QtTableSelectionRangePrivate::intersectsVertically(*this, other))
        return true;
    return false;
}

/*!
 */
QList<QtTableSelectionRange> QtTableSelectionRange::subtracted(const QtTableSelectionRange &other) const
{
    if (QtTableSelectionRangePrivate::containedBy(*this, other))
        return QList<QtTableSelectionRange>(); // nothing left
    if (QtTableSelectionRangePrivate::contains(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.top - 1, right)); // top
        ranges.append(QtTableSelectionRange(other.top, left, other.bottom, other.right - 1)); // left
        ranges.append(QtTableSelectionRange(other.bottom + 1, left, bottom, right)); // bottom
        ranges.append(QtTableSelectionRange(other.top, other.right + 1, other.bottom, right)); // right
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsEntireTopSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(other.bottom + 1, left, bottom, right)); // bottom
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsEntireLeftSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, other.right + 1, bottom, right)); // right
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsEntireBottomSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.top - 1, right)); // top
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsEntireRightSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, bottom, other.left - 1)); // left
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsTopLeftCorner(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, other.right + 1, other.bottom, right)); // top right
        ranges.append(QtTableSelectionRange(other.bottom + 1, left, bottom, right)); // bottom
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsBottomLeftCorner(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.top - 1, right)); // top
        ranges.append(QtTableSelectionRange(other.top, other.right + 1, bottom, right)); // bottom right
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsTopRightCorner(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.bottom, other.left - 1)); // top left
        ranges.append(QtTableSelectionRange(other.bottom + 1, left, bottom, right)); // bottom
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsBottomRightCorner(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.top - 1, right)); // top
        ranges.append(QtTableSelectionRange(other.top, left, bottom, other.left - 1)); // bottom left
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsIntoTopSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.bottom, other.left - 1)); // top left
        ranges.append(QtTableSelectionRange(top, other.right + 1, other.bottom, right)); // top right
        ranges.append(QtTableSelectionRange(other.bottom + 1, left, bottom, right)); // bottom
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsIntoLeftSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.top - 1, other.right)); // top left
        ranges.append(QtTableSelectionRange(other.bottom + 1, left, bottom, other.right)); // bottom left
        ranges.append(QtTableSelectionRange(top, other.right + 1, bottom, right)); // right
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsIntoBottomSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.top - 1, right)); // top
        ranges.append(QtTableSelectionRange(other.top, left, bottom, other.left - 1)); // bottom left
        ranges.append(QtTableSelectionRange(other.top, other.right + 1, bottom, right)); // bottom right
        return ranges;
    }
    if (QtTableSelectionRangePrivate::clipsIntoRightSide(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, bottom, other.left - 1)); // left
        ranges.append(QtTableSelectionRange(top, other.left, other.top - 1, right)); // top right
        ranges.append(QtTableSelectionRange(other.bottom + 1, other.left, bottom, right)); // bottom right
        return ranges;
    }
    if (QtTableSelectionRangePrivate::intersectsHorizontally(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, other.top - 1, right)); // top
        ranges.append(QtTableSelectionRange(other.bottom + 1, left, bottom, right)); // bottom
        return ranges;
    }
    if (QtTableSelectionRangePrivate::intersectsVertically(*this, other)) {
        QList<QtTableSelectionRange> ranges;
        ranges.append(QtTableSelectionRange(top, left, bottom, other.left - 1)); // left
        ranges.append(QtTableSelectionRange(top, other.right + 1, bottom, right)); // right
        return ranges;
    }
    return QList<QtTableSelectionRange>();
}

/*!
 */
bool QtTableSelectionRange::operator==(const QtTableSelectionRange &other) const
{
    return other.top == top && other.left == left && other.bottom == bottom && other.right == right;
}

/*!
 */
bool QtTableSelectionRange::operator!=(const QtTableSelectionRange &other) const
{
    return !operator==(other);
}

/*!
 */
void QtTableSelectionRange::setRange(int topRow, int leftColumn, int bottomRow, int rightColumn)
{
    top = topRow;
    left = leftColumn;
    bottom = bottomRow;
    right = rightColumn;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QtTableSelectionRange &range)
{
#ifndef Q_BROKEN_DEBUG_STREAM
    dbg.nospace() << "QtTableSelectionRange("
                  << range.topRow() << ","
                  << range.leftColumn() << ","
                  << range.bottomRow() << ","
                  << range.rightColumn() << ")";
    return dbg.space();
#else
    qWarning("This compiler doesn't support streaming QtTableSelectionRange to QDebug");
    return dbg;
    Q_UNUSED(range);
#endif
}
#endif

// QtTableSelectionManagerPrivate

QtTableSelectionManagerPrivate::QtTableSelectionManagerPrivate()
    : q_ptr(0), model(0),
      currentRow(-1), currentColumn(-1),
      anchorRow(-1), anchorColumn(-1),
      active(false),
      mode(QtTableSelectionManager::Select)
{
}

QtTableSelectionManagerPrivate::~QtTableSelectionManagerPrivate()
{
}

void QtTableSelectionManagerPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtTableSelectionManagerPrivate::_q_rowsInserted(int row, int count)
{
    Q_Q(QtTableSelectionManager);
    // update selections
    QList<QtTableSelectionRange> changed;
    QList<QtTableSelectionRange>::iterator it = selections.begin();
    for (;it != selections.end(); ++it) {
        if (row < (*it).topRow()) { // move selection range
            (*it).setRange((*it).topRow() + count, (*it).leftColumn(), (*it).bottomRow() + count, (*it).rightColumn());
            changed.append(*it);
        } else if (row <= (*it).bottomRow()) { // increase selection range
            (*it).setRange((*it).topRow(), (*it).leftColumn(), (*it).bottomRow() + count, (*it).rightColumn());
            changed.append(*it);
        }
    }
    // update current
    const int previousCurrentRow = currentRow;
    if (row <= currentRow) {
        currentRow += count;
        emit q->currentChanged(currentRow, currentColumn, previousCurrentRow, currentColumn);
    }
    // update anchor
    const int previousAnchorRow = anchorRow;
    if (row <= anchorRow) {
        anchorRow += count;
        emit q->anchorChanged(anchorRow, anchorColumn, previousAnchorRow, anchorColumn);
    }
    // update selections
    if (active && (anchorRow != previousAnchorRow || currentRow != previousCurrentRow)) {
        currentAnchorRange.setRange(qMin(anchorRow, currentRow), qMin(anchorColumn, currentColumn),
                                    qMax(anchorRow, currentRow), qMax(anchorColumn, currentColumn));
        changed.append(currentAnchorRange);
    }
    if (!changed.isEmpty())
        emit q->selectionsChanged(changed);
}

void QtTableSelectionManagerPrivate::_q_rowsRemoved(int row, int count)
{
    Q_Q(QtTableSelectionManager);
    // update selections
    QList<QtTableSelectionRange> changed;
    QList<QtTableSelectionRange>::iterator it = selections.begin();
    while (it != selections.end()) {
        if (row < (*it).topRow()) { // move selection range
            changed.append(*it);
            (*it).setRange((*it).topRow() - count, (*it).leftColumn(), (*it).bottomRow() - count, (*it).rightColumn());
        } else if (row <= (*it).bottomRow()) {
            changed.append(*it);
            if (count < ((*it).bottomRow() - (*it).topRow() + 1)) { // reduce the selection range
                 (*it).setRange((*it).topRow(), (*it).leftColumn(), (*it).bottomRow() - count, (*it).rightColumn());
            } else { // remove the selection range
                 it = selections.erase(it);
                 continue;
            }
        }
        ++it;
    }
    // update current
    const int previousCurrentRow = currentRow;
    if (row < currentRow) {
        currentRow -= count;
        emit q->currentChanged(currentRow, currentColumn, previousCurrentRow, currentColumn);
    }
    // update anchor
    const int previousAnchorRow = anchorRow;
    if (row < anchorRow) {
        anchorRow -= count;
        emit q->anchorChanged(anchorRow, anchorColumn, previousAnchorRow, anchorColumn);
    }
    // update selections
    if (active && (anchorRow != previousAnchorRow || currentRow != previousCurrentRow)) {
        currentAnchorRange.setRange(qMin(anchorRow, currentRow), qMin(anchorColumn, currentColumn),
                                    qMax(anchorRow, currentRow), qMax(anchorColumn, currentColumn));
        changed.append(currentAnchorRange);
    }
    if (!changed.isEmpty())
        emit q->selectionsChanged(changed);
}

void QtTableSelectionManagerPrivate::_q_columnsInserted(int column, int count)
{
    Q_Q(QtTableSelectionManager);
    // update selections
    QList<QtTableSelectionRange> changed;
    QList<QtTableSelectionRange>::iterator it = selections.begin();
    for (;it != selections.end(); ++it) { // move selection range
        if (column < (*it).leftColumn()) {
            (*it).setRange((*it).topRow(), (*it).leftColumn() + count, (*it).bottomRow(), (*it).rightColumn() + count);
            changed.append(*it);
        } else if (column <= (*it).rightColumn()) { // increase selection range
            (*it).setRange((*it).topRow(), (*it).leftColumn(), (*it).bottomRow(), (*it).rightColumn() + count);
            changed.append(*it);
        }
    }
    // update current
    const int previousCurrentColumn = currentColumn;
    if (column <= currentColumn) {
        currentColumn += count;
        emit q->currentChanged(currentRow, currentColumn, currentRow, previousCurrentColumn);
    }
    // update anchor
    const int previousAnchorColumn = anchorColumn;
    if (column <= anchorColumn) {
        anchorColumn += count;
        emit q->anchorChanged(anchorRow, anchorColumn, anchorRow, previousAnchorColumn);
    }
    // update selections
    if (active && (anchorColumn != previousAnchorColumn || currentColumn != previousCurrentColumn)) {
        currentAnchorRange.setRange(qMin(anchorRow, currentRow), qMin(anchorColumn, currentColumn),
                                    qMax(anchorRow, currentRow), qMax(anchorColumn, currentColumn));
        changed.append(currentAnchorRange);
    }
    if (!changed.isEmpty())
        emit q->selectionsChanged(changed);
}

void QtTableSelectionManagerPrivate::_q_columnsRemoved(int column, int count)
{
    Q_Q(QtTableSelectionManager);
    // update selections
    QList<QtTableSelectionRange> changed;
    QList<QtTableSelectionRange>::iterator it = selections.begin();
    while (it != selections.end()) {
        if (column < (*it).leftColumn()) { // move selection range
            changed.append(*it);
            (*it).setRange((*it).topRow(), (*it).leftColumn() - count, (*it).bottomRow(), (*it).rightColumn() - count);
        } else if (column <= (*it).rightColumn()) {
            changed.append(*it);
            if (count < ((*it).rightColumn() - (*it).leftColumn() + 1)) { // reduce the selection range
                 (*it).setRange((*it).topRow(), (*it).leftColumn(), (*it).bottomRow(), (*it).rightColumn() - count);
            } else { // remove the selection range
                 it = selections.erase(it);
                 continue;
            }
        }
        ++it;
    }
    // update current
    const int previousCurrentColumn = currentColumn;
    if (column < currentColumn) {
        currentColumn -= count;
        emit q->currentChanged(currentRow, currentColumn, currentRow, previousCurrentColumn);
    }
    // update anchor
    const int previousAnchorColumn = anchorColumn;
    if (column < anchorColumn) {
        anchorColumn -= count;
        emit q->anchorChanged(anchorRow, anchorColumn, anchorRow, previousAnchorColumn);
    }
    // update selections
    if (active && (anchorColumn != previousAnchorColumn || currentColumn != previousCurrentColumn)) {
        currentAnchorRange.setRange(qMin(anchorRow, currentRow), qMin(anchorColumn, currentColumn),
                                    qMax(anchorRow, currentRow), qMax(anchorColumn, currentColumn));
        changed.append(currentAnchorRange);
    }
    if (!changed.isEmpty())
        emit q->selectionsChanged(changed);
}

void QtTableSelectionManagerPrivate::_q_reset()
{
    const int rowCount = model ? model->rowCount() : 0;
    const int columnCount = model ? model->columnCount() : 0;
    currentRow = rowCount ? qBound(0, currentRow, rowCount - 1) : -1;
    currentColumn = columnCount ? qBound(0, currentColumn, columnCount - 1) : -1;
    anchorRow = rowCount ? qBound(0, anchorRow, rowCount - 1) : -1;
    anchorColumn = columnCount ? qBound(0, anchorColumn, columnCount - 1) : -1;
}

bool QtTableSelectionManagerPrivate::isRowIntersected(int row) const
{
    bool toggle = false;
    if (active && currentAnchorRange.intersectsRow(row)) {
        switch (mode) {
        case QtTableSelectionManager::Select:
            return true;
        case QtTableSelectionManager::Deselect:
            return false;
        case QtTableSelectionManager::Toggle:
            toggle = true;
        default:
            break;
        }
    }
    for (int i = 0; i < selections.count(); ++i)
        if (selections.at(i).intersectsRow(row))
            return !toggle;
    return toggle;
}

bool QtTableSelectionManagerPrivate::isColumnIntersected(int column) const
{
    bool toggle = false;
    if (active && currentAnchorRange.intersectsColumn(column)) {
        switch (mode) {
        case QtTableSelectionManager::Select:
            return true;
        case QtTableSelectionManager::Deselect:
            return false;
        case QtTableSelectionManager::Toggle:
            toggle = true;
        default:
            break;
        }
    }
    for (int i = 0; i < selections.count(); ++i)
        if (selections.at(i).intersectsColumn(column))
            return !toggle;
    return toggle;
}

bool QtTableSelectionManagerPrivate::isSelected(int row, int column) const
{
    bool toggle = false;
    if (active && currentAnchorRange.containsCell(row, column)) {
        switch (mode) {
        case QtTableSelectionManager::Select:
            return true;
        case QtTableSelectionManager::Deselect:
            return false;
        case QtTableSelectionManager::Toggle:
            toggle = true;
        default:
            break;
        }
    }
    for (int i = 0; i < selections.count(); ++i)
        if (selections.at(i).containsCell(row, column))
            return !toggle;
    return toggle;
}

void QtTableSelectionManagerPrivate::selectRange(const QtTableSelectionRange &range)
{
    //changed.append(range);
    selections.append(range);
}

void QtTableSelectionManagerPrivate::deselectRange(const QtTableSelectionRange &range)
{
    //QList<QtTableSelectionRange> changedRanges;
    QList<QtTableSelectionRange> subtractedRanges;
    QList<QtTableSelectionRange>::iterator it = selections.begin();
    while (it != selections.end()) {
        if (range.intersects(*it)) {
            //changedRanges.append(*it);
            subtractedRanges += (*it).subtracted(range);
            it = selections.erase(it);
            continue;
        }
        ++it;
    }
    selections += subtractedRanges;
    //return changedRanges;
}

void QtTableSelectionManagerPrivate::toggleRange(const QtTableSelectionRange &range)
{
    //QList<QtTableSelectionRange> changedRanges;
    QList<QtTableSelectionRange> toggleRanges;
    QList<QtTableSelectionRange> selectRanges;
    toggleRanges.append(range);
    for (int i = 0; i < selections.count(); ++i) {
        const QtTableSelectionRange &selection = selections.at(i);
        bool intersected = false;
        // check for intersections with the toggle ranges
        for (int j = 0; j < toggleRanges.count(); ++j) {
            const QtTableSelectionRange &toggle = toggleRanges.at(j);
            // if we have an intersecting range, remove the intersecting area
            // then go on to check the remaining areas for intersections
            if (selection.intersects(toggle)) {
                selectRanges += selection.subtracted(toggle);
                toggleRanges += toggle.subtracted(selection);
                toggleRanges.removeAt(j--);
                intersected = true;
            }
        }
        // if the range was intersected, the remaining areas will
        // be added to the selections; the old range is removed
        if (intersected)
            selections.removeAt(i--);
    }
    // do the actual selection
    selections += toggleRanges;
    selections += selectRanges;
}

// QtTableSelectionManager

/*!
  \class QtTableSelectionManager
  \brief A table selection manager
*/

/*!
 */
QtTableSelectionManager::QtTableSelectionManager(QObject *parent)
    : QObject(parent), d_ptr(new QtTableSelectionManagerPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTableSelectionManager::QtTableSelectionManager(QtTableSelectionManagerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTableSelectionManager::~QtTableSelectionManager()
{
    delete d_ptr;
}

/*!
    Sets the model to be the given \a model.
 */
void QtTableSelectionManager::setModel(QtTableModelInterface *model)
{
    Q_D(QtTableSelectionManager);
    if (d->model == model)
        return;
    if (d->model) {
        disconnect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        disconnect(d->model, SIGNAL(rowsInserted(int,int)), this, SLOT(_q_rowsInserted(int,int)));
        disconnect(d->model, SIGNAL(rowsRemoved(int,int)), this, SLOT(_q_rowsRemoved(int,int)));
        disconnect(d->model, SIGNAL(columnsInserted(int,int)), this, SLOT(_q_columnsInserted(int,int)));
        disconnect(d->model, SIGNAL(columnsRemoved(int,int)), this, SLOT(_q_columnsRemoved(int,int)));
    }
    d->model = model;
    if (d->model) {
        connect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        connect(d->model, SIGNAL(rowsInserted(int,int)), this, SLOT(_q_rowsInserted(int,int)));
        connect(d->model, SIGNAL(rowsRemoved(int,int)), this, SLOT(_q_rowsRemoved(int,int)));
        connect(d->model, SIGNAL(columnsInserted(int,int)), this, SLOT(_q_columnsInserted(int,int)));
        connect(d->model, SIGNAL(columnsRemoved(int,int)), this, SLOT(_q_columnsRemoved(int,int)));
    }
    d->_q_reset();
    clearSelections();
}

/*!
    Returns a pointer to the model or null if no model is set.
 */
QtTableModelInterface *QtTableSelectionManager::model() const
{
    Q_D(const QtTableSelectionManager);
    return d->model;
}

/*!
    Sets the current cell to the given \a row and \a column.

    \sa currentCell(), anchorCell()
 */
void QtTableSelectionManager::setCurrentCell(int row, int column)
{
    Q_D(QtTableSelectionManager);
    if (!d->model || !d->model->rowCount() || !d->model->columnCount())
        return;
    row = qBound(0, row, d->model->rowCount() - 1);
    column = qBound(0, column, d->model->columnCount() - 1);
    if (d->currentRow != row || d->currentColumn != column) {
        const int previousRow = d->currentRow;
        const int previousColumn = d->currentColumn;
        d->currentRow = row;
        d->currentColumn = column;
        emit currentChanged(row, column, previousRow, previousColumn);
        d->currentAnchorRange.setRange(qMin(d->currentRow, d->anchorRow),
                                        qMin(d->currentColumn, d->anchorColumn),
                                        qMax(d->currentRow, d->anchorRow),
                                        qMax(d->currentColumn, d->anchorColumn));
        // ### FIXME: check the mode
        QList<QtTableSelectionRange> changed;
        changed.append(d->currentAnchorRange);
        emit selectionsChanged(changed);
    }
}

/*!
 */
int QtTableSelectionManager::currentRow() const
{
    Q_D(const QtTableSelectionManager);
    return d->currentRow;
}

/*!
 */
int QtTableSelectionManager::currentColumn() const
{
    Q_D(const QtTableSelectionManager);
    return d->currentColumn;
}

/*!
  Sets the selection anchor to the given \a row and \a column.
 */
void QtTableSelectionManager::setAnchorCell(int row, int column)
{
    Q_D(QtTableSelectionManager);
    if (!d->model || !d->model->rowCount() || !d->model->columnCount())
        return;
    row = qBound(0, row, d->model->rowCount() - 1);
    column = qBound(0, column, d->model->columnCount() - 1);
    if (d->anchorRow != row || d->anchorColumn != column) {
        const int previousRow = d->anchorRow;
        const int previousColumn = d->anchorColumn;
        d->anchorRow = row;
        d->anchorColumn = column;
        emit anchorChanged(row, column, previousRow, previousColumn);
        d->currentAnchorRange.setRange(qMin(d->currentRow, d->anchorRow),
                                        qMin(d->currentColumn, d->anchorColumn),
                                        qMax(d->currentRow, d->anchorRow),
                                        qMin(d->currentColumn, d->anchorColumn));
        // ### FIXME: check the mode
        QList<QtTableSelectionRange> changed;
        changed.append(d->currentAnchorRange);
        emit selectionsChanged(changed);
    }
}

/*!
 */
int QtTableSelectionManager::anchorRow() const
{
    Q_D(const QtTableSelectionManager);
    return d->anchorRow;
}

/*!
 */
int QtTableSelectionManager::anchorColumn() const
{
    Q_D(const QtTableSelectionManager);
    return d->anchorColumn;
}

/*!
 */
bool QtTableSelectionManager::isSelected(int row, int column) const
{
    Q_D(const QtTableSelectionManager);
    return d->isSelected(row, column);
}

/*!
 */
bool QtTableSelectionManager::isAnchorSelectionActive() const
{
    Q_D(const QtTableSelectionManager);
    return d->active;
}

/*!
 */
QtTableSelectionManager::SelectionMode QtTableSelectionManager::selectionMode() const
{
    Q_D(const QtTableSelectionManager);
    return d->mode;
}

/*!
 */
void QtTableSelectionManager::beginAnchoredSelection(int anchorRow, int anchorColumn, SelectionMode mode)
{
    Q_D(QtTableSelectionManager);
     if (d->active)
        return;
    d->active = true;
    d->mode = mode;
    setAnchorCell(anchorRow, anchorColumn);
}

/*!
 */
void QtTableSelectionManager::endAnchoredSelection()
{
    Q_D(QtTableSelectionManager);
    if (d->active) {
        // commit anchored selection range
        switch (d->mode) {
        case Select:
            d->selectRange(d->currentAnchorRange);
            break;
        case Deselect:
            d->deselectRange(d->currentAnchorRange);
            break;
        case Toggle:
            d->toggleRange(d->currentAnchorRange);
            break;
        default:
            break;
        }
        d->active = false;
        d->mode = Select;
    }
}

/*!
 */
QList<QtTableSelectionRange> QtTableSelectionManager::selectionRanges() const
{
    Q_D(const QtTableSelectionManager);
    QList<QtTableSelectionRange> ranges = d->selections;
    if (d->active)
        ranges += d->currentAnchorRange;
    return ranges;
}

/*!
 */
void QtTableSelectionManager::setSelected(const QtTableSelectionRange &range, SelectionMode mode)
{
    Q_D(QtTableSelectionManager);
    if (!d->model || !d->model->rowCount() || !d->model->columnCount() || !range.isValid())
        return;
    QList<QtTableSelectionRange> changed;
    switch (mode) {
    case Select:
        d->selectRange(range);
        break;
    case Deselect:
        d->deselectRange(range);
        break;
    case Toggle:
        d->toggleRange(range);
        break;
    default:
        break;
    }
    changed.append(range); // ### FIXME
    if (!changed.isEmpty())
        emit selectionsChanged(changed);
}

/*!
 */
void QtTableSelectionManager::clearSelections()
{
    Q_D(QtTableSelectionManager);
    if (!d->selections.isEmpty()) {
        QList<QtTableSelectionRange> changes = d->selections;
        d->selections.clear();
        emit selectionsChanged(changes);
    }
}

QT_END_NAMESPACE

#include "moc_qtableselectionmanager.cpp"
