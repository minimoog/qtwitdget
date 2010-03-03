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

#ifndef QTTABLEMODELADAPTOR_H
#define QTTABLEMODELADAPTOR_H

#include "qtablemodelinterface.h"
#include <QtCore/qabstractitemmodel.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtTableModelAdaptorPrivate;

class Q_ITEMVIEWSNG_EXPORT QtTableModelAdaptor : public QtTableModelInterface
{
    Q_OBJECT
public:
    explicit QtTableModelAdaptor(QAbstractItemModel *model = 0, QObject *parent = 0);
    ~QtTableModelAdaptor();

    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model() const;

    void setRootIndex(const QModelIndex &root);
    QModelIndex rootIndex() const;

    int rowCount() const;
    int columnCount() const;

    QHash<QByteArray,QVariant> data(int row, int column, const QList<QByteArray> &roles = QList<QByteArray>()) const;
    bool setData(int row, int column, const QHash<QByteArray,QVariant> &values);

protected:
    QtTableModelAdaptor(QtTableModelAdaptorPrivate &, QObject *parent = 0);
    QtTableModelAdaptorPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtTableModelAdaptor)
    Q_DISABLE_COPY(QtTableModelAdaptor)
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_dataChanged(const QModelIndex&,const QModelIndex&))
    Q_PRIVATE_SLOT(d_func(), void _q_headerDataChanged(Qt::Orientation,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsInserted(const QModelIndex&,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsRemoved(const QModelIndex&,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsInserted(const QModelIndex&,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsRemoved(const QModelIndex&,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_layoutChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_modelReset())
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTABLEMODELADAPTOR_H
