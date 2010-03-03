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

#include "qlistmodeladaptor.h"
#include "qlistmodeladaptor_p.h"
#include "qdataroles_p.h"

#include <qdebug.h>

QT_BEGIN_NAMESPACE

QtListModelAdaptorPrivate::QtListModelAdaptorPrivate()
    : q_ptr(0), model(0), column(0), empty(false)
{
}

QtListModelAdaptorPrivate::~QtListModelAdaptorPrivate()
{
}

void QtListModelAdaptorPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtListModelAdaptorPrivate::_q_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_Q(QtListModelAdaptor);
    // ### make sure that topLeft.parent() == d->root && bottomRight.parent() == d->root
    int index = topLeft.row();
    int count = bottomRight.row() - topLeft.row() + 1;
    emit q->itemsChanged(index, count, QList<QByteArray>());
}

void QtListModelAdaptorPrivate::_q_headerDataChanged(Qt::Orientation orientation, int first, int last)
{
    Q_UNUSED(orientation);
    Q_UNUSED(first);
    Q_UNUSED(last);
    qWarning() << "_q_headerDataChanged not implemented";
}

void QtListModelAdaptorPrivate::_q_rowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_Q(QtListModelAdaptor);
    if (parent != root) {
        qWarning() << "_q_rowsInserted: the parent was not the same as the root index";
        return;
    }
    int index = first;
    int count = last - first + 1;
    emit q->itemsInserted(index, count);
}

void QtListModelAdaptorPrivate::_q_rowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_Q(QtListModelAdaptor);
    if (parent != root) {
        qWarning() << "_q_rowsRemoved: the parent was not the same as the root index";
        return;
    }
    int index = first;
    int count = last - first + 1;
    emit q->itemsRemoved(index, count);
}

void QtListModelAdaptorPrivate::_q_columnsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    qWarning() << "_q_columnsInserted not implemented";
}

void QtListModelAdaptorPrivate::_q_columnsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    qWarning() << "_q_columnsRemoved not implemented";
}

void QtListModelAdaptorPrivate::_q_layoutChanged()
{
    // ### we may need to cache the number of items to handle this
    qWarning() << "_q_layoutChanged not implemented";
    hideContents();
    showContents();
}

void QtListModelAdaptorPrivate::_q_modelReset()
{
    // ### we may need to cache the number of items to handle this
    qWarning() << "_q_modelReset not implemented";
    hideContents();
    showContents();
}

void QtListModelAdaptorPrivate::hideContents()
{
    Q_Q(QtListModelAdaptor);
    empty = true;
    if (int removed = model ? model->rowCount(root) : 0)
        emit q->itemsRemoved(0, removed);
}

void QtListModelAdaptorPrivate::showContents()
{
    Q_Q(QtListModelAdaptor);
    empty = false;
    if (int inserted = model ? model->rowCount(root) : 0)
        emit q->itemsInserted(0, inserted);
}

/*!
  \class QtListModelAdaptor
  \brief A list model adaptor
*/

QtListModelAdaptor::QtListModelAdaptor(QAbstractItemModel *model, QObject *parent)
    : QtListModelInterface(parent), d_ptr(new QtListModelAdaptorPrivate)
{
    d_ptr->q_ptr = this;
    setModel(model);
}

