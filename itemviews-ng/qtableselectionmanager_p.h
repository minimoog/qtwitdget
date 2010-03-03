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

#ifndef QTTABLESELECTIONMANAGER_P_H
#define QTTABLESELECTIONMANAGER_P_H

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

#include "qtablemodelinterface.h"

QT_BEGIN_NAMESPACE

class QtTableSelectionManagerPrivate
{
    Q_DECLARE_PUBLIC(QtTableSelectionManager)
public:
    QtTableSelectionManagerPrivate();
    ~QtTableSelectionManagerPrivate();
    
    void _q_modelDestroyed();
    void _q_rowsInserted(int row, int count);
    void _q_rowsRemoved(int row, int count);
    void _q_columnsInserted(int column, int count);
    void _q_columnsRemoved(int column, int count);
    void _q_reset();

    bool isRowIntersected(int row) const;
    bool isColumnIntersected(int row) const;
    bool isSelected(int row, int column) const;

    void selectRange(const QtTableSelectionRange &range);
    void deselectRange(const QtTableSelectionRange &range);
    void toggleRange(const QtTableSelectionRange &range);

    QtTableSelectionManager *q_ptr;
    QtTableModelInterface *model;
    QList<QtTableSelectionRange> selections;
    int currentRow;
    int currentColumn;
    int anchorRow;
    int anchorColumn;
    bool active;
    QtTableSelectionRange currentAnchorRange;
    QtTableSelectionManager::SelectionMode mode;
};

QT_END_NAMESPACE

#endif//QTTABLESELECTIONMANAGER_P_H
