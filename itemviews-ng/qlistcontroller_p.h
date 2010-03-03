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

#ifndef QTLISTCONTROLLER_P_H
#define QTLISTCONTROLLER_P_H

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

#include <qglobal.h>
#include <qvariant.h>
#include <qpointer.h>

QT_BEGIN_NAMESPACE

class QtListController;
class QtListSelectionManager;
class QtGraphicsListView;
class QPropertyAnimation;

class QtListControllerPrivate
{
    Q_DECLARE_PUBLIC(QtListController)
public:
    QtListControllerPrivate();
    virtual ~QtListControllerPrivate();

    void _q_modelDestroyed();
    void _q_viewDestroyed();
    void _q_selectionsDestroyed();
    void _q_firstIndexChanged(int index);
    void _q_offsetChanged(qreal offset);
    void _q_currentChanged(int current, int previous);
    void _q_animationFinished();

    void animateFirstIndex(int index);
    // ### hack - not implemented in Qt
    QVariant inputMethodQueryHelper(Qt::InputMethodQuery query) const { Q_UNUSED(query); return QVariant(); }

    QtListController *q_ptr;

    QtListModelInterface *model;
    QPointer<QtListSelectionManager> selectionManager;
    QtGraphicsListView *view;

    QtListController::SelectionBehavior behavior;

    QPropertyAnimation *animation;
    int firstIndex;

    bool scrollPerItem;
    bool wheelEnabled;
};

QT_END_NAMESPACE

#endif//QTLISTCONTROLLER_P_H
