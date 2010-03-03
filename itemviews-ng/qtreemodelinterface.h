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

#ifndef QTTREEMODELINTERFACE_H
#define QTTREEMODELINTERFACE_H

#include "qtreemodelbase.h"
#include <QtCore/qset.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

// ### API docs
template <typename T>
class QtTreeModelInterface : public QtTreeModelBase
{
protected:
    struct navigator : public navigator_base
    {
        navigator(T t, QtTreeModelInterface<T> *m)
            : navigator_base(), t(t), m(m) { m->navigators.insert(this); }
        ~navigator() { if (m) m->navigators.remove(this); }

        // implementation
        void firstChild() { if (m) t = m->firstChild(t); }
        void nextSibling() { if (m) t = m->nextSibling(t); }
        void parent() { if (m) t = m->parentItem(t); }
        void previousSibling() { if (m) t = m->previousSibling(t); }
        bool operator==(const navigator_base &other) const { return (reinterpret_cast<const navigator*>(&other)->t == t); }
        QHash<QByteArray,QVariant> data(int column, const QList<QByteArray> &roles) const { return m ? m->data(t, column, roles) : QHash<QByteArray,QVariant>(); }
        bool setData(int column, const QHash<QByteArray,QVariant> &values) { return m ? m->setData(t, column, values) : false; }
        bool isValid() const { return m && m->isValid(t); }
        int columnCount() const { return m ? m->columnCount(t) : 0; }
        bool hasChildren() const { return m ? m->hasChildren(t) : false; }
        bool hasNextSibling() const { return m ? m->hasNextSibling(t) : false; }
        bool hasParent() const { return m ? m->hasParent(t) : false; }
        bool hasPreviousSibling() const { return m ? m->hasPreviousSibling(t) : false; }
        uint hash() const { return m ? m->hash(t) : qHash(t); }
        navigator_base *detach_helper() { return new navigator(t, m); }
        QtTreeModelBase *model() { return m; }

        T t;
        QtTreeModelInterface<T> *m;
    };

    class iterator : public iterator_base
    {
    public:
        inline iterator(T t, QtTreeModelInterface *m) : iterator_base()
        {
            n = new navigator(t, m);
            n->ref.ref();
        }

        // ### hacky, but useful sometimes
        inline T operator*() { return reinterpret_cast<navigator*>(n)->t; }
    };

    // ### experimental
    class const_iterator : private iterator_base
    {
    public:
        inline const_iterator(const T t, const QtTreeModelInterface *m) : iterator_base()
        {
            n = new navigator(t, const_cast<QtTreeModelInterface*>(m));
            n->ref.ref();
        }

        inline const_iterator(const iterator_base &other) : iterator_base(other) {}

        inline bool operator==(const iterator_base &other) const { return iterator_base::operator==(other); }
        inline const_iterator &firstChild() { (void)iterator_base::firstChild(); return *this; }
        inline const_iterator &nextSibling() { (void)iterator_base::nextSibling(); return *this; }
        inline const_iterator &parent() { (void)iterator_base::parent(); return *this; }
        inline const_iterator &previousSibling() { (void)iterator_base::previousSibling(); return *this; }
        inline QHash<QByteArray,QVariant> data(int column = 0, const QList<QByteArray> &roles = (QList<QByteArray>()<<"DisplayRole")) const
        {
            return iterator_base::data(column, roles);
        }
        inline bool isValid() const { return iterator_base::isValid(); }
        inline int columnCount() const { return iterator_base::columnCount(); } // ### per model ???

        inline bool hasChildren() const { return iterator_base::hasChildren(); }
        inline bool hasNextSibling() const { return iterator_base::hasNextSibling(); }

        inline bool hasParent() const { return iterator_base::hasParent(); }
        inline bool hasPreviousSibling() const { return iterator_base::hasPreviousSibling(); }

        inline uint hash() const { return iterator_base::hash(); }
    };

public:
    QtTreeModelInterface(QObject *parent = 0) : QtTreeModelBase(parent) {}
    ~QtTreeModelInterface()
    {
        foreach(navigator* n, navigators)
            n->m = 0;
    }

    iterator_base begin()
    {
        return iterator(firstChild(T()), this);
    }

    const_iterator begin() const
    {
        return const_iterator(firstChild(T()), this);
    }

    int columnCount() const
    {
        return columnCount(T());
    }

protected:
    // ### this is what needs to be implemented
    virtual T firstChild(T t) const = 0;
    virtual T nextSibling(T t) const = 0; // ### these may be hard to implement efficiently
    virtual T previousSibling(T t) const = 0;
    virtual T parentItem(T t) const = 0; // ### we can't use a stack, since we construct iterators for the signals    
    virtual QHash<QByteArray,QVariant> data(T t, int column, const QList<QByteArray> &roles) const = 0;
    virtual bool isValid(T t) const = 0; // ### no good default implementation

    // ### these are optional
    virtual int columnCount(T t) const { Q_UNUSED(t); return 1; }
    virtual bool hasChildren(T t) const { return isValid(t) && isValid(const_cast<QtTreeModelInterface*>(this)->firstChild(t)); }
    virtual bool hasNextSibling(T t) const { return isValid(t) && isValid(const_cast<QtTreeModelInterface*>(this)->nextSibling(t)); }
    virtual bool hasPreviousSibling(T t) const { return isValid(t) && isValid(const_cast<QtTreeModelInterface*>(this)->previousSibling(t)); }
    virtual bool hasParent(T t) const { return isValid(t) && isValid(const_cast<QtTreeModelInterface*>(this)->parentItem(t)); }
    virtual uint hash(T t) const { return qHash(t); } // ### needed for selections
    virtual bool setData(T t, int column, const QHash<QByteArray,QVariant> &values)
    {
        Q_UNUSED(t); Q_UNUSED(column); Q_UNUSED(values);
        return false;
    }

    inline void itemsInserted(iterator &it, int count) { QtTreeModelBase::itemsInserted(it, count); }
    inline void itemsRemoved(iterator &it, int count) { QtTreeModelBase::itemsRemoved(it, count); }
    inline void itemsChanged(iterator &it, int count, const QList<QByteArray> &roles) { QtTreeModelBase::itemsChanged(it, count, roles); }
    
protected: // let the developers have access to the navigators
    QtTreeModelInterface(QObjectPrivate &dd, QObject *parent) : QtTreeModelBase(dd, parent) {}
    QSet<navigator*> navigators;
};

inline uint qHash(const QtTreeModelBase::iterator_base &it) { return it.hash(); }
//template <class T> inline uint qHash(const T &t) { qDebug() << "%%%%%%"; Q_UNUSED(t); return 0; }

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
/*Q_CORE_EXPORT_INLINE*/ inline QDebug operator<<(QDebug dbg, const QtTreeModelBase::iterator_base &it)
{
#ifndef QT_BROKEN_DEBUG_STREAM
    dbg.nospace() << "QtTreeModelIterator(" << it.hash() << ")";
    return dbg.space();
#else
    Q_UNUSED(it);
    qWarning("This compiler doesn't support streaming QtTreeModelIterator to QDebug");
    return dbg;
#endif
}
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTREEMODELINTERFACE_H
