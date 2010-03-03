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

#ifndef QTTREESELECTIONMANAGER_P_H
#define QTTREESELECTIONMANAGER_P_H

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

#include <qset.h>

#include "qtreemodelinterface.h"

QT_BEGIN_NAMESPACE

class QtTreeSelectionManagerPrivate
{
    Q_DECLARE_PUBLIC(QtTreeSelectionManager);
public:
    QtTreeSelectionManagerPrivate();
    ~QtTreeSelectionManagerPrivate();

    void _q_modelDestroyed();
    void _q_itemsRemoved(const QtTreeModelBase::iterator_base &it, int count);
    void _q_reset();

    QtTreeSelectionManager *q_ptr;
    QtTreeModelBase *model;
    QtTreeModelBase::iterator_base currentItem;
    QtTreeModelBase::iterator_base anchorItem;
    QSet<QtTreeModelBase::iterator_base> anchorSelections;
    QSet<QtTreeModelBase::iterator_base> selections;
    QtTreeSelectionManager::SelectionMode mode;
};

QT_END_NAMESPACE

#endif//QTTREESELECTIONMANAGER_P_H
