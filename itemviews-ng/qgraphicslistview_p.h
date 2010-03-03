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

#ifndef QTGRAPHICSLISTVIEW_P_H
#define QTGRAPHICSLISTVIEW_P_H

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

#include <qgraphicsitem.h>
#include <qstyleoption.h>
#include <qtimer.h>
#include <qhash.h>

#define CACHING_ENABLED 1

QT_BEGIN_NAMESPACE

class QtGraphicsListViewItemPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsListViewItem)
public:
    int index;
    QtGraphicsListView *view;
    mutable QStyleOptionViewItemV4 option;
    QtGraphicsListViewItem *q_ptr;
};

class QtGraphicsTransform : public QGraphicsItem
{
public:
    QtGraphicsTransform(QGraphicsItem *parent) : QGraphicsItem(parent)
    { setFlag(QGraphicsItem::ItemHasNoContents); }
    ~QtGraphicsTransform() {}
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) {}
    QRectF boundingRect() const { return QRectF(pos(), QSizeF()); }
};

// QtGraphicsListViewPrivate

class QtGraphicsListViewPrivate //: public QGraphicsWidgetPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsListView)
public:
    QtGraphicsListViewPrivate();
    virtual ~QtGraphicsListViewPrivate();

    void _q_controllerDestroyed();
    void _q_modelDestroyed();
    void _q_selectionsDestroyed();

    void _q_itemsChanged(int index, int count, const QList<QByteArray> &roles);
    void _q_itemsInserted(int index, int count);
    void _q_itemsRemoved(int index, int count);
    void _q_reset();

    void _q_selectionsChanged(const QtListSelectionChange &change);
    void _q_currentChanged(int current, int previous);

    void updateHighlightGeometry();

    void checkCache(int index, int count);
    QVariant cachedData(int index, const QByteArray &role) const;

    bool isSelected(int index) const;
    int currentItem() const;

    // used by updateLayout
    QSizeF itemSize(int index) const;

    QtGraphicsListView *q_ptr;
    QtListController *controller;
    QtListModelInterface *model;
    QtListSelectionManager *selectionManager;

    Qt::Orientation orientation;
    Qt::TextElideMode textElideMode;

    QGraphicsObject *highlight;

    int firstIndex;
    qreal offset;

#if CACHING_ENABLED
    mutable int cachedIndexOffset;
    mutable qreal cachedCoordinateOffset;
#endif

    mutable QHash<QByteArray, QVariant> cachedDataHash;
    mutable int cachedDataIndex;

    QList<QPair<int, QGraphicsObject*> > items;
    QtGraphicsListViewItemCreatorBase *creator;
    QBasicTimer layoutTimer;
};

QT_END_NAMESPACE

#endif//QTGRAPHICSLISTVIEW_P_H
