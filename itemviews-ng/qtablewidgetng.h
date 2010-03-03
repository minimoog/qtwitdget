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

#ifndef QTTABLEWIDGETNG_H
#define QTTABLEWIDGETNG_H

#include "qitemviewsglobal.h"

#include <QtGui/qgraphicsview.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtTableController;
class QtTableDefaultModel;
class QtTableModelInterface;
class QtTableSelectionManager;
class QtTableWidgetNGPrivate;

/// Api docs
class Q_ITEMVIEWSNG_EXPORT QtTableWidgetNG : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QtTableWidgetNG(QWidget *parent = 0, int rows = 0, int columns = 0);
    explicit QtTableWidgetNG(QtTableController *controller, QWidget *parent = 0);
    virtual ~QtTableWidgetNG();

    QtTableController *controller() const;
    QtTableDefaultModel *defaultModel() const;

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

    void scrollContentsBy(int dx, int dy);

protected:
    QtTableWidgetNGPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtTableWidgetNG)
    Q_DISABLE_COPY(QtTableWidgetNG)
    Q_PRIVATE_SLOT(d_func(), void _q_update(const QRegion &region))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsInserted(int row, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsRemoved(int row, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsMoved(int from, int to, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsInserted(int column, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsRemoved(int column, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsMoved(int from, int to, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_showView())
    Q_PRIVATE_SLOT(d_func(), void _q_hideView())
    Q_PRIVATE_SLOT(d_func(), void _q_updateGeometries())
//    Q_PRIVATE_SLOT(d_func(), void _q_modelChanged(QtTableModelInterface *current, QtTableModelInterface *previous))
//    Q_PRIVATE_SLOT(d_func(), void _q_selectionManagerChanged(QtTableSelectionManager *current, QtTableSelectionManager *previous))
//    Q_PRIVATE_SLOT(d_func(), void _q_viewChanged(QtGraphicsTableView *current, QtGraphicsTableView *previous))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTABLEWIDGETNG_H
