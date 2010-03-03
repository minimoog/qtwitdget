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

#ifndef QTGRAPHICSSCROLLBAR_P_H
#define QTGRAPHICSSCROLLBAR_P_H

#include <qbasictimer.h>
#include <qstyle.h>

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

QT_BEGIN_NAMESPACE

class QtGraphicsScrollBarPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsScrollBar)
public:
    QtGraphicsScrollBarPrivate();
    ~QtGraphicsScrollBarPrivate();

    void performSubControlAction(QStyle::SubControl control);

    qreal value;
    qreal minimum;
    qreal maximum;

    qreal singleStep;
    qreal pageStep;

    Qt::Orientation orientation;

    qreal pressedValue;

    QStyle::SubControl hoveredControl;
    QStyle::SubControl pressedControl;

    QBasicTimer initialDelayTimer;
    QBasicTimer repeatActionTimer;

    int initialDelay;
    int repeatFrequency;

    QtGraphicsScrollBar *q_ptr;
};

QT_END_NAMESPACE

#endif//QTGRAPHICSSCROLLBAR_P_H
