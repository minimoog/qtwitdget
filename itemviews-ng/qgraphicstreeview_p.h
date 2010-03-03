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

#ifndef QTGRAPHICSTREEVIEW_P_H
#define QTGRAPHICSTREEVIEW_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qtreemodelinterface.h"
#include "qtreeselectionmanager.h"

#include <qstack.h>
#include <qtimer.h>
#include <qstyleoption.h>

QT_BEGIN_NAMESPACE

/*
  Notes To The Maintainer
  -----------------------

  1. The vertical offset
  The offset, in pixels, before items are drawn.

  2. The first index
  The first item to be drawn.

  3. The iterator stack
  This is an optimization. In order to avoid calls to parent, we traverse the tree with the help of a stack.
  The next() and previous() functions also use the stack because we might have to get the parent if we are
  skipping a closed branch or moving from the last leaf in one branch to the next item.

  4. The cached first index and cached first item
  This pair of variables contains the cached mapping between the first index and the first iterator.
  Both these values are referring to the same item, the index is the flat index in the tree control.

  5. The index to item mapping
  This is an optimization. It uses a mapping from flat index to iterator. The mapping only maps every
  mapIndexInterval. This allows fast lookup of iterator values without having to iterate from begin()
  every time.

  6. The cached data map
  ...

  Note that it should be possible to disable all optimizations and still have a working layout.
*/

class QtGraphicsHeader;
class QtTreeController;
class QtGraphicsTreeView;
class QtGraphicsTreeViewItemPrivate;

class QtGraphicsTreeViewItemPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsTreeViewItem)
public:
    int index;
    QtTreeModelIterator it;
    QtGraphicsTreeView *view;
    QtGraphicsHeader *header;
    mutable QStyleOptionViewItemV4 option;
    QtGraphicsTreeViewItem *q_ptr;
};

#define MAPPING_ENABLED 1

class QtGraphicsTreeViewPrivate //: public QGraphicsWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsTreeView)
public:
    QtGraphicsTreeViewPrivate();
    ~QtGraphicsTreeViewPrivate();
    
    void _q_controllerDestroyed();
    void _q_modelDestroyed();
    void _q_selectionsDestroyed();
    void _q_headerDestroyed();

    void _q_itemsChanged(const QtTreeModelBase::iterator_base &it, int count, const QList<QByteArray> &roles);
    void _q_itemsInserted(const QtTreeModelBase::iterator_base &it, int count);
    void _q_itemsRemoved(const QtTreeModelBase::iterator_base &it, int count);
    void _q_reset();

    void _q_selectionsChanged(const QtTreeSelection &selections);
    void _q_currentChanged(const QtTreeModelBase::iterator_base &current, const QtTreeModelBase::iterator_base &previous);

    void checkCache(const QtTreeModelBase::iterator_base &it, int count);
    void checkMapping(const QtTreeModelBase::iterator_base &it, int count);
    void checkHeader(const QtTreeModelBase::iterator_base &it, int count);

    QVariant cachedData(const QtTreeModelBase::iterator_base &it, int column, const QByteArray &role) const;
    bool isCurrent(const QtTreeModelBase::iterator_base &it, int column) const;
    bool isSelected(const QtTreeModelBase::iterator_base &it, int column) const;
    bool isExpanded(const QtTreeModelBase::iterator_base &it) const;

    void expand(QtTreeModelBase::iterator_base &it);
    void collapse(QtTreeModelBase::iterator_base &it);

    void syncStack(const QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const;
    void nextBranch(QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const;
    void next(QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const;
    void previous(QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const;
    QtTreeModelBase::iterator_base firstItem(QStack<QtTreeModelBase::iterator_base> &stack, int *flat = 0) const;

    qreal itemHeight(QStyleOptionViewItemV4 &option, const QtTreeModelBase::iterator_base &it) const;
    QRectF disclosureRect(const QRectF &rect) const;

    qreal columnWidth(int column) const;
    qreal columnPosition(int column) const;

    int visibleChildCount(const QtTreeModelBase::iterator_base &it) const;
#if MAPPING_ENABLED
    void _q_mapIndexesToItems();
#endif

    void updateItem(const QtTreeModelBase::iterator_base &it) const;
    
    QtGraphicsTreeView *q_ptr;
    QtTreeController *controller;
    QtTreeModelBase *model;
    QtTreeSelectionManager *selectionManager;
    QtGraphicsHeader *header;

    Qt::TextElideMode textElideMode;

    mutable QHash<QByteArray, QVariant> cachedDataHash;
    mutable QtTreeModelBase::iterator_base cachedDataItem;
    mutable int cachedDataColumn;

    qreal verticalOffset;
    int firstIndex;

    QSet<QtTreeModelBase::iterator_base> expandedItems;

    mutable int cachedFirstIndex;
    mutable QtTreeModelBase::iterator_base cachedFirstItem;
    mutable qreal cachedVerticalOffset;
    mutable QtTreeModelBase::iterator_base cachedFirstVisibleItem;

    // About mapping: the idea is that the control should work without mapping.
    // The mapping is there only to speed up the algorithms; but it should be
    // possible to remove the mapping and still have a functional, if somewhat
    // slower, tree control.
#if MAPPING_ENABLED
    mutable QHash<int, QtTreeModelBase::iterator_base> mapIndexToItem;
    static const int mapIndexInterval = 100; // ### map every 100th index
#endif

    static inline qreal indentation() { return 20; } // ### hardcoded value; make styleHint

    QHash<QtTreeModelBase::iterator_base, QGraphicsObject*> items;

    QtGraphicsTreeViewItemCreatorBase *creator;
    QBasicTimer layoutTimer;
};

QT_END_NAMESPACE

#endif//QTGRAPHICSTREEVIEW_P_H
