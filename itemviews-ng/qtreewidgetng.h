/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the ItemWidgets NG project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please reWidget the following information to ensure GNU
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

#ifndef QTTREEWIDGETNG_H
#define QTTREEWIDGETNG_H

#include "qtreemodelinterface.h"

#include <QtGui/qgraphicsview.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtTreeController;
class QtTreeDefaultModel;
class QtTreeModelBase;
class QtTreeSelectionManager;
class QtTreeWidgetNGPrivate;

/// Api docs
class Q_ITEMVIEWSNG_EXPORT QtTreeWidgetNG : public QGraphicsView
{
    Q_OBJECT
public:
    QtTreeWidgetNG(QWidget *parent = 0);
    explicit QtTreeWidgetNG(QtTreeController *controller, QWidget *parent = 0);
    ~QtTreeWidgetNG();

    QtTreeController *controller() const;
    QtTreeDefaultModel *defaultModel() const;

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
    void timerEvent(QTimerEvent *event);

    void scrollContentsBy(int dx, int dy);

protected:
    QtTreeWidgetNGPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtTreeWidgetNG)
    Q_DISABLE_COPY(QtTreeWidgetNG)
    Q_PRIVATE_SLOT(d_func(), void _q_itemExpanded(QtTreeModelIterator &it, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemCollapsed(QtTreeModelIterator &it, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsInserted(QtTreeModelIterator &it, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsRemoved(QtTreeModelIterator &it, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsMoved(QtTreeModelIterator &from, QtTreeModelIterator &to, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_showView())
    Q_PRIVATE_SLOT(d_func(), void _q_hideView())
    Q_PRIVATE_SLOT(d_func(), void _q_updateGeometries())
    Q_PRIVATE_SLOT(d_func(), void _q_modelChanged(QtTreeModelBase *current, QtTreeModelBase *previous))
    Q_PRIVATE_SLOT(d_func(), void _q_selectionManagerChanged(QtTreeSelectionManager *current, QtTreeSelectionManager *previous))
    Q_PRIVATE_SLOT(d_func(), void _q_viewChanged(QtGraphicsTreeView *current, QtGraphicsTreeView *previous))
    Q_PRIVATE_SLOT(d_func(), void _q_firstIndexChanged(int index))
    Q_PRIVATE_SLOT(d_func(), void _q_firstSectionChanged(int section))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTREEWIDGETNG_H
