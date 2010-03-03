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

#ifndef QTGRAPHICSLISTVIEW_H
#define QTGRAPHICSLISTVIEW_H

#include "qitemviewsglobal.h"

#include <QtGui/qgraphicswidget.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QSizeF;
class QPointF;
class QtListController;
class QtListModelInterface;
class QtListSelectionManager;
class QtListSelectionChange;
class QStyleOptionViewItemV4;

class QtGraphicsListViewItemPrivate;
class QtGraphicsListViewPrivate;
class QtGraphicsListView;

class Q_ITEMVIEWSNG_EXPORT QtGraphicsListViewItem : public QGraphicsWidget
{
    Q_OBJECT
public:
    QtGraphicsListViewItem(int index, QtGraphicsListView *view);
    virtual ~QtGraphicsListViewItem();

    int index() const;
    void setIndex(int index);

    QRectF boundingRect() const;

    virtual QSizeF sizeHint(Qt::SizeHint which = Qt::PreferredSize, const QSizeF &constraint = QSizeF()) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void itemChanged(const QList<QByteArray> &roles = QList<QByteArray>());
    virtual void initStyleOption(QStyleOptionViewItemV4 *option) const;

    QHash<QByteArray, QVariant> data(const QList<QByteArray> &roles = QList<QByteArray>()) const;
    QtGraphicsListView *view() const;

protected:
    QtGraphicsListViewItemPrivate *d_ptr;

private:
    Q_DISABLE_COPY(QtGraphicsListViewItem)
    Q_DECLARE_PRIVATE(QtGraphicsListViewItem)
};

class Q_ITEMVIEWSNG_EXPORT QtGraphicsListViewItemCreatorBase
{
public:
    virtual ~QtGraphicsListViewItemCreatorBase();
    virtual QGraphicsObject *create(int index, QtGraphicsListView *view) = 0;
    virtual QGraphicsObject *reassign(int index, QGraphicsObject *item, QtGraphicsListView *view) = 0;
    virtual void update(int index, QGraphicsObject *item, const QList<QByteArray> &roles);
    virtual void recycle(QGraphicsObject *item);
};

template <class T>
class QtGraphicsListViewItemCreator : public QtGraphicsListViewItemCreatorBase
{
public:
    inline ~QtGraphicsListViewItemCreator();
    inline QGraphicsObject *create(int index, QtGraphicsListView *view);
    inline QGraphicsObject *reassign(int index, QGraphicsObject *item, QtGraphicsListView *view);
    inline void update(int index, QGraphicsObject *item, const QList<QByteArray> &roles);
};

template <class T>
QtGraphicsListViewItemCreator<T>::~QtGraphicsListViewItemCreator()
{}

template <class T>
inline QGraphicsObject *QtGraphicsListViewItemCreator<T>::create(int index, QtGraphicsListView *view)
{ return new T(index, view); }

template <class T>
inline QGraphicsObject *QtGraphicsListViewItemCreator<T>::reassign(int index, QGraphicsObject *item, QtGraphicsListView *view)
{
	Q_UNUSED(view);
	static_cast<T*>(item)->setIndex(index);
	return item;
}

template <class T>
inline void QtGraphicsListViewItemCreator<T>::update(int index, QGraphicsObject *item, const QList<QByteArray> &roles)
{
	Q_UNUSED(index);
	static_cast<T*>(item)->itemChanged(roles);
}

class Q_ITEMVIEWSNG_EXPORT QtGraphicsListView : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(Qt::TextElideMode textElideMode READ textElideMode WRITE setTextElideMode)
    Q_PROPERTY(int firstIndex READ firstIndex WRITE setFirstIndex NOTIFY firstIndexChanged)
    Q_PROPERTY(qreal offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(QGraphicsObject* highlight READ highlight WRITE setHighlight)

public:
    QtGraphicsListView(Qt::Orientation orientation = Qt::Vertical, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~QtGraphicsListView();

    QtListController *controller() const;
    QtListModelInterface *model() const;
    QtListSelectionManager *selectionManager() const;

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    Qt::TextElideMode textElideMode() const;
    void setTextElideMode(Qt::TextElideMode mode);

    virtual int itemAt(const QPointF &position) const;
    virtual QRectF itemGeometry(int index) const;
    virtual void doLayout();

    int firstIndex() const;
    qreal offset() const;

    QGraphicsObject *highlight() const;
    void setHighlight(QGraphicsObject *highlight);

    virtual int maximumFirstIndex() const;
    virtual qreal maximumOffset() const;

    virtual void initStyleOption(QStyleOptionViewItemV4 *option) const;

    QtGraphicsListViewItemCreatorBase *itemCreator() const;
    void setItemCreator(QtGraphicsListViewItemCreatorBase *creator);

    QGraphicsObject *itemForIndex(int index) const;

    qreal offsetToEnsureIndexIsVisible(int index) const;
    int firstIndexToEnsureIndexIsVisible(int index) const;

public Q_SLOTS:
    void setFirstIndex(int index);
    void setOffset(qreal offset);
    void updateLayout();

Q_SIGNALS:
    void orientationChanged(Qt::Orientation orientation);
    void firstIndexChanged(int index);
    void offsetChanged(qreal offset);

protected:
    QtGraphicsListView(QtGraphicsListViewPrivate &, Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags = 0);

    // ### making them virtual allows the user to hook up to any signals they need
    virtual void setController(QtListController *control);
    virtual void setModel(QtListModelInterface *model);
    virtual void setSelectionManager(QtListSelectionManager *selectionManager);

    virtual bool event(QEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QtGraphicsListViewPrivate *d_ptr;

private:
    friend class QtPrinterListView;
    friend class QtListController;
    friend class QtGraphicsListViewItem;
    Q_DECLARE_PRIVATE(QtGraphicsListView)
    Q_DISABLE_COPY(QtGraphicsListView)
    Q_PRIVATE_SLOT(d_func(), void _q_controllerDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_itemsChanged(int index, int count, const QList<QByteArray> &roles))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsInserted(int index, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsRemoved(int index, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsChanged(const QtListSelectionChange &change))
    Q_PRIVATE_SLOT(d_func(), void _q_currentChanged(int current, int previous))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTGRAPHICSLISTVIEW_H
