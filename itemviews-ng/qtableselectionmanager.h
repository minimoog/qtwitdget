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

#ifndef QTTABLESELECTIONMANAGER_H
#define QTTABLESELECTIONMANAGER_H

#include "qitemviewsglobal.h"

#include <QtCore/qobject.h>
#include <QtCore/qpair.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtTableModelInterface;
class QtTableSelectionManagerPrivate;

// ### make shared ?
class Q_ITEMVIEWSNG_EXPORT QtTableSelectionRange
{
public:
    QtTableSelectionRange();
    QtTableSelectionRange(int top, int left, int bottom, int right);
    QtTableSelectionRange(const QtTableSelectionRange &other);
    ~QtTableSelectionRange();

    inline bool isValid() const { return (top > -1 && left > -1 && bottom > -1 && right > -1); }
    inline int topRow() const { return top; }
    inline int bottomRow() const { return bottom; }
    inline int leftColumn() const { return left; }
    inline int rightColumn() const { return right; }
    inline int rowCount() const { return (bottom > -1 && top > - 1 ? bottom - top + 1 : 0); }
    inline int columnCount() const { return (right > -1 && left > -1 ? right - left + 1 : 0); }
    inline bool intersectsRow(int row) const { return (row >= top && row <= bottom) && isValid(); }
    inline bool intersectsColumn(int column) const { return (column >= left && column <= right) && isValid(); }
    inline bool containsCell(int row, int column) const { return intersectsRow(row) && intersectsColumn(column) && isValid(); }

    bool intersects(const QtTableSelectionRange &other) const;
    QList<QtTableSelectionRange> subtracted(const QtTableSelectionRange &other) const;

    bool operator==(const QtTableSelectionRange&) const;
    bool operator!=(const QtTableSelectionRange&) const;

    void setRange(int topRow, int leftColumn, int bottomRow, int rightColumn);

private:
    int top, left, bottom, right;
};

Q_DECLARE_TYPEINFO(QtTableSelectionRange, Q_MOVABLE_TYPE);

typedef QPair<int,int> QtCell;

class Q_ITEMVIEWSNG_EXPORT QtTableSelectionManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QtCell currentCell READ currentCell WRITE setCurrentCell)
    Q_PROPERTY(QtCell anchorCell READ anchorCell WRITE setAnchorCell)
    Q_PROPERTY(bool anchorSelectionActive READ isAnchorSelectionActive)
    Q_PROPERTY(SelectionMode selectionMode READ selectionMode)
public:
    enum SelectionMode {
        Select,
        Deselect,
        Toggle
    };

    QtTableSelectionManager(QObject *parent = 0);
    ~QtTableSelectionManager();

    QtTableModelInterface *model() const;

    int currentRow() const;
    int currentColumn() const;
    inline QPair<int,int> currentCell() const { return QPair<int,int>(currentRow(), currentColumn()); }
    inline void setCurrentCell(const QPair<int,int> &cell) { setCurrentCell(cell.first, cell.second); }

    int anchorRow() const;
    int anchorColumn() const;
    inline QPair<int,int> anchorCell() const { return QPair<int,int>(anchorRow(), anchorColumn()); }
    inline void setAnchorCell(const QPair<int,int> &cell) { setAnchorCell(cell.first, cell.second); }

    bool isSelected(int row, int column) const;

    bool isAnchorSelectionActive() const;
    SelectionMode selectionMode() const;

    void beginAnchoredSelection(int anchorRow, int anchorColumn, SelectionMode mode = Select);
    void endAnchoredSelection();

    QList<QtTableSelectionRange> selectionRanges() const;

public Q_SLOTS:
    void setCurrentCell(int row, int column);
    void setAnchorCell(int row, int column);
    void setSelected(const QtTableSelectionRange &range, SelectionMode mode = Select);
    void clearSelections();

Q_SIGNALS:
    void currentChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void anchorChanged(int anchorRow, int anchorColumn, int previousRow, int previousColumn);
    void selectionsChanged(const QList<QtTableSelectionRange> &changed);

protected:
    QtTableSelectionManager(QtTableSelectionManagerPrivate&, QObject *parent = 0);
    QtTableSelectionManagerPrivate *d_ptr;
    void setModel(QtTableModelInterface *model);

private:
    friend class QtTableController;
    Q_DECLARE_PRIVATE(QtTableSelectionManager)
    Q_DISABLE_COPY(QtTableSelectionManager)
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_rowsInserted(int row, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsRemoved(int row, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsInserted(int column, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsRemoved(int column, int count))
};

#ifndef QT_NO_DEBUG_STREAM
Q_ITEMVIEWSNG_EXPORT QDebug operator<<(QDebug, const QtTableSelectionRange &);
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTABLESELECTIONMANAGER_H
