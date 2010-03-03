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

#include "qtreedefaultmodel.h"

QT_BEGIN_NAMESPACE

// QtTreeDefaultItemPrivate

struct QtTreeDefaultItemPrivate
{
    QtTreeDefaultItemPrivate() : parent(0), previous(0), next(0), model(0) {}
    QVector<QHash<QByteArray,QVariant> > data;
    QtTreeDefaultItem *parent;
    QtTreeDefaultItem *previous;
    QtTreeDefaultItem *next;
    QList<QtTreeDefaultItem*> children;
    QtTreeDefaultModel *model;
};

// QtTreeDefaultItem

/*!
  Constructs a tree model item and appends it as a child of the given \a parent.
 */
QtTreeDefaultItem::QtTreeDefaultItem(QtTreeDefaultItem *parent)
    :d_ptr(new QtTreeDefaultItemPrivate)
{
    if (parent) {
        d_ptr->parent = parent;
        d_ptr->parent->appendChild(this);
    }
    d_ptr->data.append(QHash<QByteArray,QVariant>());
}

/*!
  Constructs a tree model item and appends it as a child of the given \a parent.
  The given \a text will be set as the item text.
 */
QtTreeDefaultItem::QtTreeDefaultItem(const QString &text, QtTreeDefaultItem *parent)
    :d_ptr(new QtTreeDefaultItemPrivate)
{
    if (parent) {
        d_ptr->parent = parent;
        d_ptr->parent->appendChild(this);
    }
    d_ptr->data.append(QHash<QByteArray,QVariant>());
    d_ptr->data.last().insert("DisplayRole", text);
}

/*!
  Destroys the tree model item.
 */
QtTreeDefaultItem::~QtTreeDefaultItem()
{
    if (d_ptr->parent)
        d_ptr->parent->removeChild(this);
    for (int i = 0; i < d_ptr->children.count(); ++i) {
        QtTreeDefaultItem *child = d_ptr->children.at(i);
        child->d_ptr->parent = 0;
        delete child;
    }
    delete d_ptr;
}

/*!
  Returns the item's parent.
  \sa children()
 */
QtTreeDefaultItem *QtTreeDefaultItem::parent() const
{
    return d_ptr->parent;
}

/*!
  Returns the item's first child.
  \sa parent()
 */
QtTreeDefaultItem *QtTreeDefaultItem::firstChild() const
{
    return d_ptr->children.isEmpty() ? 0 : d_ptr->children.first();
}

/*!
  Returns the item's next sibling.
  \sa previous()
 */
QtTreeDefaultItem *QtTreeDefaultItem::next() const
{
    return d_ptr->next;
}

/*!
  Returns the item's previous sibling.
  \sa next()
 */
QtTreeDefaultItem *QtTreeDefaultItem::previous() const
{
    return d_ptr->previous;
}

/*!
  Returns the number of columns in the item.
 */
int QtTreeDefaultItem::columnCount() const
{
    return d_ptr->data.count();
}

/*!
  Sets the items column count to be the given \a count.
  \sa columnCount()
 */
void QtTreeDefaultItem::setColumnCount(int count)
{
    if (d_ptr->data.count() != count) {
        d_ptr->data.resize(count);
        if (d_ptr->model)
            d_ptr->model->itemChanged(this, QList<QByteArray>());
    }
}

/*!
  Returns the item's children.
  \sa parent()
 */
const QList<QtTreeDefaultItem*> &QtTreeDefaultItem::children() const
{
    return d_ptr->children;
}

/*!
  Inserts the \a child at \a i in the list of children.
  \sa appendChild(), takeChild(), removeChild()
 */
void QtTreeDefaultItem::insertChild(int i, QtTreeDefaultItem *child)
{
    i = qBound(0, i, d_ptr->children.count());
    QtTreeDefaultItem *previous = i > 0 ? d_ptr->children.value(i - 1) : 0;
    child->d_ptr->previous = previous;
    if (previous)
        previous->d_ptr->next = child;
    QtTreeDefaultItem *next = i < d_ptr->children.count() ? d_ptr->children.value(i) : 0;
    child->d_ptr->next = next;
    if (next)
        next->d_ptr->previous = child;
    child->d_ptr->model = d_ptr->model;
    d_ptr->children.insert(i, child);
    if (d_ptr->model)
        d_ptr->model->itemInserted(child);
}

/*!
  Appends the \a child to the list of children.
  \sa insertChild(), takeChild(), removeChild()
 */
