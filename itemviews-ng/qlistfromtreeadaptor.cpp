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

#include "qlistfromtreeadaptor.h"
#include "qlistfromtreeadaptor_p.h"
#include "qtreemodelbase.h"
#include "qdataroles_p.h"

QT_BEGIN_NAMESPACE

/*!
  \internal
 */
QtListFromTreeAdaptorPrivate::QtListFromTreeAdaptorPrivate()
    : q_ptr(0), model(0), column(0)
{
}

/*!
  \internal
 */
QtListFromTreeAdaptorPrivate::~QtListFromTreeAdaptorPrivate()
{
}

/*!
  \internal
 */
void QtListFromTreeAdaptorPrivate::_q_modelDestroyed()
{
    model = 0;
}

/*!
  \internal
  \see QtListModelInterface::itemsInserted()
 */
void QtListFromTreeAdaptorPrivate::_q_itemsInserted(const QtTreeModelBase::iterator_base &it, int count)
{
    Q_Q(QtListFromTreeAdaptor);

    QtTreeModelBase::iterator_base _it = it;
    int index = 0;

    if (this->it == _it.parent()) {
        for (_it = it; _it.hasPreviousSibling(); _it.previousSibling()) {
            index++;
        }

        emit q->itemsInserted(index, count);
    }
}

/*!
  \internal
  \see QtListModelInterface::itemsRemoved()
 */
void QtListFromTreeAdaptorPrivate::_q_itemsRemoved(const QtTreeModelBase::iterator_base &it, int count)
{
    Q_Q(QtListFromTreeAdaptor);

    QtTreeModelBase::iterator_base _it = it;
    int index = 0;

    if (this->it == _it.parent()) {
        for (_it = it; _it.hasPreviousSibling(); _it.previousSibling()) {
            index++;
        }

        emit q->itemsRemoved(index, count);
    }
}

/*!
  \internal
  \see QtListModelInterface::itemsChanged()
 */
void QtListFromTreeAdaptorPrivate::_q_itemsChanged(const QtTreeModelBase::iterator_base &it, int count,
                                                    const QList<QByteArray> &roles)
{
    Q_Q(QtListFromTreeAdaptor);

    QtTreeModelBase::iterator_base _it = it;
    int index = 0;

    if (this->it == _it.parent()) {
        for (_it = it; _it.hasPreviousSibling(); _it.previousSibling()) {
            index++;
        }

        emit q->itemsChanged(index, count, roles);
    }
}

/*!
  \internal

  Returns the iterator from an individual list item based on the given index.
 */
QtTreeModelBase::iterator_base QtListFromTreeAdaptorPrivate::iteratorAt(int index) const
{
    Q_Q(const QtListFromTreeAdaptor);

    QtTreeModelBase::iterator_base _it = it;

    if (q->count() < index)
        return QtTreeModelBase::iterator_base();

    _it.firstChild();
    while (index != 0) {
        _it.nextSibling();
        index--;
    }

    return _it;
}

/*!
  \class QtListFromTreeAdaptor
  \brief Provides an adaptor from children items of a tree model parent item to a list model.

  QtListFromTreeAdaptor implements a model adaptor which translates a group of children items
  (given a parent item iterator) from a tree model to a list model, where each list item is
  equivalent to a child from the given parent item.

  The index of each individual list items are based on the hierarchy order of the original tree
  model (i.e. firstChild() has index 0, and subsequent siblings have increasing indexes).

  The QtListFromTreeAdaptor constructor receives a QtTreeModelBase (or derived) model, the parent
  item iterator from that model, the default tree item data column to be used and the parent
  QObject object (optional).

  \sa QtListModelInterface, QtTreeModelInterface
*/

QtListFromTreeAdaptor::QtListFromTreeAdaptor(QtTreeModelBase *model,
                                           const QtTreeModelBase::iterator_base &it,
                                           int column, QObject *parent)
        : QtListModelInterface(parent), d_ptr(new QtListFromTreeAdaptorPrivate)
{
    d_ptr->q_ptr = this;
    setModel(model);
    setParentItem(it);
    setColumn(column);
}

/*!
  \internal
 */
