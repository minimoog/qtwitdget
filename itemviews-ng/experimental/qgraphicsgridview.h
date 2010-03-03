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

#ifndef QTGRAPHICSGRIDVIEW_H
#define QTGRAPHICSGRIDVIEW_H

#include "../qgraphicslistview.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtGraphicsGridViewPrivate;

class Q_ITEMVIEWSNG_EXPORT QtGraphicsGridView : public QtGraphicsListView
{
    Q_OBJECT
    Q_PROPERTY(int sectionCount READ sectionCount WRITE setSectionCount)
public:
    explicit QtGraphicsGridView(int sections = 3, Qt::Orientation = Qt::Vertical, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~QtGraphicsGridView();

    int sectionCount() const;
    void setSectionCount(int count);

    virtual int maximumFirstIndex() const;
    virtual qreal maximumHorizontalOffset() const;
    virtual qreal maximumVerticalOffset() const;
    
    virtual void doLayout();
    virtual QRectF itemGeometry(int index) const;
    virtual int itemAt(const QPointF &position) const;
    virtual void initStyleOption(QStyleOptionViewItemV4 *option) const;
    //virtual void initStyleOption(QStyleOptionViewItemV4 *option, int index) const;
    
protected:
    QtGraphicsGridView(QtGraphicsGridViewPrivate &, Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags = 0);
    
private:
    Q_DECLARE_PRIVATE_D(QtGraphicsListView::d_ptr, QtGraphicsGridView)
    Q_DISABLE_COPY(QtGraphicsGridView)
    Q_PRIVATE_SLOT(d_func(), void _q_selectionManagerChanged(QtListSelectionManager *current, QtListSelectionManager *previous))
    Q_PRIVATE_SLOT(d_func(), void _q_currentChanged(int current, int previous))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTGRAPHICSGRIDVIEW_H
