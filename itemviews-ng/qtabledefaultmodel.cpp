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

#include "qtabledefaultmodel.h"

QT_BEGIN_NAMESPACE

class QtTableDefaultItemPrivate
{
public:
    inline QtTableDefaultItemPrivate() : model(0), index(-1) {}
    QHash<QByteArray,QVariant> data;
    QtTableDefaultModel *model;
    mutable int index;
};

/*!
  \class QtTableDefaultItem
  \brief A table model item
*/

/*!
 */
QtTableDefaultItem::QtTableDefaultItem()
    : d_ptr(new QtTableDefaultItemPrivate)
{
}

/*!
 */
QtTableDefaultItem::QtTableDefaultItem(const QString &text)
    : d_ptr(new QtTableDefaultItemPrivate)
{
    d_ptr->data.insert("DisplayRole", text);
}

/*!
 */
QtTableDefaultItem::~QtTableDefaultItem()
{
    if (d_ptr->model)
        d_ptr->model->itemDeleted(this);
    delete d_ptr;
}

#ifndef QT_NO_DATASTREAM
/*!
 */
void QtTableDefaultItem::read(QDataStream &in)
{
    in >> d_ptr->data;
}

/*!
 */
void QtTableDefaultItem::write(QDataStream &out) const
{
    out << d_ptr->data;
}
#endif

/*!
 */
QHash<QByteArray,QVariant> QtTableDefaultItem::data(const QList<QByteArray> &roles) const
{
    /*
    QHash<int,QVariant> hash;
    QList<int>::const_iterator it = roles.constBegin();
    for (; it != roles.constEnd(); ++it)
        hash.insert(*it, d_ptr->data.value(*it));
    return hash;
    */
    Q_UNUSED(roles);
    return d_ptr->data;
}

/*!
 */
void QtTableDefaultItem::setData(const QHash<QByteArray,QVariant> &data)
{
    QList<QByteArray> roles;
    QHash<QByteArray,QVariant>::const_iterator it = data.constBegin();
    for (; it != data.constEnd(); ++it) {
        d_ptr->data.insert(it.key(), it.value());
        roles << it.key();
    }
    itemChanged(roles);
}

/*!
 */
void QtTableDefaultItem::itemChanged(const QList<QByteArray> &roles)
{
    if (d_ptr->model)
        d_ptr->model->itemChanged(this, roles);
}

// QtTableDefaultModelPrivate

class QtTableDefaultModelPrivate
{
    Q_DECLARE_PUBLIC(QtTableDefaultModel)
public:
    QtTableDefaultModelPrivate();
    ~QtTableDefaultModelPrivate();
    void insertRows(int row, int count);
    void removeRows(int row, int count);
    void moveRows(int fromRow, int toRow, int count);
    void insertColumns(int column, int count);
    void removeColumns(int column, int count);
    void moveColumns(int fromColumn, int toColumn, int count);
    void clearContents();
    void clear();

    inline int index(int row, int column) const { return (row * columnCount) + column; }
    
    QtTableDefaultModel *q_ptr;
    int rowCount;
    int columnCount;
    QVector<QtTableDefaultItem*> cells;
};

QtTableDefaultModelPrivate::QtTableDefaultModelPrivate()
    : q_ptr(0), rowCount(0), columnCount(0)
{
}

QtTableDefaultModelPrivate::~QtTableDefaultModelPrivate()
{
    clearContents();
}

void QtTableDefaultModelPrivate::insertRows(int row, int count)
{
    row = qBound(0, row, rowCount);
    cells.insert(index(row, 0), count * columnCount, 0);
    ++rowCount;
}

void QtTableDefaultModelPrivate::removeRows(int row, int count)
{
    row = qBound(0, row, rowCount - 1);
    int first = index(row, 0);
    int last = first + (count * columnCount) - 1;
    for (int i = first; i <= last; ++i)
        delete cells.at(i);
    cells.remove(first, count * columnCount);
    --rowCount;
}

void QtTableDefaultModelPrivate::moveRows(int fromRow, int toRow, int count)
{
    int from = index(fromRow, 0);
    int to = index(toRow, 0);
    int moveCount = count * columnCount;
    QVector<QtTableDefaultItem*> moved = cells.mid(from, moveCount);
    if (from < to) {
        for (int i = 0; i < (to - (from + moveCount)); ++i)
            cells[from + i] = cells.at(from + moveCount + i);
    } else {
        for (int i = from + moveCount - 1; i >= to + moveCount; --i)
            cells[i] = cells.at(i - moveCount);
    }
}

void QtTableDefaultModelPrivate::insertColumns(int column, int count)
{
    column = qBound(0, column, columnCount);
    for (int row = 0; row < rowCount; ++row)
        cells.insert(index(row, column), count, 0);
    ++columnCount;
}

