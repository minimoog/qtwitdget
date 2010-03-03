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

#include "qtablemodelinterface.h"
#include "qdataroles_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QtTableModelInterface
    \brief api docs go here
*/

QtTableModelInterface::QtTableModelInterface(QObject *parent)
    : QObject(parent)
{
}

QtTableModelInterface::QtTableModelInterface(QObjectPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

QtTableModelInterface::~QtTableModelInterface()
{
}

/*!
    Sets the data for the cell at \a row and \a column to the given \a values.
    Returns true if the data was set on the cell; returns false otherwise.

    The default implementation does not set the data, and will always return
    false.
*/
bool QtTableModelInterface::setData(int row, int column, const QHash<QByteArray,QVariant> &values)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(values);
    return false;
}

QT_END_NAMESPACE
