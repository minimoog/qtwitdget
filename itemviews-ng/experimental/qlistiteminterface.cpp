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

#include "qlistiteminterface.h"
#include "../qlistmodelinterface.h"

class QtListItemInterfacePrivate
{
    Q_DECLARE_PUBLIC(QtListItemInterface)
public:
    QtListItemInterfacePrivate() : q_ptr(0), current(-1), model(0) {}
    ~QtListItemInterfacePrivate() {}

    void _q_modelDestroyed();
    void _q_itemsInserted(int index, int count);
    void _q_itemsRemoved(int index, int count);
    void _q_itemsMoved(int from, int to, int count);
    void _q_itemsChanged(int index, int count, const QList<int> &roles);

    QtListItemInterface *q_ptr;
    int current;
    QtListModelInterface *model;
};

void QtListItemInterfacePrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtListItemInterfacePrivate::_q_itemsInserted(int index, int count)
{
    if (current >= index) {
        if (current > index + count) {
            index += count;
        } else {
            index = -1;
        }
    }
}

void QtListItemInterfacePrivate::_q_itemsRemoved(int index, int count)
{
    if (current >= index) {
        if (current > index + count) {
            index -= count;
        } else {
            index = -1;
        }
    }
}

void QtListItemInterfacePrivate::_q_itemsMoved(int from, int to, int count)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    Q_UNUSED(count);
/*
    if (current >= from && current < from + count)
        current = to + (current - from);
    else if (current > from && current < to)
        index -= count;
    else if (current < from && current > to)
        index += count;
*/
}

void QtListItemInterfacePrivate::_q_itemsChanged(int index, int count, const QList<int> &roles)
{
    Q_Q(QtListItemInterface);
    static const QList<int> textRole = QList<int>() << Qt::DisplayRole;
    static const QList<int> iconRole = QList<int>() << Qt::DecorationRole;
    if (current >= index && current < index + count) {
        for (int i = 0; i < roles.count(); ++i) {
            switch (roles.at(i)) {
            case Qt::DisplayRole:
                emit q->textChanged(q->text());
            case Qt::DecorationRole:
                emit q->iconChanged(q->icon());
            default:
                break;
            }
        }
    }
}

/*!
  \class QtListItemInterface
  \brief The QtListItemInterface is there to give a simple and quickly usable data model for a QListViewNG.

*/

/*!
  Constructs a QtListItemInterface

  The \a model is used as a parent to the QObject for memory management purposes
 */
QtListItemInterface::QtListItemInterface(QtListModelInterface *model)
    : QObject(*new QtListItemInterfacePrivate, model)
{
    Q_D(QtListItemInterface);
    d->model = model;
}

/*!
  Constructs a QtListItemInterface

  The \a model is used as a parent to the QObject for memory management purposes
 */
QtListItemInterface::QtListItemInterface(int index, QtListModelInterface *model)
    : QObject(model), d_ptr(new QtListItemInterfacePrivate)
{
    Q_D(QtListItemInterface);
    d->q_ptr = this;
    d->current = index;
    d->model = model;
}

/*!
  Destructor
 */
QtListItemInterface::~QtListItemInterface()
{
    delete d_ptr;
}

/*!
  \property index
 */
void QtListItemInterface::setIndex(int index)
{
    Q_D(QtListItemInterface);
    d->current = index;
}

int QtListItemInterface::index() const
{
    Q_D(const QtListItemInterface);
    return d->current;
}

/*!
  \property text
 */
void QtListItemInterface::setText(const QString &text)
{
    Q_D(QtListItemInterface);
    d->model->setSingleData(d->current, text, Qt::DisplayRole);
}

QString QtListItemInterface::text() const
{
    Q_D(const QtListItemInterface);
    static const QList<int> roles = QList<int>() << Qt::DisplayRole;
    return d->model->data(d->current, roles).value(Qt::DisplayRole).toString();
}

/*!
  \property icon
 */
void QtListItemInterface::setIcon(const QIcon &icon)
{
    Q_D(QtListItemInterface);
    d->model->setSingleData(d->current, icon, Qt::DecorationRole);
}

QIcon QtListItemInterface::icon() const
{
    Q_D(const QtListItemInterface);
    static const QList<int> roles = QList<int>() << Qt::DecorationRole;
    return qvariant_cast<QIcon>(d->model->data(d->current, roles).value(Qt::DecorationRole));
}

#include "moc_qlistiteminterface.cpp"
