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

#ifndef QTTABLEMODELINTERFACE_H
#define QTTABLEMODELINTERFACE_H

#include "qitemviewsglobal.h"

#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class Q_ITEMVIEWSNG_EXPORT QtTableModelInterface : public QObject
{
    Q_OBJECT
public:
    explicit QtTableModelInterface(QObject *parent = 0);
    virtual ~QtTableModelInterface();

    virtual int rowCount() const = 0;
    virtual int columnCount() const = 0;
    virtual QHash<QByteArray,QVariant> data(int row, int column, const QList<QByteArray> &roles = QList<QByteArray>()) const = 0;
    virtual bool setData(int row, int column, const QHash<QByteArray,QVariant> &values);

    // ### we may need something along the lines of cacheHint() that gives the model an idea
    // ### of what can be kept, what can be thrown away and what needs to be fetched

Q_SIGNALS:
    void cellsChanged(int firstRow, int firstColumn, int rowCount, int columnCount, const QList<QByteArray> &roles);
    void rowsInserted(int row, int count);
    void rowsRemoved(int row, int count);
    void rowsMoved(int from, int to, int count);
    void columnsInserted(int column, int count);
    void columnsRemoved(int column, int count);
    void columnsMoved(int from, int to, int count);

protected:
    QtTableModelInterface(QObjectPrivate &dd, QObject *parent = 0);
};

QT_END_NAMESPACE

QT_END_HEADER

#endif //QTTABLEMODELINTERFACE_H
