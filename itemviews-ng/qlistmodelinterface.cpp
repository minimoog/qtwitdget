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

#include "qlistmodelinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QtListModelInterface
    \brief provides the abstract interface for list models.

    \omit
    A QtListWidgetNG is backed by four classes, a list controller
    (QtListController) and a model. While QtListController is responsible for
    handling all user input and model changes, QtListModelInterface is
    responsible for displaying items on the list.
    \endomit

    QtListModelInterface provides an abstract interface for list model classes.

    Each individual list item is referenced using an index, in the form of an
    integer. Each item has multiple data roles, as specified by
    Qt::DataRoles). Data can be requested using the data() method.

    \sa QtListController, QtListDefaultModel
*/

/*!
 */
QtListModelInterface::QtListModelInterface(QObject *parent)
    : QObject(parent)
{
}

/*!
  \internal
 */
QtListModelInterface::QtListModelInterface(QObjectPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

/*!
 */
QtListModelInterface::~QtListModelInterface()
{
}

/*!
    Sets the data for the item at \a index to the given \a values. Returns true
    if the data was set on the item; returns false otherwise.

    The default implementation does not set the data, and will always return
    false.
*/
bool QtListModelInterface::setData(int index, const QHash<QByteArray,QVariant> &values)
{
    Q_UNUSED(index);
    Q_UNUSED(values);
    return false;
}

/*!
    \fn QHash<int,QVariant> data(int index, const QList<int> &roles = (QList<int>())) const

    Returns the data for a given \a index, according to the \a roles.
*/

/*!
    \fn int count() const

    Returns the number of items in the list.
*/

/*!
    \fn void itemsInserted(int index, int count)

    This signal is emitted when \a count number of items were inserted in the
    model beginning with \a index.
*/

/*!
    \fn void itemsRemoved(int index, int count)

    This signal is emitted when \a count number of items were removed from the
    model beginning with \a index.
*/

/*!
    \fn void itemsMoved(int a, int b, int count)

    This signal is emitted when \a count number of items were moved in the
    model from \a a to \a b.
*/

/*!
    \fn void itemsChanged(int index, int count, const QList<int> &roles)

    This signal is emitted when the data for \a roles in \a count number of
    items were changed in the model beginning with \a index.
*/

#include "moc_qlistmodelinterface.cpp"

QT_END_NAMESPACE
