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

#include "qtreemodeladaptor.h"
#include "qtreemodeladaptor_p.h"
#include "qdataroles_p.h"

#include <qdebug.h>

QT_BEGIN_NAMESPACE

QtTreeModelAdaptorPrivate::QtTreeModelAdaptorPrivate()
    : q_ptr(0), model(0)
{
}

QtTreeModelAdaptorPrivate::~QtTreeModelAdaptorPrivate()
{
}

void QtTreeModelAdaptorPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtTreeModelAdaptorPrivate::_q_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_Q(QtTreeModelAdaptor);
    QtTreeModelAdaptor::iterator it(QPersistentModelIndex(topLeft), q);
    int count = bottomRight.row() - topLeft.row();
    emit q->itemsChanged(it, count, QList<QByteArray>());
}

void QtTreeModelAdaptorPrivate::_q_headerDataChanged(Qt::Orientation orientation, int first, int last)
{
    Q_UNUSED(orientation);
    Q_UNUSED(first);
    Q_UNUSED(last);
    qWarning() << "_q_headerDataChanged not implemented";
}

void QtTreeModelAdaptorPrivate::_q_rowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_Q(QtTreeModelAdaptor);
    QPersistentModelIndex index = model ? model->index(first, 0, parent) : QModelIndex();
    QtTreeModelAdaptor::iterator it(index, q);
    int count = last - first + 1;
    emit q->itemsInserted(it, count);
}

void QtTreeModelAdaptorPrivate::_q_rowsRemoved(const QModelIndex &parent, int first, int last)
{
    // ### this will not work, since the item has already been removed
    Q_Q(QtTreeModelAdaptor);
    QPersistentModelIndex index = model ? model->index(first, 0, parent) : QModelIndex();
    QtTreeModelAdaptor::iterator it(index, q);
    int count = last - first + 1;
    emit q->itemsRemoved(it, count);
}

void QtTreeModelAdaptorPrivate::_q_columnsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    qWarning() << "_q_columnsInserted not implemented";
}

void QtTreeModelAdaptorPrivate::_q_columnsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    qWarning() << "_q_columnsRemoved not implemented";
}

void QtTreeModelAdaptorPrivate::_q_layoutChanged()
{
    // ### we may need to cache the number of items to handle this
    qWarning() << "_q_layoutChanged not implemented";
}

void QtTreeModelAdaptorPrivate::_q_modelReset()
{
    // ### we may need to cache the number of items to handle this
    qWarning() << "_q_modelReset not implemented";
}

// QtTreeModelAdaptor

/*!
  \class QtTreeModelAdaptor
  \brief A tree model adaptor
*/

QtTreeModelAdaptor::QtTreeModelAdaptor(QObject *parent)
    : QtTreeModelInterface<QPersistentModelIndex>(parent), d_ptr(new QtTreeModelAdaptorPrivate())
{
    d_ptr->q_ptr = this;
}

QtTreeModelAdaptor::QtTreeModelAdaptor(QAbstractItemModel *model, QObject *parent)
    : QtTreeModelInterface<QPersistentModelIndex>(parent), d_ptr(new QtTreeModelAdaptorPrivate())
{
    Q_D(QtTreeModelAdaptor);
    d->q_ptr = this;
    d->model = model;
}

QtTreeModelAdaptor::QtTreeModelAdaptor(QtTreeModelAdaptorPrivate &dd, QObject *parent)
    : QtTreeModelInterface<QPersistentModelIndex>(parent), d_ptr(&dd)
{
}

QtTreeModelAdaptor::~QtTreeModelAdaptor()
{
    delete d_ptr;
}

