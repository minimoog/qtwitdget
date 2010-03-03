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

#ifndef QTLISTMODELADAPTOR_H
#define QTLISTMODELADAPTOR_H

#include "qlistmodelinterface.h"
#include <QtCore/qabstractitemmodel.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtListModelAdaptorPrivate;

class Q_ITEMVIEWSNG_EXPORT QtListModelAdaptor : public QtListModelInterface
{
    Q_OBJECT
public:
    explicit QtListModelAdaptor(QAbstractItemModel *model = 0, QObject *parent = 0);
    virtual ~QtListModelAdaptor();

    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model() const;

    void setRootIndex(const QModelIndex &root);
    QModelIndex rootIndex() const;

    void setColumn(int column);
    int column() const;

    int count() const;
    QHash<QByteArray,QVariant> data(int index, const QList<QByteArray> &roles) const;
    bool setData(int index, const QHash<QByteArray,QVariant> &values);

protected:
    QtListModelAdaptor(QtListModelAdaptorPrivate &, QObject *parent = 0);
    QtListModelAdaptorPrivate *d_ptr;
    
private:
    Q_DECLARE_PRIVATE(QtListModelAdaptor)
    Q_DISABLE_COPY(QtListModelAdaptor)
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

#endif//QTLISTMODELADAPTOR_H
