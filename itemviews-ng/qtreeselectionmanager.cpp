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

#include "qtreeselectionmanager.h"
#include "qtreeselectionmanager_p.h"

QT_BEGIN_NAMESPACE

QtTreeSelectionManagerPrivate::QtTreeSelectionManagerPrivate()
    : q_ptr(0), model(0), mode(QtTreeSelectionManager::Select)
{
}

QtTreeSelectionManagerPrivate::~QtTreeSelectionManagerPrivate()
{
}

void QtTreeSelectionManagerPrivate::_q_modelDestroyed()
{
}

void QtTreeSelectionManagerPrivate::_q_itemsRemoved(const QtTreeModelBase::iterator_base &it, int count)
{
    QtTreeModelBase::iterator_base it2 = it;
    for (int i = 0; i < count && it.isValid(); ++i) {
        selections.remove(it2);
        it2.nextSibling();
    }
}

void QtTreeSelectionManagerPrivate::_q_reset()
{
}

/*!
  \class QtTreeSelectionManager
  \brief A tree selection manager
*/

QtTreeSelectionManager::QtTreeSelectionManager(QObject *parent)
    : QObject(parent), d_ptr(new QtTreeSelectionManagerPrivate)
{
    d_ptr->q_ptr = this;
}

QtTreeSelectionManager::QtTreeSelectionManager(QtTreeSelectionManagerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

QtTreeSelectionManager::~QtTreeSelectionManager()
{
    delete d_ptr;
}

void QtTreeSelectionManager::setCurrentItem(const QtTreeModelIterator &it)
{
    Q_D(QtTreeSelectionManager);
    QtTreeModelIterator previous = d->currentItem;
    d->currentItem = it;
    emit currentChanged(it, previous);
}

QtTreeModelIterator QtTreeSelectionManager::currentItem() const
{
    Q_D(const QtTreeSelectionManager);
    return d->currentItem;
}

void QtTreeSelectionManager::setAnchorItem(const QtTreeModelIterator &it)
{
    Q_D(QtTreeSelectionManager);
    QtTreeModelIterator previous = d->anchorItem;
    d->anchorItem = it;
    emit anchorChanged(it, previous);
}

QtTreeModelIterator QtTreeSelectionManager::anchorItem() const
{
    Q_D(const QtTreeSelectionManager);
    return d->anchorItem;
}

QtTreeSelectionManager::SelectionMode QtTreeSelectionManager::anchorSelectionMode() const
{
    Q_D(const QtTreeSelectionManager);
    return d->mode;
}

void QtTreeSelectionManager::setAnchorSelectionMode(SelectionMode mode)
{
    Q_D(QtTreeSelectionManager);
    if (d->mode != mode) {
        d->mode = mode;
        if (!d->anchorSelections.isEmpty())
            emit selectionsChanged(d->anchorSelections);
    }
}

void QtTreeSelectionManager::setSelected(const QtTreeSelection &selection, SelectionMode mode)
{
    Q_D(QtTreeSelectionManager);
    const QtTreeSelection previous = d->selections;
    switch (mode) {
    case Select:
        d->selections += selection;
        break;
    case Deselect:
        d->selections -= selection;
        break;
    case Toggle: { // ### optimize
            QtTreeSelection intersection = d->selections & selection;
            d->selections -= intersection;
            d->selections += (selection - intersection);
        }
    default:
        break;
    }
    emit selectionsChanged(previous + selection);
}

void QtTreeSelectionManager::setAnchorSelection(const QtTreeSelection &selection)
{
    Q_D(QtTreeSelectionManager);
    const QtTreeSelection previous = d->anchorSelections;
    d->anchorSelections = selection;
    emit selectionsChanged(previous + selection);
}

bool QtTreeSelectionManager::isSelected(const QtTreeModelBase::iterator_base &it) const
{
    Q_D(const QtTreeSelectionManager);
    if (d->anchorSelections.contains(it)) {
        switch (d->mode) {
        case Select:
            return true;
        case Deselect:
            return false;
        case Toggle:
            return !d->selections.contains(it);
        default:
            break;
        }
    }
    return d->selections.contains(it);
}

void QtTreeSelectionManager::clearSelections()
{
    Q_D(QtTreeSelectionManager);
    QtTreeSelection changed = d->selections + d->anchorSelections;
    d->selections.clear();
    d->anchorSelections.clear();
    emit selectionsChanged(changed);
}

QT_END_NAMESPACE

#include "moc_qtreeselectionmanager.cpp"
