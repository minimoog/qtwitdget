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

#include "qlistselectionmanager.h"
#include "qlistselectionmanager_p.h"
#include "qlistmodelinterface.h"

#include <qdebug.h>

QT_BEGIN_NAMESPACE

// selection change data

QtListSelectionChangeData::QtListSelectionChangeData()
    : toggle(false)
{
}

QtListSelectionChangeData::~QtListSelectionChangeData()
{
}

void QtListSelectionChangeData::setRanges(int oldAnchor, int oldCurrent, int newAnchor, int newCurrent,
                                          const QBitArray &selections, QtListSelectionManager::SelectionMode mode)
{
    previousRange.first = qMin(oldAnchor, oldCurrent);
    previousRange.second = qMax(oldAnchor, oldCurrent);
    currentRange.first = qMin(newAnchor, newCurrent);
    currentRange.second = qMax(newAnchor, newCurrent);
    boundingRange.first = qMin(previousRange.first, currentRange.first);
    boundingRange.second = qMax(previousRange.second, currentRange.second);
    toggle = (mode == QtListSelectionManager::Toggle);
    previousSelections = selections;
    currentSelections = selections;
    indexes.clear();
}

void QtListSelectionChangeData::setIndexes(int from, int to, const QList<int> &changed)
{
    previousRange = QPair<int,int>(-1, -1);
    currentRange = QPair<int,int>(-1, -1);
    boundingRange = QPair<int,int>(from, to);
    toggle = false;
    previousSelections.clear();
    currentSelections.clear();
    indexes = changed;
}

void QtListSelectionChangeData::setSelections(const QBitArray &previous, const QBitArray &current)
{
    previousRange = QPair<int,int>(-1, -1);
    currentRange = QPair<int,int>(-1, -1);
    boundingRange = QPair<int,int>(0, qMin(previous.count(), current.count()));
    toggle = false;
    previousSelections = previous;
    currentSelections = current;
    previousSelections.resize(current.size());
    indexes.clear();
}

void QtListSelectionChangeData::clear()
{
    // used to avoid the array being detached
    previousRange = QPair<int,int>(-1, -1);
    currentRange = QPair<int,int>(-1, -1);
    boundingRange = QPair<int,int>(-1, -1);
    toggle = false;
    previousSelections.clear();
    currentSelections.clear();
    indexes.clear();
}

QList<int> QtListSelectionChangeData::changedIndexes() const
{
    if (!indexes.isEmpty() || currentSelections.isEmpty())
        return indexes; // already computed
    for (int i = boundingRange.first; i <= boundingRange.second; ++i) {
        // old selection value
        bool oldSelect = false;
        if (previousRange.first <= i && i <= previousRange.second)
            oldSelect = (toggle ? !previousSelections.testBit(i) : true);
        else if (0 <= i && i < previousSelections.count())
            oldSelect = previousSelections.testBit(i);
        // new selection value
        bool newSelect = false;
        if (currentRange.first <= i && i <= currentRange.second)
            newSelect = (toggle ? !currentSelections.testBit(i) : true);
        else if (0 <= i && i < currentSelections.count())
            newSelect = currentSelections.testBit(i);
        // compare old and new
        if (oldSelect != newSelect)
            indexes.append(i);
    }
    return indexes;
}

int QtListSelectionChangeData::from() const
{
    return boundingRange.first;
}

int QtListSelectionChangeData::to() const
{
    return boundingRange.second;
}

int QtListSelectionChangeData::count() const
{
    return boundingRange.second - boundingRange.first + 1;
}

// QtListSelectionChange

QtListSelectionChange::QtListSelectionChange()
{
    d = new QtListSelectionChangeData();
}

QtListSelectionChange::QtListSelectionChange(const QtListSelectionChange &other)
    : d(other.d)
{
}

QtListSelectionChange::~QtListSelectionChange()
{
}

QtListSelectionChange &QtListSelectionChange::operator=(const QtListSelectionChange &other)
{
    d = other.d;
    return *this;
}

int QtListSelectionChange::index() const
{
    return d->from();
}

int QtListSelectionChange::count() const
{
    return d->count();
}

QList<int> QtListSelectionChange::indexes() const
{
    return d->changedIndexes();
}

// selection manager

QtListSelectionManagerPrivate::QtListSelectionManagerPrivate()
    : q_ptr(0),
    model(0),
    current(-1),
    anchor(-1),
    active(false),
    mode(QtListSelectionManager::Select)
{
    qRegisterMetaType<QtListSelectionChange>();
}

