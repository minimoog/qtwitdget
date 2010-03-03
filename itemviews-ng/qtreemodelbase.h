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

#ifndef QTTREEMODELBASE_H
#define QTTREEMODELBASE_H

#include "qitemviewsglobal.h"

#include <QtCore/qhash.h>
#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qatomic.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class Q_ITEMVIEWSNG_EXPORT QtTreeModelBase : public QObject
{
    Q_OBJECT

public:
    class navigator_base
    {
    protected:
        navigator_base() {}
    public:
        virtual ~navigator_base() {}
        virtual void firstChild() = 0;
        virtual void nextSibling() = 0;
        virtual void parent() = 0;
        virtual void previousSibling() = 0;
        virtual bool operator==(const navigator_base &other) const = 0;
        virtual QHash<QByteArray,QVariant> data(int column, const QList<QByteArray> &roles) const = 0;
        virtual bool setData(int column, const QHash<QByteArray,QVariant> &values)  = 0;
        virtual bool isValid() const = 0;
        virtual int columnCount() const = 0;
        virtual bool hasChildren() const = 0;
        virtual bool hasNextSibling() const = 0;
        virtual bool hasParent() const = 0;
        virtual bool hasPreviousSibling() const = 0;
        virtual uint hash() const = 0;
        virtual navigator_base *detach_helper() = 0;
        virtual QtTreeModelBase *model() = 0;
        QAtomicInt ref;
    }; 

    class iterator_base // wraps the navigator class
    {
    public:
        inline iterator_base() : n(0) {} // invalid

        ~iterator_base()
        {
            if (n && !n->ref.deref())
                delete n;
        }

        iterator_base(const iterator_base &other)
        {
            n = other.n;
            if (n)
                n->ref.ref();
        }

        iterator_base &operator=(const iterator_base &other)
        {
            if (n == other.n)
                return *this;
            if (n && !n->ref.deref())
                delete n;
            n = other.n;
            if (n)
                n->ref.ref();
            return *this;
        }        

        void detach()
        {
            if (n && n->ref != 1) {
                n->ref.deref();
                n = n->detach_helper();
                n->ref.ref();
            }
        }

        inline bool isDetached() const { return (n ? n->ref == 1 : true); }
        inline bool operator==(const iterator_base &other) const
        {
            return n == other.n || (n && other.n ? *n == *other.n : false);
        }
        inline bool operator!=(const iterator_base &other) const { return !operator==(other); }
        inline iterator_base &firstChild() { if (n) { detach(); n->firstChild(); } return *this; }
        inline iterator_base &nextSibling() { if (n) { detach(); n->nextSibling(); } return *this; }

        inline iterator_base &parent() { if (n) { detach(); n->parent(); } return *this; }
        inline iterator_base &previousSibling() { if (n) { detach(); n->previousSibling(); } return *this; }

        inline QHash<QByteArray,QVariant> data(int column = 0, const QList<QByteArray> &roles = (QList<QByteArray>()<<"DisplayRole")) const
        {
            return (n ? n->data(column, roles) : QHash<QByteArray,QVariant>());
        }
        inline bool setData(int column, const QHash<QByteArray,QVariant> &values)
        {
            return (n ? n->setData(column, values) : false);
        }
        inline bool setData(int column, const QVariant &value, const QByteArray &role)
        {
            QHash<QByteArray,QVariant> values;
            values.insert(role, value);
            return setData(column, values);
        }

        inline bool isValid() const { return (n ? n->isValid() : false); }
        inline int columnCount() const { return (n ? n->columnCount() : 0); }

        inline bool hasChildren() const { return (n ? n->hasChildren() : false); }
        inline bool hasNextSibling() const { return (n ? n->hasNextSibling() : false); }

        inline bool hasParent() const { return (n ? n->hasParent() : false); }
        inline bool hasPreviousSibling() const { return (n ? n->hasPreviousSibling() : false); }

        inline uint hash() const { return (n ? n->hash() : 0); }

        inline QtTreeModelBase *model() { return (n ? n->model() : 0); }

    protected:
        navigator_base *n; // shared
    };

public:
    QtTreeModelBase(QObject *parent = 0) : QObject(parent) {}
    virtual ~QtTreeModelBase() {}
    virtual iterator_base begin() = 0;
    virtual int columnCount() const = 0;
    // QHash<int,QString> roles() const = 0; // ### FIXME
    
Q_SIGNALS:
    void itemsInserted(const QtTreeModelBase::iterator_base &it, int count);
    void itemsRemoved(const QtTreeModelBase::iterator_base &it, int count);
    void itemsChanged(const QtTreeModelBase::iterator_base &it, int count, const QList<QByteArray> &roles);

protected:
    QtTreeModelBase(QObjectPrivate &dd, QObject *parent) : QObject(dd, parent) {}
};

Q_DECLARE_TYPEINFO(QtTreeModelBase::iterator_base, Q_MOVABLE_TYPE);
typedef QtTreeModelBase::iterator_base QtTreeModelIterator;

inline uint qHash(const QtTreeModelBase::iterator_base &it);

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTREEMODELBASE_H
