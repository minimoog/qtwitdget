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

#include "qlistdefaultmodel.h"

QT_BEGIN_NAMESPACE

class QtListDefaultItemPrivate
{
public:
    inline QtListDefaultItemPrivate() : model(0), index(-1) {}
    QHash<QByteArray,QVariant> data;
    QtListDefaultModel *model;
    mutable int index;
};

// QtListDefaultItem

/*!
  \class QtListDefaultItem
  \brief A list model item
*/

/*!
 */
QtListDefaultItem::QtListDefaultItem()
    : d_ptr(new QtListDefaultItemPrivate)
{
}

/*!
 */
QtListDefaultItem::QtListDefaultItem(const QString &text)
    : d_ptr(new QtListDefaultItemPrivate)
{
    d_ptr->data.insert("DisplayRole", text);
}

/*!
 */
QtListDefaultItem::~QtListDefaultItem()
{
    if (d_ptr->model)
        d_ptr->model->itemDeleted(this);
    delete d_ptr;
}

#ifndef QT_NO_DATASTREAM
/*!
 */
void QtListDefaultItem::read(QDataStream &in)
{
    in >> d_ptr->data;
}

/*!
 */
void QtListDefaultItem::write(QDataStream &out) const
{
    out << d_ptr->data;
}
#endif

/*!
 */
QHash<QByteArray,QVariant> QtListDefaultItem::data(const QList<QByteArray> &roles) const
{
    Q_UNUSED(roles);
    return d_ptr->data;
}

/*!
 */
void QtListDefaultItem::setData(const QHash<QByteArray,QVariant> &data)
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
void QtListDefaultItem::itemChanged(const QList<QByteArray> &roles)
{
    if (d_ptr->model)
        d_ptr->model->itemChanged(this, roles);
}

// QtListDefaultModelPrivate

class QtListDefaultModelPrivate
{
    Q_DECLARE_PUBLIC(QtListDefaultModel)
public:
    QtListDefaultModelPrivate();
    ~QtListDefaultModelPrivate();

    void insertItems(int index, int count);
    void removeItems(int index, int count);
    void moveItems(int from, int to, int count);
    void clear();

    QtListDefaultModel *q_ptr;
    QList<QtListDefaultItem*> items;
};

QtListDefaultModelPrivate::QtListDefaultModelPrivate()
    : q_ptr(0)
{
}

QtListDefaultModelPrivate::~QtListDefaultModelPrivate()
{
    clear();
}

void QtListDefaultModelPrivate::insertItems(int index, int count)
{
    for (int i = 0; i < count; ++i)
        items.insert(index, 0);
}

void QtListDefaultModelPrivate::removeItems(int index, int count)
{
    for (int i = 0; i < count; ++i)
        items.removeAt(index);
}

void QtListDefaultModelPrivate::moveItems(int from, int to, int count)
{
    for (int i = 0; i < count; ++i)
        items.move(from, to);
}

void QtListDefaultModelPrivate::clear()
{
    QList<QtListDefaultItem*>::iterator it = items.begin();
    for (; it != items.end(); ++it)
        if (*it)
            delete *it;
    items.clear();
}

// QtListDefaultModel

/*!
    \class QtListDefaultModel
    \brief provides a default list item model.

    QtListDefaultModel implements QtListModelInterface so that you can get
    started with a default list model, without having to do any subclassing
    yourself.

    \sa QtListModelInterface
*/