QtListModelAdaptor::QtListModelAdaptor(QtListModelAdaptorPrivate &dd, QObject *parent)
    : QtListModelInterface(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

QtListModelAdaptor::~QtListModelAdaptor()
{
    delete d_ptr;
}

void QtListModelAdaptor::setModel(QAbstractItemModel *model)
{
    Q_D(QtListModelAdaptor);
    if (d->model) {
        QObject::disconnect(d->model, SIGNAL(destroyed()),
                            this, SLOT(_q_modelDestroyed()));
        QObject::disconnect(d->model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
                            this, SLOT(_q_dataChanged(const QModelIndex&, const QModelIndex&)));
        QObject::disconnect(d->model, SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
                            this, SLOT(_q_headerDataChanged(Qt::Orientation,int,int)));
        QObject::disconnect(d->model, SIGNAL(rowsInserted(const QModelIndex&,int,int)),
                            this, SLOT(_q_rowsInserted(const QModelIndex&,int,int)));
        QObject::disconnect(d->model, SIGNAL(rowsRemoved(const QModelIndex&,int,int)),
                            this, SLOT(_q_rowsRemoved(const QModelIndex&,int,int)));
        QObject::disconnect(d->model, SIGNAL(columnsInserted(const QModelIndex&,int,int)),
                            this, SLOT(_q_columnsInserted(const QModelIndex&,int,int)));
        QObject::disconnect(d->model, SIGNAL(columnsRemoved(const QModelIndex&,int,int)),
                            this, SLOT(_q_columnsRemoved(const QModelIndex&,int,int)));
    }
    d->hideContents();
    d->model = model;
    d->showContents();
    if (d->model) {
        QObject::connect(d->model, SIGNAL(destroyed()),
                         this, SLOT(_q_modelDestroyed()));
        QObject::connect(d->model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
                         this, SLOT(_q_dataChanged(const QModelIndex&, const QModelIndex&)));
        QObject::connect(d->model, SIGNAL(headerDataChanged(Qt::Orientation,int,int)),
                         this, SLOT(_q_headerDataChanged(Qt::Orientation,int,int)));
        QObject::connect(d->model, SIGNAL(rowsInserted(const QModelIndex&,int,int)),
                         this, SLOT(_q_rowsInserted(const QModelIndex&,int,int)));
        QObject::connect(d->model, SIGNAL(rowsRemoved(const QModelIndex&,int,int)),
                         this, SLOT(_q_rowsRemoved(const QModelIndex&,int,int)));
        QObject::connect(d->model, SIGNAL(columnsInserted(const QModelIndex&,int,int)),
                         this, SLOT(_q_columnsInserted(const QModelIndex&,int,int)));
        QObject::connect(d->model, SIGNAL(columnsRemoved(const QModelIndex&,int,int)),
                         this, SLOT(_q_columnsRemoved(const QModelIndex&,int,int)));
    }
}

QAbstractItemModel *QtListModelAdaptor::model() const
{
    Q_D(const QtListModelAdaptor);
    return d->model;
}

void QtListModelAdaptor::setRootIndex(const QModelIndex &root)
{
    Q_D(QtListModelAdaptor);
    d->hideContents();
    d->root = root;
    d->showContents();
}

QModelIndex QtListModelAdaptor::rootIndex() const
{
    Q_D(const QtListModelAdaptor);
    return d->root;
}

void QtListModelAdaptor::setColumn(int column)
{
    Q_D(QtListModelAdaptor);
    d->column = column;
}

int QtListModelAdaptor::column() const
{
    Q_D(const QtListModelAdaptor);
    return d->column;
}

int QtListModelAdaptor::count() const
{
    Q_D(const QtListModelAdaptor);
    return d->model && !d->empty ? d->model->rowCount(d->root) : 0;
}

QHash<QByteArray,QVariant> QtListModelAdaptor::data(int index, const QList<QByteArray> &roles) const
{
    Q_D(const QtListModelAdaptor);
    QHash<QByteArray,QVariant> values;
    if (d->model) {
        const QModelIndex idx = d->model->index(index, d->column, d->root);
        for (int i = 0; i < roles.count(); ++i)
            values.insert(roles.at(i), d->model->data(idx, QtDataRoles::value(roles.at(i))));
    }
    return values;
}

bool QtListModelAdaptor::setData(int index, const QHash<QByteArray,QVariant> &values)
{
    Q_D(QtListModelAdaptor);
    QHash<QByteArray,QVariant>::const_iterator it = values.begin();
    for (; it != values.end(); ++it) {
        const QModelIndex idx = d->model->index(index, d->column, d->root);
        int role = QtDataRoles::value(it.key());
        if (!d->model->setData(idx, it.value(), role))
            return false;
    }
    return true;
}

QT_END_NAMESPACE

#include "moc_qlistmodeladaptor.cpp"