void QtTreeDefaultItem::appendChild(QtTreeDefaultItem *child)
{
    QtTreeDefaultItem *previous = (d_ptr->children.isEmpty() ? 0 : d_ptr->children.last());
    child->d_ptr->previous = previous;
    if (previous)
        previous->d_ptr->next = child;
    child->d_ptr->model = d_ptr->model;
    child->d_ptr->next = 0;
    d_ptr->children.append(child);
    if (d_ptr->model)
        d_ptr->model->itemInserted(child);
}

/*!
  Removes the item at \a i and retuns it, otherwise returns 0.
  \sa appendChild(), insertChild(), removeChild()
 */
QtTreeDefaultItem *QtTreeDefaultItem::takeChild(int i)
{
    if (i < 0 || i >= d_ptr->children.count())
        return 0;
    QtTreeDefaultItem *previous = d_ptr->children.value(i - 1);
    QtTreeDefaultItem *next = d_ptr->children.value(i + 1);
    if (previous)
        previous->d_ptr->next = next;
    if (next)
        next->d_ptr->previous = previous;
    QtTreeDefaultItem *child = d_ptr->children.takeAt(i);
    child->d_ptr->previous = 0;
    child->d_ptr->next = 0;
    child->d_ptr->parent = 0;
    child->d_ptr->model = 0;
    if (d_ptr->model)
        d_ptr->model->itemRemoved(child);
    return child;
}

/*!
  Removes the given \a child from the list of children.
  \sa takeChild(), appendChild(), insertChild()
 */
void QtTreeDefaultItem::removeChild(QtTreeDefaultItem *child)
{
    int i = d_ptr->children.indexOf(child);
    if (i != -1)
        (void)takeChild(i);
}

#ifndef QT_NO_DATASTREAM
/*!
  Reads the item from stream \a in. This only reads data into a single item.

  \sa write()
*/
void QtTreeDefaultItem::read(QDataStream &in)
{
    in >> d_ptr->data;
}

/*!
  Writes the item to stream \a out. This only writes data from one single item.
  
  \sa read()
*/
void QtTreeDefaultItem::write(QDataStream &out) const
{
    out << d_ptr->data;
}
#endif

/*!
  Returns the data for the item's \a column and \a roles.
  \sa setData()
 */
QHash<QByteArray,QVariant> QtTreeDefaultItem::data(int column, const QList<QByteArray> &roles) const
{
    /*
    QHash<int, QVariant> retval;
    const QHash<int, QVariant> &columnData = d_ptr->data.value(column);
    for (int i = 0; i < roles.count(); ++i)
        retval.insert(roles.at(i), columnData.value(roles.at(i)));
    return retval;
    */
    Q_UNUSED(roles);
    return d_ptr->data.value(column);
}

/*!
  Returns the data for the item's \a column and \a role.
  \sa setData()
 */
QVariant QtTreeDefaultItem::data(int column, const QByteArray &role) const
{
    return d_ptr->data.value(column).value(role);
}

/*!
  Sets the value for the item's \a column and \a role to the given \a data.
  \sa data()
 */
void QtTreeDefaultItem::setData(const QVariant &data, int column, const QByteArray &role)
{
    if (column >= 0 && column < d_ptr->data.count()) {
        d_ptr->data[column].insert(role, data);
        if (d_ptr->model)
            d_ptr->model->itemChanged(this, QList<QByteArray>() << role);
    }
}

// QtTreeDefaultModelPrivate

class QtTreeDefaultModelPrivate
{
    Q_DECLARE_PUBLIC(QtTreeDefaultModel);
public:
    QtTreeDefaultModelPrivate();
    ~QtTreeDefaultModelPrivate();

    QtTreeDefaultModel *q_ptr;
    QtTreeDefaultItem *root;
    int columnCount;
};

QtTreeDefaultModelPrivate::QtTreeDefaultModelPrivate()
    : q_ptr(0), root(new QtTreeDefaultItem), columnCount(1)
{
}

QtTreeDefaultModelPrivate::~QtTreeDefaultModelPrivate()
{
    delete root;
}
    
// QtTreeDefaultModel

/*!
  Constructs a tree item model with \a parent object.
*/
QtTreeDefaultModel::QtTreeDefaultModel(QObject *parent)
    : QtTreeModelInterface<QtTreeDefaultItem*>(parent), d_ptr(new QtTreeDefaultModelPrivate)
{
    Q_D(QtTreeDefaultModel);
    d->q_ptr = this;
    d->root->d_ptr->model = this;
}