/*!
    Constructs a list model with the given \a parent.
*/
QtListDefaultModel::QtListDefaultModel(QObject *parent)
    : QtListModelInterface(parent), d_ptr(new QtListDefaultModelPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
    \internal
*/
QtListDefaultModel::QtListDefaultModel(QtListDefaultModelPrivate &dd, QObject *parent)
    : QtListModelInterface(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys the list model.
*/
QtListDefaultModel::~QtListDefaultModel()
{
    delete d_ptr;
}

/*!
    Returns the index of the given \a item.
*/
int QtListDefaultModel::indexOf(const QtListDefaultItem *item) const
{
    Q_D(const QtListDefaultModel);
    if (item == 0)
        return -1;
    int index = item->d_ptr->index;
    if (index >= 0 && index < d->items.count() && d->items.at(index) == item)
        return index;
    index = d->items.indexOf(const_cast<QtListDefaultItem*>(item));
    if (index != -1)
        item->d_ptr->index = index;
    return index;
}

/*!
    Returns the item for the given \a index.
*/
QtListDefaultItem *QtListDefaultModel::item(int index) const
{
    Q_D(const QtListDefaultModel);
    if (index >= 0 && index < d->items.count())
        return d->items.at(index);
    return 0;
}

/*!
    Returns the number of items in the model.
*/
int QtListDefaultModel::count() const
{
    Q_D(const QtListDefaultModel);
    return d->items.count();
}

/*!
    Returns the data stored for the gievn \a index, according to the given
    \a roles.

    \sa setData()
*/
QHash<QByteArray,QVariant> QtListDefaultModel::data(int index, const QList<QByteArray> &roles) const
{
    if (const QtListDefaultItem *itm = item(index))
        return itm->data(roles);
    return QHash<QByteArray,QVariant>();
}

/*!
    Sets the item at \a index to \a values.
    \a values is a QHash role and data as the key-value pair.

    \sa data()
*/
bool QtListDefaultModel::setData(int index, const QHash<QByteArray,QVariant> &values)
{
    Q_D(QtListDefaultModel);
    if (index < 0 || index >= d->items.count())
        return false;
    QtListDefaultItem *item = d->items.at(index);
    if (item) {
        item->d_ptr->data.unite(values);
    } else {
        item = new QtListDefaultItem;
        item->d_ptr->data = values;
        item->d_ptr->index = index;
        d->items[index] = item;
    }
    emit itemsChanged(index, 1, values.keys());
    return true;
}

/*!
    Appends the given \a item to the model.
*/
void QtListDefaultModel::appendItem(QtListDefaultItem *item)
{
    insertItem(count(), item);
}

/*!
    Inserts the given \a item at \a index.

    \sa removeItem()
*/
void QtListDefaultModel::insertItem(int index, QtListDefaultItem *item)
{
    Q_D(QtListDefaultModel);
    index = qBound(0, index, d->items.count());
    d->insertItems(index, 1);
    d->items[index] = item;
    emit itemsInserted(index, 1);
}

/*!
    Removes the given \a item at \a index.

    \sa insertItem()
*/
void QtListDefaultModel::removeItem(int index)
{
    Q_D(QtListDefaultModel);
    delete d->items.at(index);
    d->removeItems(index, 1);
    emit itemsRemoved(index, 1);
}

/*!
    \fn void QtListDefaultModel::moveItem(int a, int b)

    Moves the item from position \a a to \a b.
*/
void QtListDefaultModel::moveItem(int from, int to)
{
    Q_D(QtListDefaultModel);
    d->moveItems(from, to, 1);
    emit itemsMoved(from, to, 1);
}

/*!
    Clears the model; all items will be deleted.
*/
void QtListDefaultModel::clear()
{
    Q_D(QtListDefaultModel);
    int count = d->items.count();
    d->clear();
    emit itemsRemoved(0, count);
}

/*!
*/
void QtListDefaultModel::itemChanged(QtListDefaultItem *item, const QList<QByteArray> &roles)
{
    if (item) {
        int index = indexOf(item);
        emit itemsChanged(index, 1, roles);
    }
}

/*!
*/
void QtListDefaultModel::itemDeleted(QtListDefaultItem *item)
{
    Q_D(QtListDefaultModel);
    if (item) {
        int index = indexOf(item);
        d->removeItems(index, 1);
        emit itemsRemoved(index, 1);
    }
}

QT_END_NAMESPACE

#include "moc_qlistdefaultmodel.cpp"