QtListSelectionManagerPrivate::~QtListSelectionManagerPrivate()
{
}

void QtListSelectionManagerPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtListSelectionManagerPrivate::_q_itemsInserted(int index, int count)
{
    Q_Q(QtListSelectionManager);
    selections.resize(selections.count() + count);
    Q_ASSERT(model->count() == selections.count());
    // move selections
    QList<int> indexes;
    for (int i = selections.count() - 1; i >= index + count; --i) {
        if (selections[i] != selections[i - count]) // ### avoid lookup
            indexes.prepend(i);
        selections[i] = selections[i - count];
    }
    selections.fill(false, index, index + count);
    if (!indexes.isEmpty()) { // indexes are already computed
        change.d->setIndexes(index, selections.count() - 1, indexes);
        emit q->selectionsChanged(change);
        change.d->clear(); // avoid selections detach later
    }
    // move current
    const int previousCurrent = current;
    if (current >= index) {
        current += count;
        emit q->currentChanged(current, previousCurrent);
    }
    // move anchor
    const int previousAnchor = anchor;
    if (anchor >= index) {
        anchor += count;
        emit q->anchorChanged(anchor, previousAnchor);
    }
    // update the anchored selection
    if (active && (anchor != previousAnchor || current != previousCurrent)) {
        change.d->setRanges(previousAnchor, previousCurrent, anchor, current, selections, mode);
        emit q->selectionsChanged(change);
        change.d->clear(); // avoid detach later
    }
}

void QtListSelectionManagerPrivate::_q_itemsRemoved(int index, int count)
{
    Q_Q(QtListSelectionManager);
    const int end = selections.count() - count;
    Q_ASSERT(count <= selections.count()); // ### maybe make into a warning ?
    // move selections
    QList<int> indexes;
    for (int i = index; i < end; ++i) {
        if (selections[i] != selections[i + count]) // ### avoid lookup
            indexes.prepend(i);
        selections[i] = selections[i + count];
    }
    selections.truncate(end);
    Q_ASSERT(model->count() == selections.count());
    if (!indexes.isEmpty()) {  // indexes are already computed
        change.d->setIndexes(index, selections.count() - 1, indexes);
        emit q->selectionsChanged(change);
        change.d->clear(); // avoid selections detach later
    }
    const int last = model->count() ? model->count() - 1 : 0;
    // move current
    const int previousCurrent = current;
    if (current >= index)
        current -= count;
    current = qBound(0, current, last);
    if (previousCurrent != current)
        emit q->currentChanged(current, previousCurrent);

    // move anchor
    const int previousAnchor = anchor;
    if (anchor >= index)
        anchor -= count;
    anchor = qBound(0, anchor, last);
    if (previousAnchor != anchor)
        emit q->anchorChanged(anchor, previousAnchor);

    // update the anchored selection
    if (active && (anchor != previousAnchor || current != previousCurrent)) {
        change.d->setRanges(previousAnchor, previousCurrent, anchor, current, selections, mode);
        emit q->selectionsChanged(change);
        change.d->clear(); // avoid detach later
    }
}

void QtListSelectionManagerPrivate::_q_itemsMoved(int from, int to, int count)
{
    Q_Q(QtListSelectionManager);
    if (from == to || count == 0)
        return;
    QBitArray moving(count);
    // save the moved selections
    for (int i = 0; i < count; ++i) {
        moving[i] = selections[from + i];
    }
    QList<int> indexes;
    // move the existing selections
    if (from < to) {
        for (int j = from; j < to; ++j) {
            if (selections[j] != selections[j - count]) // ### avoid lookup
                indexes.append(j);
            selections[j] = selections[j + count];
        }
    } else { // from > to
        for (int j = from + count - 1; j > to; --j) {
            if (selections[j] != selections[j - count]) // ### avoid lookup
                indexes.prepend(j);
            selections[j] = selections[j - count];
         }
    }
    // insert the moved selections
    for (int k = 0; k < count; ++k) {
        if (selections[to + k] != moving[k]) // ### avoid lookup
            indexes.append(to + k);
        selections[to + k] = moving[k];
    }
    if (!indexes.isEmpty()) { // indexes are already computed
        change.d->setIndexes(qMin(from, to), qMax(from, to) + count - 1, indexes);
        emit q->selectionsChanged(change);
        change.d->clear(); // avoid selections detach later
    }
    // move the current index if needed
    const int previousCurrent = current;
    if (from < current && current < from + count) {
        current = to + (current - from);
        emit q->currentChanged(current, previousCurrent);
    }
    // move the anchor index if needed
    const int previousAnchor = anchor;
    if (from < anchor && anchor < from + count) {
        anchor = to + (anchor - from);
        emit q->anchorChanged(anchor, previousAnchor);
    }
    // update the anchored selection
    if (active && (anchor != previousAnchor || current != previousCurrent)) {
        change.d->setRanges(previousAnchor, previousCurrent, anchor, current, selections, mode);
        emit q->selectionsChanged(change);
        change.d->clear(); // avoid detach later
    }
}

