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

#ifndef QTLISTWIDGETNG_H
#define QTLISTWIDGETNG_H

#include "qitemviewsglobal.h"

#include <QtGui/qgraphicsview.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtListController;
class QtListDefaultModel;
class QtListModelInterface;
class QtGraphicsListView;
class QtListWidgetNGPrivate;

class Q_ITEMVIEWSNG_EXPORT QtListWidgetNG : public QAbstractScrollArea
{
    Q_OBJECT
public:
    QtListWidgetNG(QWidget *parent = 0);
    explicit QtListWidgetNG(QtListController *controller, QWidget *parent = 0);
    virtual ~QtListWidgetNG();

    QtListController *controller() const;
    QtListDefaultModel *defaultModel() const;

protected:
    bool viewportEvent(QEvent *event);
    void scrollContentsBy(int dx, int dy);

protected:
    QtListWidgetNGPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtListWidgetNG)
    Q_DISABLE_COPY(QtListWidgetNG)
    Q_PRIVATE_SLOT(d_func(), void _q_itemsInserted(int index, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsRemoved(int index, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsMoved(int from, int to, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsChanged(int from, int count, const QList<int> &roles))
    Q_PRIVATE_SLOT(d_func(), void _q_showView())
    Q_PRIVATE_SLOT(d_func(), void _q_hideView())
    Q_PRIVATE_SLOT(d_func(), void _q_updateGeometries())
    Q_PRIVATE_SLOT(d_func(), void _q_modelChanged(QtListModelInterface *current, QtListModelInterface *previous))
    Q_PRIVATE_SLOT(d_func(), void _q_selectionManagerChanged(QtListSelectionManager *current, QtListSelectionManager *previous))
    Q_PRIVATE_SLOT(d_func(), void _q_viewChanged(QtGraphicsListView *current, QtGraphicsListView *previous))
    Q_PRIVATE_SLOT(d_func(), void _q_firstIndexChanged(int index))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTLISTWIDGETNG_H