QtListFromTreeAdaptor::QtListFromTreeAdaptor(QtListFromTreeAdaptorPrivate &dd, QObject *parent)
        : QtListModelInterface(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtListFromTreeAdaptor::~QtListFromTreeAdaptor()
{
    delete d_ptr;
}

/*!
  Sets the QtTreeModelBase source model.
 */
void QtListFromTreeAdaptor::setModel(QtTreeModelBase *model)
{
    Q_D(QtListFromTreeAdaptor);
    if (d->model) {
        QObject::disconnect(d->model, SIGNAL(destroyed()),
                            this, SLOT(_q_modelDestroyed()));
        QObject::disconnect(d->model, SIGNAL(itemsInserted(const QtTreeModelBase::iterator_base&,int)),
                            this, SLOT(_q_itemsInserted(const QtTreeModelBase::iterator_base&,int)));
        QObject::disconnect(d->model, SIGNAL(itemsRemoved(const QtTreeModelBase::iterator_base&,int)),
                            this, SLOT(_q_itemsRemoved(const QtTreeModelBase::iterator_base&,int)));
        QObject::disconnect(d->model, SIGNAL(itemsChanged(const QtTreeModelBase::iterator_base&,int,QList<QByteArray>)),
                            this, SLOT(_q_itemsChanged(const QtTreeModelBase::iterator_base&,int,QList<QByteArray>)));
    }
    d->model = model;
    if (d->model) {
        QObject::connect(d->model, SIGNAL(destroyed()),
                         this, SLOT(_q_modelDestroyed()));
        QObject::connect(d->model, SIGNAL(itemsInserted(const QtTreeModelBase::iterator_base&,int)),
                         this, SLOT(_q_itemsInserted(const QtTreeModelBase::iterator_base&,int)));
        QObject::connect(d->model, SIGNAL(itemsRemoved(const QtTreeModelBase::iterator_base&,int)),
                         this, SLOT(_q_itemsRemoved(const QtTreeModelBase::iterator_base&,int)));
        QObject::connect(d->model, SIGNAL(itemsChanged(const QtTreeModelBase::iterator_base&,int,QList<QByteArray>)),
                         this, SLOT(_q_itemsChanged(const QtTreeModelBase::iterator_base&,int,QList<QByteArray>)));
    }
}

/*!
  \brief Returns the QtTreeModelBase source model.

  The QtTreeModelBase model is used by the adaptor as the source of list items data and
  signals. The adaptor captures the signals generated by the source model and translates them to
  similar ones implemented by the QtListModelInterface.
 */
QtTreeModelBase *QtListFromTreeAdaptor::model() const
{
    Q_D(const QtListFromTreeAdaptor);
    return d->model;
}

/*!
  Sets the tree model parent item iterator.
 */
void QtListFromTreeAdaptor::setParentItem(const QtTreeModelBase::iterator_base &it)
{
    Q_D(QtListFromTreeAdaptor);
    d->it = it;
}

/*!
  \brief Returns the tree model parent item iterator.

  The parent item iterator is used to traverse through its childs without the need to know about
  their implementation.
 */
QtTreeModelBase::iterator_base QtListFromTreeAdaptor::parentItem() const
{
    Q_D(const QtListFromTreeAdaptor);
    return d->it;
}

/*!
  Sets the default tree item data column.
 */
void QtListFromTreeAdaptor::setColumn(int column)
{
    Q_D(QtListFromTreeAdaptor);
    d->column = column;
}

/*!
  \brief Returns the default tree item data column.

  The default tree item data column is used by the adaptor list items to get and set data.
 */
int QtListFromTreeAdaptor::column() const
{
    Q_D(const QtListFromTreeAdaptor);
    return d->column;
}

/*!
  \see QtListModelInterface::count()
 */
int QtListFromTreeAdaptor::count() const
{
    Q_D(const QtListFromTreeAdaptor);

    QtTreeModelBase::iterator_base it = d->it;
    int count = 0;

    it.firstChild();

    if (it.isValid())
        count++;

    while (it.hasNextSibling()) {
        it.nextSibling();
        count++;
    }

    return count;
}

/*!
  \see QtListModelInterface::data()
 */
QHash<QByteArray,QVariant> QtListFromTreeAdaptor::data(int index, const QList<QByteArray> &roles) const
{
    Q_D(const QtListFromTreeAdaptor);

    if (index < 0 || index >= count())
        return QHash<QByteArray, QVariant>();

    return d->iteratorAt(index).data(d->column, roles);
}

/*!
  \see QtListModelInterface::setData()
 */
bool QtListFromTreeAdaptor::setData(int index, const QHash<QByteArray,QVariant> &values)
{
    Q_D(const QtListFromTreeAdaptor);

    QtTreeModelBase::iterator_base it = d->iteratorAt(index);

    if (index < 0 || index >= count())
        return false;

    if (it.isValid()) {
        QHash<QByteArray,QVariant> oldValues = it.data(d->column);
        it.setData(d->column, oldValues.unite(values));
    }

    emit itemsChanged(index, 1, values.keys());
    return true;
}

QT_END_NAMESPACE

#include "moc_qlistfromtreeadaptor.cpp"