void QtListSelectionManagerPrivate::_q_reset()
{
    const int count = model ? model->count() : 0;
    selections.resize(count);
    selections.fill(false);
    current = count ? qBound(0, current, count - 1) : -1;
    anchor = count ? qBound(0, anchor, count - 1) : -1;
}

/*!
  \class QtListSelectionManager
  \brief A list selection manager
*/

QtListSelectionManager::QtListSelectionManager(QObject *parent)
    : QObject(parent), d_ptr(new QtListSelectionManagerPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtListSelectionManager::QtListSelectionManager(QtListSelectionManagerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtListSelectionManager::~QtListSelectionManager()
{
    delete d_ptr;
}

/*!
 */
void QtListSelectionManager::setModel(QtListModelInterface *model)
{
    Q_D(QtListSelectionManager);
    if (d->model == model)
        return;
    if (d->model) {
        disconnect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        disconnect(d->model, SIGNAL(itemsInserted(int,int)), this, SLOT(_q_itemsInserted(int,int)));
        disconnect(d->model, SIGNAL(itemsRemoved(int,int)), this, SLOT(_q_itemsRemoved(int,int)));
        disconnect(d->model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(_q_itemsMoved(int,int,int)));
    }
    d->model = model;
    if (d->model) {
        connect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        connect(d->model, SIGNAL(itemsInserted(int,int)), this, SLOT(_q_itemsInserted(int,int)));
        connect(d->model, SIGNAL(itemsRemoved(int,int)), this, SLOT(_q_itemsRemoved(int,int)));
        connect(d->model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(_q_itemsMoved(int,int,int)));
    }
    d->_q_reset();
}

/*!
 */
QtListModelInterface *QtListSelectionManager::model() const
{
    Q_D(const QtListSelectionManager);
    return d->model;
}

/*!
  Sets the current item to be the given \a current.

  \sa currentItem()
*/
void QtListSelectionManager::setCurrentItem(int current)
{
    Q_D(QtListSelectionManager);
    if (!d->model || !d->model->count())
        return;
    current = qBound(0, current, d->model->count() - 1);
    if (current != d->current) {
        // update current
        int previous = d->current;
        d->current = current;
        emit currentChanged(current, previous);
        // update the anchored selection
        if (d->active) {
            d->change.d->setRanges(d->anchor, d->anchor, previous, current, d->selections, d->mode);
            emit selectionsChanged(d->change);
            d->change.d->clear(); // avoid detach later
        }
    }
}

/*!
  Returns the index of the current item. If the returned value is -1,
  the current is disabled.

  \sa setCurrentItem()
 */
int QtListSelectionManager::currentItem() const
{
    Q_D(const QtListSelectionManager);
    return d->current;
}

/*!
  Sets the anchor item to be the given \a anchor.

  \sa anchorItem()
*/
void QtListSelectionManager::setAnchorItem(int anchor)
{
    Q_D(QtListSelectionManager);
    if (!d->model || !d->model->count())
        return;
    anchor = qBound(0, anchor, d->model->count() - 1);
    if (anchor != d->anchor) {
        // update anchor
        int previous = d->anchor;
        d->anchor = anchor;
        emit anchorChanged(anchor, previous);
        // update the anchored selection
        if (d->active) {
            d->change.d->setRanges(previous, anchor, d->current, d->current, d->selections, d->mode);
            emit selectionsChanged(d->change);
            d->change.d->clear(); // avoid detach later
        }
    }
}

/*!
  Returns the index of the anchor item. If the returned value is -1,
  the anchor is disabled.

  \sa setAnchorItem()
 */
int QtListSelectionManager::anchorItem() const
{
    Q_D(const QtListSelectionManager);
    return d->anchor;
}

bool QtListSelectionManager::isSelected(int index) const
{
    Q_D(const QtListSelectionManager);
    if (d->active) {
        const int from = qMin(d->anchor, d->current);
        const int to = qMax(d->anchor, d->current);
        if (from <= index && index <= to)
            switch (d->mode) {
            case Select:
                return true;
            case Deselect:
                return false;
            case Toggle:
                return !d->selections.testBit(index);
            default:
                break;
            }
    }
    if (index >= 0 && index < d->selections.count())
        return d->selections.testBit(index);
    return false;
}

/*!
  Sets the selected state of the items in the range indicated by the given
  \a index and \a count to be either true or false, depending on the given
  \a mode.

  \sa isSelected()
 */
void QtListSelectionManager::setSelected(int index, int count, SelectionMode mode)
{
    Q_D(QtListSelectionManager);
    if (index < 0) {
        // cut off the part before the zero index.
        count += index;
        index = 0;
    }
    if (count <= 0)
        return;
    if (index + count > d->selections.count()) { // ### FIXME
        qWarning() << "QtListSelectionManager::setSelected selecting non-existing items";
        d->selections.resize(index + count);
    }
    const int from = index;
    count = qMin(count, d->selections.count()); // this helps the usecase where count passed is INT_MAX
    const int to = qMin(index + count, d->selections.count());
    // set selection
    const QBitArray old = d->selections;
    switch (mode) {
    case Select:
        d->selections.fill(true, from, to);
        break;
    case Deselect:
        d->selections.fill(false, from, to);
        break;
    case Toggle:
        for (int i = from; i <= to; ++i)
            d->selections.toggleBit(i);
        break;
    }
    d->change.d->setSelections(old, d->selections);
    emit selectionsChanged(d->change);
    d->change.d->clear(); // avoid selections detach later
}

/*!
 */
QList<int> QtListSelectionManager::selectedItems() const
{
    Q_D(const QtListSelectionManager);
    QList<int> items;
    for (int i = 0; i < d->selections.count(); ++i)
        if (isSelected(i))
            items.append(i);
    return items;
}

/*!
 */
void QtListSelectionManager::clearSelections()
{
    Q_D(QtListSelectionManager);
    if (d->active) {
        d->active = false;
        d->mode = Select;
    }
    // make the change find the difference between old selection and no selection
    const QBitArray old = d->selections;
    d->selections.fill(false);
    d->change.d->setSelections(old, d->selections);
    emit selectionsChanged(d->change);
    d->change.d->clear();
}

/*!
 */
bool QtListSelectionManager::isAnchoredSelectionActive() const
{
    Q_D(const QtListSelectionManager);
    return d->active;
}

/*!
 */
void QtListSelectionManager::setAnchoredSelectionActive(bool active)
{
    Q_D(QtListSelectionManager);
    d->active = active;
    if (d->active) {
        d->change.d->setRanges(d->anchor, d->anchor, d->current, d->current, d->selections, d->mode);
        emit selectionsChanged(d->change);
        d->change.d->clear(); // avoid detach later
    }
}

/*!
 */
QtListSelectionManager::SelectionMode QtListSelectionManager::anchoredSelectionMode() const
{
    Q_D(const QtListSelectionManager);
    return d->mode;
}

/*!
 */
void QtListSelectionManager::setAnchoredSelectionMode(QtListSelectionManager::SelectionMode mode)
{
    Q_D(QtListSelectionManager);
    if (mode != d->mode) {
        d->mode = mode;
        if (d->active) {
            d->change.d->setRanges(d->anchor, d->anchor, d->current, d->current, d->selections, d->mode);
            emit selectionsChanged(d->change);
            d->change.d->clear(); // avoid detach later
        }
    }
}

/*!
 */
void QtListSelectionManager::beginAnchoredSelection(int anchor, SelectionMode mode)
{
    Q_D(QtListSelectionManager);
    if (d->active)
        return;
    d->active = true;
    d->mode = mode;
    setAnchorItem(anchor);
}

/*!
 */
void QtListSelectionManager::endAnchoredSelection()
{
    Q_D(QtListSelectionManager);
    if (d->active) {
        commitAnchoredSelection();
        d->active = false;
    }
}

/*!
 */
void QtListSelectionManager::commitAnchoredSelection()
{
    Q_D(QtListSelectionManager);
    // commit anchored selection range
    if (d->current >= 0 && d->anchor >= 0) {
        const int from = qMin(d->anchor, d->current);
        const int to = qMax(d->anchor, d->current) + 1;
        switch (d->mode) {
        case Select:
            d->selections.fill(true, from, to);
            break;
        case Deselect:
            d->selections.fill(false, from, to);
            break;
        case Toggle:
            for (int i = from; i <= to; ++i)
                d->selections.toggleBit(i);
            break;
        }
    }
}

QT_END_NAMESPACE

#include "moc_qlistselectionmanager.cpp"
