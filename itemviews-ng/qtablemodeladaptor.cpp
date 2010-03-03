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

#include "qtablemodeladaptor.h"
#include "qtablemodeladaptor_p.h"
#include "qdataroles_p.h"

#include <qdebug.h>

QT_BEGIN_NAMESPACE

QtTableModelAdaptorPrivate::QtTableModelAdaptorPrivate()
    : q_ptr(0), model(0)
{
}

QtTableModelAdaptorPrivate::~QtTableModelAdaptorPrivate()
{
}

void QtTableModelAdaptorPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtTableModelAdaptorPrivate::_q_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    Q_Q(QtTableModelAdaptor);
    // ### make sure that topLeft.parent() == d->root && bottomRight.parent() == d->root
    int row = topLeft.row();
    int column = topLeft.column();
    int rowCount = bottomRight.row() - topLeft.row() + 1;
    int columnCount = bottomRight.column() - topLeft.column() + 1;
    emit q->cellsChanged(row, column, rowCount, columnCount, QList<QByteArray>());
}

void QtTableModelAdaptorPrivate::_q_headerDataChanged(Qt::Orientation orientation, int first, int last)
{
    Q_UNUSED(orientation);
    Q_UNUSED(first);
    Q_UNUSED(last);
    qWarning() << "_q_headerDataChanged not implemented";
}

void QtTableModelAdaptorPrivate::_q_rowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_Q(QtTableModelAdaptor);
    if (parent != root) {
        qWarning() << "_q_rowsInserted: the parent was not the same as the root index";
        return;
    }
    int row = first;
    int count = last - first + 1;
    emit q->rowsInserted(row, count);
}

void QtTableModelAdaptorPrivate::_q_rowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_Q(QtTableModelAdaptor);
    if (parent != root) {
        qWarning() << "_q_rowsRemoved: the parent was not the same as the root index";
        return;
    }
    int row = first;
    int count = last - first + 1;
    emit q->rowsRemoved(row, count);
}

void QtTableModelAdaptorPrivate::_q_columnsInserted(const QModelIndex &parent, int first, int last)
{
    Q_Q(QtTableModelAdaptor);
    if (parent != root) {
        qWarning() << "_q_columnsInserted: the parent was not the same as the root index";
        return;
    }
    int column = first;
    int count = last - first + 1;
    emit q->columnsInserted(column, count);
}

void QtTableModelAdaptorPrivate::_q_columnsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_Q(QtTableModelAdaptor);
    if (parent != root) {
        qWarning() << "_q_columnsRemoved: the parent was not the same as the root index";
        return;
    }
    int column = first;
    int count = last - first + 1;
    emit q->columnsRemoved(column, count);
}

void QtTableModelAdaptorPrivate::_q_layoutChanged()
{
    // ### we may need to cache the number of rows and columns to handle this
    qWarning() << "_q_layoutChanged not implemented";
}

void QtTableModelAdaptorPrivate::_q_modelReset()
{
    // ### we may need to cache the number of rows and columns to handle this
    qWarning() << "_q_modelReset not implemented";
}

/*!
  \class QtTableModelAdaptor
  \brief A table model adaptor
*/

QtTableModelAdaptor::QtTableModelAdaptor(QAbstractItemModel *model, QObject *parent)
    : QtTableModelInterface(parent), d_ptr(new QtTableModelAdaptorPrivate)
{
    d_ptr->q_ptr = this;
    setModel(model);
}

QtTableModelAdaptor::QtTableModelAdaptor(QtTableModelAdaptorPrivate &dd, QObject *parent)
    : QtTableModelInterface(parent), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

QtTableModelAdaptor::~QtTableModelAdaptor()
{
    delete d_ptr;
}

void QtTableModelAdaptor::setModel(QAbstractItemModel *model)
{
    Q_D(QtTableModelAdaptor);
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
    d->model = model;
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

QAbstractItemModel *QtTableModelAdaptor::model() const
{
    Q_D(const QtTableModelAdaptor);
    return d->model;
}

void QtTableModelAdaptor::setRootIndex(const QModelIndex &root)
{
    Q_D(QtTableModelAdaptor);
    d->root = root;
    // emit signals
}

QModelIndex QtTableModelAdaptor::rootIndex() const
{
    Q_D(const QtTableModelAdaptor);
    return d->root;
}

int QtTableModelAdaptor::rowCount() const
{
    Q_D(const QtTableModelAdaptor);
    return d->model ? d->model->rowCount(d->root) : 0;
}

int QtTableModelAdaptor::columnCount() const
{
    Q_D(const QtTableModelAdaptor);
    return d->model ? d->model->columnCount(d->root) : 0;
}

QHash<QByteArray,QVariant> QtTableModelAdaptor::data(int row, int column, const QList<QByteArray> &roles) const
{
    Q_D(const QtTableModelAdaptor);
    QHash<QByteArray,QVariant> values;
    if (d->model) {
        const QModelIndex idx = d->model->index(row, column, d->root);
        for (int i = 0; i < roles.count(); ++i) {
            int role = QtDataRoles::value(roles.at(i));
            values.insert(roles.at(i), d->model->data(idx, role));
        }
    }
    return values;
}

bool QtTableModelAdaptor::setData(int row, int column, const QHash<QByteArray,QVariant> &values)
{
    Q_D(QtTableModelAdaptor);
    QHash<QByteArray,QVariant>::const_iterator it = values.begin();
    for (; it != values.end(); ++it) {
        int role = QtDataRoles::value(it.key());
        if (!d->model->setData(d->model->index(row, column, d->root), it.value(), role))
            return false;
    }
    return true;
}

QT_END_NAMESPACE

#include "moc_qtablemodeladaptor.cpp"
