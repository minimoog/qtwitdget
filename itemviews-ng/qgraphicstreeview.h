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

#ifndef QTGRAPHICSTREEVIEW_H
#define QTGRAPHICSTREEVIEW_H

#include "qitemviewsglobal.h"

#include "qtreemodelinterface.h"

#include <QtCore/qsize.h>
#include <QtCore/qstack.h>
#include <QtGui/qtransform.h>
#include <QtGui/qgraphicswidget.h>
#include <QtGui/qstyleoption.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtGraphicsTreeView;
class QtGraphicsTreeViewPrivate;
class QtGraphicsTreeViewItemPrivate;
class QtTreeSelectionManager;
class QtTreeSelections;
class QtTreeController;
class QtGraphicsHeader;

class QPointF;
class QRegion;
class QPainter;
class QWidget;

class Q_ITEMVIEWSNG_EXPORT QtGraphicsTreeViewItem : public QGraphicsWidget
{
public:
    QtGraphicsTreeViewItem(const QtTreeModelIterator &it, QtGraphicsTreeView *view);
    ~QtGraphicsTreeViewItem();

    QtTreeModelIterator iterator() const;
    void setIterator(const QtTreeModelIterator &it);

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual QSizeF sizeHint(int column, const QStyleOptionViewItemV4 *option, Qt::SizeHint which, const QSizeF &constraint) const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void paintBranches(QPainter *painter, QStyleOptionViewItemV4 *option, const QStack<QtTreeModelIterator> &stack);
    virtual void itemChanged(const QList<QByteArray> &roles = QList<QByteArray>());

    virtual void initStyleOption(QStyleOptionViewItemV4 *option, int column) const;
    virtual void initStyleOptionState(QStyleOptionViewItemV4 *option, const QtTreeModelIterator &it, int column) const;
    void copyStyleOptionState(const QStyleOptionGraphicsItem *source, QStyleOptionViewItemV4 *dest);

    QHash<QByteArray, QVariant> data(int column = 0, const QList<QByteArray> &roles = QList<QByteArray>()) const;
    QtGraphicsTreeView *view() const;

protected:
    QtGraphicsTreeViewItemPrivate *d_ptr;

private:
    Q_DISABLE_COPY(QtGraphicsTreeViewItem)
    Q_DECLARE_PRIVATE(QtGraphicsTreeViewItem)
};

class Q_ITEMVIEWSNG_EXPORT QtGraphicsTreeViewItemCreatorBase
{
public:
    virtual ~QtGraphicsTreeViewItemCreatorBase();
    virtual QGraphicsObject *create(const QtTreeModelIterator &it, QtGraphicsTreeView *view) = 0;
    virtual QGraphicsObject *reassign(const QtTreeModelIterator &it, QGraphicsObject *item, QtGraphicsTreeView *view) = 0;
    virtual void update(const QtTreeModelIterator &it, QGraphicsObject *item, const QList<QByteArray> &roles);
    virtual void recycle(QGraphicsObject *item);
};

template <class T>
class QtGraphicsTreeViewItemCreator : public QtGraphicsTreeViewItemCreatorBase
{
public:
    inline ~QtGraphicsTreeViewItemCreator();
    inline QGraphicsObject *create(const QtTreeModelIterator &it, QtGraphicsTreeView *view);
    inline QGraphicsObject *reassign(const QtTreeModelIterator &it, QGraphicsObject *item, QtGraphicsTreeView *view);
    inline void update(const QtTreeModelIterator &it, QGraphicsObject *item, const QList<QByteArray> &roles);
};

template <class T>
QtGraphicsTreeViewItemCreator<T>::~QtGraphicsTreeViewItemCreator()
{}

template <class T>
inline QGraphicsObject *QtGraphicsTreeViewItemCreator<T>::create(const QtTreeModelIterator &it, QtGraphicsTreeView *view)
{ return new T(it, view); }

template <class T>
inline QGraphicsObject *QtGraphicsTreeViewItemCreator<T>::reassign(const QtTreeModelIterator &it, QGraphicsObject *item, QtGraphicsTreeView *view)
{
	Q_UNUSED(view);
	static_cast<QtGraphicsTreeViewItem*>(item)->setIterator(it);
	return item;
}