void QtTreeModelAdaptor::setModel(QAbstractItemModel *model)
{
    Q_D(QtTreeModelAdaptor);
    Q_ASSERT(model);
    if (d->model != model) {
        disconnect(d->model, SIGNAL(destroyed()),
                this, SLOT(_q_modelDestroyed()));
        disconnect(d->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(_q_dataChanged(QModelIndex,QModelIndex)));
        disconnect(d->model, SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
                this, SLOT(_q_headerDataChanged(Qt::Orientation,int,int)));
        disconnect(d->model, SIGNAL(rowsInserted(QModelIndex,int,int)),
                this, SLOT(_q_rowsInserted(QModelIndex,int,int)));
        disconnect(d->model, SIGNAL(columnsInserted(QModelIndex,int,int)),
                this, SLOT(_q_columnsInserted(QModelIndex,int,int)));
        disconnect(d->model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
                this, SLOT(_q_rowsRemoved(QModelIndex,int,int)));
        disconnect(d->model, SIGNAL(columnsRemoved(QModelIndex,int,int)),
                this, SLOT(_q_columnsRemoved(QModelIndex,int,int)));
        disconnect(d->model, SIGNAL(layoutChanged()),
                this, SLOT(_q_layoutChanged()));
        disconnect(d->model, SIGNAL(modelReset()),
                this, SLOT(_q_sourceReset()));
        d->model = model;
        connect(d->model, SIGNAL(destroyed()),
                this, SLOT(_q_modelDestroyed()));
        connect(d->model, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                this, SLOT(_q_dataChanged(QModelIndex,QModelIndex)));
        connect(d->model, SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
                this, SLOT(_q_headerDataChanged(Qt::Orientation,int,int)));
        connect(d->model, SIGNAL(rowsInserted(QModelIndex,int,int)),
                this, SLOT(_q_rowsInserted(QModelIndex,int,int)));
        connect(d->model, SIGNAL(columnsInserted(QModelIndex,int,int)),
                this, SLOT(_q_columnsInserted(QModelIndex,int,int)));
        connect(d->model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
                this, SLOT(_q_rowsRemoved(QModelIndex,int,int)));
        connect(d->model, SIGNAL(columnsRemoved(QModelIndex,int,int)),
                this, SLOT(_q_columnsRemoved(QModelIndex,int,int)));
        connect(d->model, SIGNAL(layoutChanged()),
                this, SLOT(_q_layoutChanged()));
        connect(d->model, SIGNAL(modelReset()),
                this, SLOT(_q_sourceReset()));
    }
}

QAbstractItemModel *QtTreeModelAdaptor::model() const
{
    Q_D(const QtTreeModelAdaptor);
    return d->model;
}

QPersistentModelIndex QtTreeModelAdaptor::firstChild(QPersistentModelIndex t) const
{
    Q_D(const QtTreeModelAdaptor);
    if (d->model) {
        if (d->model->canFetchMore(t))
            d->model->fetchMore(t);
        return d->model->index(0, 0, t);
    }
    return t.child(0, 0);
}

QPersistentModelIndex QtTreeModelAdaptor::nextSibling(QPersistentModelIndex t) const
{
    return t.sibling(t.row() + 1, 0);
}

QPersistentModelIndex QtTreeModelAdaptor::parentItem(QPersistentModelIndex t) const
{
    return t.parent();
}

QPersistentModelIndex QtTreeModelAdaptor::previousSibling(QPersistentModelIndex t) const
{
    return t.sibling(t.row() - 1, 0);
}

QHash<QByteArray,QVariant> QtTreeModelAdaptor::data(QPersistentModelIndex t, int column, const QList<QByteArray> &roles) const
{
    Q_D(const QtTreeModelAdaptor);
    QHash<QByteArray,QVariant> values;
    if (d->model) {
        QModelIndex index = t.sibling(t.row(), column);
        for (int i = 0; i < roles.count(); ++i) {
            int role = QtDataRoles::value(roles.at(i));
            values.insert(roles.at(i), d->model->data(index, role));
        }
    }
    return values;
}

bool QtTreeModelAdaptor::setData(QPersistentModelIndex t, int column, const QHash<QByteArray,QVariant> &values)
{
    Q_D(QtTreeModelAdaptor);
    if (!d->model)
        return false;
    QHash<QByteArray,QVariant>::const_iterator it = values.begin();
    for (; it != values.end(); ++it) {
        int role = QtDataRoles::value(it.key());
        if (!d->model->setData(t.sibling(t.row(), column), it.value(), role))
            return false;
    }
    return true;
}

bool QtTreeModelAdaptor::hasChildren(QPersistentModelIndex t) const
{
    Q_D(const QtTreeModelAdaptor);
    return d->model ? d->model->hasChildren(t) : false;
}

bool QtTreeModelAdaptor::isValid(QPersistentModelIndex t) const
{
    return t.isValid();
}

int QtTreeModelAdaptor::columnCount(QPersistentModelIndex t) const
{
    Q_D(const QtTreeModelAdaptor);
    return d->model ? d->model->columnCount(t.parent()) : 1;
}

QT_END_NAMESPACE

#include "moc_qtreemodeladaptor.cpp"