void QtTableDefaultModelPrivate::removeColumns(int column, int count)
{
    column = qBound(0, column, columnCount - 1);
    for (int row = rowCount - 1; row >= 0; --row) {
        int first = index(row, column);
        int last = first + count - 1;
        for (int i = first; i <= last; ++i)
            delete cells.at(i);
        cells.remove(first, count);
    }
    --columnCount;
}

void QtTableDefaultModelPrivate::moveColumns(int fromColumn, int toColumn, int count)
{
    QVector<QtTableDefaultItem*> moved(count);
    for (int row = 0; row < rowCount; ++row) {
        int from = index(row, fromColumn);
        int to = index(row, toColumn);
        // save the cells we are moving in this row
        for (int i = 0; i < count; ++i)
            moved[i] = cells[from + i];
        // shift the remaining cells
        if (from < to) {
            for (int j = from + count; j < to; ++j)
                cells[j - count] = cells.at(j);
        } else {
            for (int j = from - 1; j >= (to + count); --j)
                cells[j + count] = cells.at(j);
        }
        // insert the cells in their new columns
        for (int k = 0; k < count; ++k)
            cells[to + k] = moved.at(k);
    }
}

void QtTableDefaultModelPrivate::clearContents()
{
    QVector<QtTableDefaultItem*>::iterator it = cells.begin();
    for (; it != cells.end(); ++it) {
        if (*it) {
            delete *it;
            *it = 0;
        }
    }
}

void QtTableDefaultModelPrivate::clear()
{
    clearContents();
    rowCount = 0;
    columnCount = 0;
    cells.clear();
}

// QtTableDefaultModel

/*!
  \class QtTableDefaultModel
  \brief A table item model
*/

/*!
 */
