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

#ifndef QTGRAPHICSTABLEVIEW_P_H
#define QTGRAPHICSTABLEVIEW_P_H

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

#include <qhash.h>
#include <qfont.h>
#include <qvariant.h>

#include "qtableselectionmanager.h"

QT_BEGIN_NAMESPACE

class QtTableModelInterface;
class QtTableSelectionManager;
class QtTableSelectionRange;
class QtTableController;
class QtGraphicsHeader;

class QtGraphicsTableView;

class QtGraphicsTableViewItemPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsTableViewItem)
public:
    int row;
    int column;
    QtGraphicsTableView *view;
    mutable QStyleOptionViewItemV4 option;
    QtGraphicsTableViewItem *q_ptr;
};

class QtGraphicsTableViewPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsTableView)
public:
    QtGraphicsTableViewPrivate();
    ~QtGraphicsTableViewPrivate();

    void _q_controllerDestroyed();
    void _q_modelDestroyed();
    void _q_selectionsDestroyed();
    void _q_verticalHeaderDestroyed();
    void _q_horizontalHeaderDestroyed();

    void _q_cellsChanged(int firstRow, int firstColumn, int rowCount, int columnCount, const QList<QByteArray> &roles);
    void _q_selectionsChanged(const QList<QtTableSelectionRange> &changed);
    void _q_currentChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void _q_rowsInserted(int row, int count);
    void _q_rowsRemoved(int row, int count);
    void _q_rowsMoved(int from, int to, int count);
    void _q_columnsInserted(int column, int count);
    void _q_columnsRemoved(int column, int count);
    void _q_columnsMoved(int from, int to, int count);

    bool isSelected(int row, int column) const;
    bool isCurrent(int row, int column) const;

    void checkItems(int firstRow, int firstColumn, int rowCount, int columnCount);
    void checkCache(int firstRow, int firstColumn, int rowCount, int columnCount);

    QVariant cachedData(int row, int column, const QByteArray &role) const;
    
    QtTableController *controller;
    QtTableModelInterface *model;
    QtTableSelectionManager *selectionManager;
    QtGraphicsHeader *horizontalHeader;
    QtGraphicsHeader *verticalHeader;

    Qt::TextElideMode textElideMode;

    // only used when we have no headers
    qreal defaultRowHeight;
    qreal defaultColumnWidth;
    int firstRow;
    int firstColumn;
    qreal horizontalOffset;
    qreal verticalOffset;
    bool showGrid;

    // cached data
    mutable QHash<QByteArray,QVariant> cachedDataHash;
    mutable int cachedDataRow;
    mutable int cachedDataColumn;

    // view items
    QtTableSelectionRange visibleSections;
    QHash<qint64, QGraphicsObject*> items;
    QtGraphicsTableViewItemCreatorBase *creator;

    QtGraphicsTableView *q_ptr;
};

QT_END_NAMESPACE

#endif//QTGRAPHICSTABLEVIEW_P_H
