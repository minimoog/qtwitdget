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

#ifndef QTLISTSELECTIONMANAGER_P_H
#define QTLISTSELECTIONMANAGER_P_H

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

#include <qpair.h>
#include <qbitarray.h>
#include <qshareddata.h>

QT_BEGIN_NAMESPACE

  /*
  Notes To The Maintainer
  -----------------------
  The selections are composed of two layers:
  1. The selections bit array.
  2. The anchored selection, consisting of the range from anchor to current item.
  */

class QtListSelectionManager;
class QtListModelInterface;

class QtListSelectionChangeData : public QSharedData
{
public:
    QtListSelectionChangeData();
    ~QtListSelectionChangeData();

    void clear();
    void setRanges(int oldAnchor, int oldCurrent, int newAnchor, int newCurrent,
                   const QBitArray &selections, QtListSelectionManager::SelectionMode mode);
    void setIndexes(int from, int to, const QList<int> &changed);
    void setSelections(const QBitArray &previous, const QBitArray &current);

    QList<int> changedIndexes() const;
    int from() const;
    int to() const;
    int count() const;

private:
    QPair<int,int> previousRange;
    QPair<int,int> currentRange;
    QPair<int,int> boundingRange;

    bool toggle;
    QBitArray previousSelections;
    QBitArray currentSelections;

    // cached changed indexes
    mutable QList<int> indexes;
};

class QtListSelectionManagerPrivate
{
    Q_DECLARE_PUBLIC(QtListSelectionManager)
public:
    QtListSelectionManagerPrivate();
    virtual ~QtListSelectionManagerPrivate();

    void _q_modelDestroyed();
    void _q_itemsInserted(int index, int count);
    void _q_itemsRemoved(int index, int count);
    void _q_itemsMoved(int from, int to, int count);
    void _q_reset();

    QtListSelectionManager *q_ptr;
    QtListModelInterface *model;

    int current;
    int anchor;
    bool active;
    QBitArray selections;

    QtListSelectionManager::SelectionMode mode;

    QtListSelectionChange change;
};

QT_END_NAMESPACE

#endif//QTLISTSELECTIONMANAGER_P_H