QtTableDefaultModel::QtTableDefaultModel(QObject *parent)
    : QtTableModelInterface(parent), d_ptr(new QtTableDefaultModelPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTableDefaultModel::QtTableDefaultModel(int rows, int columns, QObject *parent)
    : QtTableModelInterface(parent), d_ptr(new QtTableDefaultModelPrivate)
{
    Q_D(QtTableDefaultModel);
    d->q_ptr = this;
    d->rowCount = rows;
    d->columnCount = columns;
    d->cells.resize(rows * columns);
}

/*!
 */
QtTableDefaultModel::QtTableDefaultModel(QtTableDefaultModelPrivate &dd, QObject *parent)
    : QtTableModelInterface(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtTableDefaultModel::~QtTableDefaultModel()
{
    delete d_ptr;
}

/*!
 */
void QtTableDefaultModel::setRowCount(int rows)
{
    Q_D(QtTableDefaultModel);
    int count = rows - d->rowCount;
    if (count > 0) {
        d->cells.resize(rows * d->columnCount);
        d->rowCount = rows;
        emit rowsInserted(rows - count, count);
    } else if (count < 0) {
        int first = d->index(rows, 0);
        int last = d->index(rows - count, 0) - 1;
        for (int i = first; i <= last; ++i) {
            if (QtTableDefaultItem *item = d->cells.at(i)) {
                item->d_ptr->model = 0; // don't call itemDeleted
                delete item;
            }
        }
        d->cells.resize(rows * d->columnCount);
        d->rowCount = rows; // affects index()
        emit rowsRemoved(rows, -count);
    }
}

/*!
 */
int QtTableDefaultModel::rowCount() const
{
    Q_D(const QtTableDefaultModel);
    return d->rowCount;
}

/*!
 */
void QtTableDefaultModel::setColumnCount(int columns)
{
    Q_D(QtTableDefaultModel);
    int count = columns - d->columnCount;
    if (count > 0) {
        for (int row = 0; row < d->rowCount; ++row)
            d->cells.insert(d->index(row, columns - count), count, 0);
        d->columnCount = columns;
        emit columnsInserted(columns - count, count);
    } else if (count < 0) {
        for (int row = d->rowCount - 1; row >= 0; --row) {
            int first = d->index(row, columns);
            int last = d->index(row, columns - count);
            for (int i = first; i <= last; ++i) {
                if (QtTableDefaultItem *item = d->cells.at(i)) {
                    item->d_ptr->model = 0; // don't call itemDeleted
                    delete item;
                }
            }
            d->cells.remove(first, -count);
        }
        d->columnCount = columns; // affects index()
        emit columnsRemoved(columns, -count);
    }
}

/*!
 */
int QtTableDefaultModel::columnCount() const
{
    Q_D(const QtTableDefaultModel);
    return d->columnCount;
}

/*!
 */
int QtTableDefaultModel::itemRow(const QtTableDefaultItem *item) const
{
    Q_D(const QtTableDefaultModel);
    if (item == 0)
        return -1;
    int index = item->d_ptr->index;
    if (index >= 0 && index < d->cells.count() && d->cells.at(index) == item)
        return index / d->columnCount;
    index = d->cells.indexOf(const_cast<QtTableDefaultItem*>(item));
    if (index != -1) {
        item->d_ptr->index = index;
        return index / d->columnCount;
    }
    return -1;
}

/*!
 */
int QtTableDefaultModel::itemColumn(const QtTableDefaultItem *item) const
{
    Q_D(const QtTableDefaultModel);
    if (item == 0)
        return -1;
    int index = item->d_ptr->index;
    if (d->cells.at(index) == item)
        return index % d->columnCount;
    index = d->cells.indexOf(const_cast<QtTableDefaultItem*>(item));
    if (index != -1) {
        item->d_ptr->index = index;
        return index % d->columnCount;
    }
    return -1;
}

/*!
 */
QtTableDefaultItem *QtTableDefaultModel::item(int row, int column) const
{
    Q_D(const QtTableDefaultModel);
    if (row >= 0 && column >= 0 && row < d->rowCount && column < d->columnCount)
        return d->cells.at(d->index(row, column));
    return 0;
}

/*!
 */
void QtTableDefaultModel::setItem(int row, int column, QtTableDefaultItem *item)
{
    Q_D(QtTableDefaultModel);
    if (item && row >= 0 && column >= 0 && row < d->rowCount && column < d->columnCount) {
        int index = d->index(row, column);
        item->d_ptr->index = index;
        d->cells[index] = item;
        emit cellsChanged(row, column, 1, 1, QList<QByteArray>());
    }
}

/*!
 */
QHash<QByteArray,QVariant> QtTableDefaultModel::data(int row, int column, const QList<QByteArray> &roles) const
{
    if (const QtTableDefaultItem *itm = item(row, column))
        return itm->data(roles);
    return QHash<QByteArray,QVariant>();
}

/*!
 */
bool QtTableDefaultModel::setData(int row, int column, const QHash<QByteArray,QVariant> &values)
{
    Q_D(QtTableDefaultModel);
    int index = d->index(row, column);
    if (index < 0 || index >= d->cells.count())
        return false;
    QtTableDefaultItem *item = d->cells.at(index);
    if (item) {
        item->d_ptr->data.unite(values);
    } else {
        item = new QtTableDefaultItem;
        item->d_ptr->data  = values;
        item->d_ptr->index = index;
        d->cells[index] = item;
    }
    emit cellsChanged(row, column, 1, 1, values.keys());
    return true;
}

/*!
 */
void QtTableDefaultModel::insertRow(int row)
{
    Q_D(QtTableDefaultModel);
    d->insertRows(row, 1);
    emit rowsInserted(row, 1);
}

/*!
 */
void QtTableDefaultModel::removeRow(int row)
{
    Q_D(QtTableDefaultModel);
    d->removeRows(row, 1);
    emit rowsRemoved(row, 1);
}

/*!
 */
void QtTableDefaultModel::moveRow(int fromRow, int toRow)
{
    Q_D(QtTableDefaultModel);
    d->moveRows(fromRow, toRow, 1);
    emit rowsMoved(fromRow, toRow, 1);
}

/*!
 */
void QtTableDefaultModel::insertColumn(int column)
{
    Q_D(QtTableDefaultModel);
    d->insertColumns(column, 1);
    emit columnsInserted(column, 1);
}

/*!
 */
void QtTableDefaultModel::removeColumn(int column)
{
    Q_D(QtTableDefaultModel);
    d->removeColumns(column, 1);
    emit columnsRemoved(column, 1);
}

/*!
 */
void QtTableDefaultModel::moveColumn(int fromColumn, int toColumn)
{
    Q_D(QtTableDefaultModel);
    d->moveColumns(fromColumn, toColumn, 1);
    emit columnsMoved(fromColumn, toColumn, 1);
}

/*!
 */
void QtTableDefaultModel::clearContents()
{
    Q_D(QtTableDefaultModel);
    d->clearContents();
    emit cellsChanged(0, 0, d->rowCount, d->columnCount, QList<QByteArray>());
}

/*!
 */
void QtTableDefaultModel::clear()
{
    Q_D(QtTableDefaultModel);
    int rowCount = d->rowCount;
    d->clear();
    emit rowsRemoved(0, rowCount);
}

/*!
 */
void QtTableDefaultModel::itemChanged(QtTableDefaultItem *item, const QList<QByteArray> &roles)
{
    if (item) {
        int row = itemRow(item);
        int column = itemColumn(item);
        emit cellsChanged(row, column, 1, 1, roles);
    }
}

/*!
 */
void QtTableDefaultModel::itemDeleted(QtTableDefaultItem *item)
{
    if (item) {
        int row = itemRow(item);
        int column = itemColumn(item);
        emit cellsChanged(row, column, 1, 1, QList<QByteArray>());
    }
}

QT_END_NAMESPACE

#include "moc_qtabledefaultmodel.cpp"
