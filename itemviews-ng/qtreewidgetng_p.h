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

#ifndef QTTREEWIDGETNG_P_H
#define QTTREEWIDGETNG_P_H

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

#include <qbasictimer.h>

QT_BEGIN_NAMESPACE

class QtTreeDefaultModel;
class QtTreeSelectionManager;
class QtTreeController;
class QtGraphicsTreeView;
class QtGraphicsHeader;
class QtGraphicsScrollBar;

class QtTreeWidgetNGPrivate
{
    Q_DECLARE_PUBLIC(QtTreeWidgetNG)
public:
    QtTreeWidgetNGPrivate();

    void _q_itemExpanded(QtTreeModelBase::iterator_base &it, int count);
    void _q_itemCollapsed(QtTreeModelBase::iterator_base &it, int count);
    void _q_itemsInserted(QtTreeModelBase::iterator_base &it, int count);
    void _q_itemsRemoved(QtTreeModelBase::iterator_base &, int count);
    void _q_itemsMoved(QtTreeModelBase::iterator_base &from, QtTreeModelBase::iterator_base &to, int count);
    void _q_showView();
    void _q_hideView();
    void _q_updateGeometries();

    void _q_firstIndexChanged(int index);
    void _q_firstSectionChanged(int section);
    void _q_offsetChanged(qreal offset);

    void _q_modelChanged(QtTreeModelBase *current, QtTreeModelBase *previous);
    void _q_selectionManagerChanged(QtTreeSelectionManager *current, QtTreeSelectionManager *previous);

    void _q_viewChanged(QtGraphicsTreeView *current, QtGraphicsTreeView *previous);
    void _q_headerChanged(QtGraphicsHeader *current, QtGraphicsHeader *previous);
    void _q_controllerChanged(QtTreeController *current, QtTreeController *previous);

    void initialize();
    
    QtTreeController *controller;
    QtGraphicsHeader *header;

    QBasicTimer timer;

    QtTreeWidgetNG *q_ptr;
};

QT_END_NAMESPACE

#endif//QTTREEWIDGETNG_P_H
