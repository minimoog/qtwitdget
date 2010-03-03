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

#ifndef QTLISTMODELADAPTOR_P_H
#define QTLISTMODELADAPTOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qabstractitemmodel.h>

QT_BEGIN_NAMESPACE

class QtListModelAdaptor;

class QtListModelAdaptorPrivate
{
    Q_DECLARE_PUBLIC(QtListModelAdaptor)
public:
    QtListModelAdaptorPrivate();
    virtual ~QtListModelAdaptorPrivate();

    void _q_modelDestroyed();
    void _q_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void _q_headerDataChanged(Qt::Orientation orientation, int first, int last);
    void _q_rowsInserted(const QModelIndex &parent, int first, int last);
    void _q_rowsRemoved(const QModelIndex &parent, int first, int last);
    void _q_columnsInserted(const QModelIndex &parent, int first, int last);
    void _q_columnsRemoved(const QModelIndex &parent, int first, int last);
    void _q_layoutChanged();
    void _q_modelReset();

    void hideContents();
    void showContents();

    QtListModelAdaptor *q_ptr;
    QAbstractItemModel *model;
    QPersistentModelIndex root;
    int column;
    bool empty;
};

QT_END_NAMESPACE

#endif//QTLISTMODELADAPTOR_P_H