/*!
  Destroys the tree item model.
*/
QtTreeDefaultModel::~QtTreeDefaultModel()
{
    delete d_ptr;
}

/*!
  Returns the iterator for the given \a item.
 */
QtTreeDefaultModel::iterator QtTreeDefaultModel::itemIterator(QtTreeDefaultItem *item)
{
    Q_D(const QtTreeDefaultModel);
    return QtTreeDefaultModel::iterator(item != d->root ? item : 0, this);
}

/*!
  Returns the item for the given \a iterator.
 */
QtTreeDefaultItem *QtTreeDefaultModel::item(iterator &iterator)
{
    return (*iterator);
}

/*!
  Returns the root item used by the model.
*/
QtTreeDefaultItem *QtTreeDefaultModel::rootItem() const
{
    Q_D(const QtTreeDefaultModel);
    return d->root;
}

/*!
  \internal
  \reimp
*/
QtTreeDefaultItem *QtTreeDefaultModel::firstChild(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    if (item && item != d->root) {
        const QList<QtTreeDefaultItem*> &children = item->children();
        return children.isEmpty() ? 0 : children.first();
    }    
    return d->root->firstChild();
}

/*!
  \internal
  \reimp
*/
QtTreeDefaultItem *QtTreeDefaultModel::nextSibling(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item != d->root ? item->next() : 0;
}

/*!
  \internal
  \reimp
*/
QtTreeDefaultItem *QtTreeDefaultModel::previousSibling(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item != d->root ? item->previous() : 0;
}

/*!
  \internal
  \reimp
*/
QtTreeDefaultItem *QtTreeDefaultModel::parentItem(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item->parent() != d->root ? item->parent() : 0;
}

/*!
  \internal
  \reimp
*/
QHash<QByteArray,QVariant> QtTreeDefaultModel::data(QtTreeDefaultItem *item, int column, const QList<QByteArray> &roles) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item != d->root ? item->data(column, roles) : QHash<QByteArray,QVariant>();
}

/*!
  \internal
  \reimp
*/
bool QtTreeDefaultModel::setData(const QVariant &data, QtTreeDefaultItem *item, int column, const QByteArray &role)
{
    Q_D(const QtTreeDefaultModel);
    if (item && item != d->root) {
        item->setData(data, column, role);
        return true;
    }
    return false;
}

/*!
  \internal
  \reimp
*/
bool QtTreeDefaultModel::isValid(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return (!!item) && (item != d->root);
}

/*!
  \internal
  \reimp
 */
int QtTreeDefaultModel::columnCount(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item != d->root ? item->columnCount() : d->columnCount;
}

/*!
  \internal
  \reimp
 */
bool QtTreeDefaultModel::hasChildren(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel); // ### what about the root ?
    return item && item != d->root ? !item->children().isEmpty() : false;
}

/*!
  \internal
  \reimp
 */
bool QtTreeDefaultModel::hasNextSibling(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item != d->root ? item->next() != 0 : false;
}

/*!
  \internal
  \reimp
 */
bool QtTreeDefaultModel::hasPreviousSibling(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item != d->root ? item->previous() != 0 : false;
}

/*!
  \internal
  \reimp
 */
bool QtTreeDefaultModel::hasParent(QtTreeDefaultItem *item) const
{
    Q_D(const QtTreeDefaultModel);
    return item && item->parent() != d->root ? !!item->parent() : false;
}

/*!
  \internal
  Report that an item has been inserted by emitting the itemsInserted signal.
 */
void QtTreeDefaultModel::itemInserted(QtTreeDefaultItem *item)
{
    QtTreeDefaultModel::iterator it(item, this);
    emit itemsInserted(it, 1);
}

/*!
  \internal
  Report that an item has been removed by emitting the itemsRemoved signal.
 */
void QtTreeDefaultModel::itemRemoved(QtTreeDefaultItem *item)
{
    QtTreeDefaultModel::iterator it(item, this);
    emit itemsRemoved(it, 1);
}

/*!
  \internal
  Report that an item has been changed by emitting the itemsChanged signal.
 */
void QtTreeDefaultModel::itemChanged(QtTreeDefaultItem *item, const QList<QByteArray> &roles)
{
    QtTreeDefaultModel::iterator it(item, this);
    emit itemsChanged(it, 1, roles);
}

QT_END_NAMESPACE

#include "moc_qtreedefaultmodel.cpp"