template <class T>
inline void QtGraphicsTreeViewItemCreator<T>::update(const QtTreeModelIterator &it, QGraphicsObject *item, const QList<QByteArray> &roles)
{
	Q_UNUSED(it);
	static_cast<QtGraphicsTreeViewItem*>(item)->itemChanged(roles);
}

class Q_ITEMVIEWSNG_EXPORT QtGraphicsTreeView : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QtTreeModelBase* model READ model WRITE setModel) // ### 
    Q_PROPERTY(int firstIndex READ firstIndex WRITE setFirstIndex)
    Q_PROPERTY(qreal horizontalOffset READ horizontalOffset WRITE setHorizontalOffset)
    Q_PROPERTY(qreal verticalOffset READ verticalOffset WRITE setVerticalOffset)
    Q_PROPERTY(Qt::TextElideMode textElideMode READ textElideMode WRITE setTextElideMode)

public:
    QtGraphicsTreeView(QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~QtGraphicsTreeView();

    QtTreeController *controller() const;
    void setController(QtTreeController *controller);

    QtTreeModelBase *model() const;
    QtTreeSelectionManager *selectionManager() const;

    QtGraphicsHeader *header() const;
    void setHeader(QtGraphicsHeader *header);

    void setTextElideMode(Qt::TextElideMode mode);
    Qt::TextElideMode textElideMode() const;

    int firstIndex() const;
    int firstSection() const;

    qreal horizontalOffset() const;
    qreal verticalOffset() const;

    int maximumFirstIndex(int *count = 0) const;
    int maximumFirstSection() const;

    qreal maximumHorizontalOffset() const;
    qreal maximumVerticalOffset() const;

    bool isItemExpanded(const QtTreeModelIterator &it) const;
    QList<QtTreeModelIterator> expandedItems() const;

    void nextItem(QtTreeModelIterator &it) const;
    void previousItem(QtTreeModelIterator &it) const;

    QRectF disclosureRect(const QRectF &rect) const; // ### FIXME: make virtual ?

    QtGraphicsTreeViewItemCreatorBase *itemCreator() const;
    void setItemCreator(QtGraphicsTreeViewItemCreatorBase *creator);

    QGraphicsObject *itemForIterator(const QtTreeModelIterator &it) const; // ### FIXME: make virtual ?

    virtual void doLayout();
    virtual QtTreeModelIterator itemAt(const QPointF &position, int *index = 0, QRectF *rect = 0) const;
    virtual int indexOf(const QtTreeModelIterator &it) const;

    virtual void initStyleOption(QStyleOptionViewItemV4 *option) const;

public Q_SLOTS:
    void setFirstIndex(int index);
    void setHorizontalOffset(qreal offset);
    void setVerticalOffset(qreal offset);
    void setItemExpanded(QtTreeModelIterator &it, bool expanded = true);
    void updateLayout();

Q_SIGNALS:
    void firstIndexChanged(int index);
    void horizontalOffsetChanged(qreal offset);
    void verticalOffsetChanged(qreal offset);
    void itemExpanded(QtTreeModelIterator &it, int count);
    void itemCollapsed(QtTreeModelIterator &it, int count);
    void layoutChanged();

protected:
    QtGraphicsTreeView(QtGraphicsTreeViewPrivate &, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual bool event(QEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void setModel(QtTreeModelBase *model);
    void setSelectionManager(QtTreeSelectionManager *selections);

protected:
    QtGraphicsTreeViewPrivate *d_ptr;

private:
    friend class QtTreeController;
    friend class QtGraphicsTreeViewItem;
    Q_DECLARE_PRIVATE(QtGraphicsTreeView)
    Q_DISABLE_COPY(QtGraphicsTreeView)
    Q_PRIVATE_SLOT(d_func(), void _q_controllerDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_headerDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_itemsChanged(const QtTreeModelBase::iterator_base &it, int count, const QList<QByteArray> &roles))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsInserted(const QtTreeModelBase::iterator_base &it, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsRemoved(const QtTreeModelBase::iterator_base &it, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsChanged(const QtTreeSelection &selection))
    Q_PRIVATE_SLOT(d_func(), void _q_currentChanged(const QtTreeModelBase::iterator_base &current, const QtTreeModelBase::iterator_base &previous))
    Q_PRIVATE_SLOT(d_func(), void _q_reset())
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTGRAPHICSTREEVIEW_H
