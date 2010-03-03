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

#ifndef QTTABLECONTROLLER_P_H
#define QTTABLECONTROLLER_P_H

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

#include <qhash.h>
#include <qfont.h>
#include <qvariant.h>
#include <qitemeditorfactory.h>

QT_BEGIN_NAMESPACE

class QGraphicsSceneMouseEvent;

class QtGraphicsTableView;
class QtTableModelInterface;
class QtTableSelectionManager;
class QTableHeaderDataProvider;

class QtGraphicsHeader;

class QtTableControllerPrivate
{
    Q_DECLARE_PUBLIC(QtTableController)
public:
    QtTableControllerPrivate();
    ~QtTableControllerPrivate();

    void _q_modelDestroyed();
    void _q_viewDestroyed();
    void _q_selectionsDestroyed();
    void _q_verticalHeaderDestroyed();
    void _q_horizontalHeaderDestroyed();

    void _q_rowPressed(int row, Qt::KeyboardModifiers modifiers);
    void _q_rowReleased(int row, Qt::KeyboardModifiers modifiers);
    void _q_rowDragSelected(int row, Qt::KeyboardModifiers modifiers);
    void _q_columnPressed(int column, Qt::KeyboardModifiers modifiers);
    void _q_columnReleased(int column, Qt::KeyboardModifiers modifiers);
    void _q_columnDragSelected(int column, Qt::KeyboardModifiers modifiers);

    void _q_firstRowChanged(int row);
    void _q_firstColumnChanged(int column);
    void _q_verticalOffsetChanged(qreal offset);
    void _q_horizontalOffsetChanged(qreal offset);

    void _q_setHorizontalHeader(QtGraphicsHeader* header);
    void _q_setVerticalHeader(QtGraphicsHeader* header);

    // experimental editor stuff
    QWidget *cellEditor(int row, int column) const;
    void setCellEditor(QWidget *editor, int row, int column);
    void setCellEditorData(QWidget *editor, int row, int column);
    
    QtTableController *q_ptr;
    QtGraphicsTableView *view;
    QtTableModelInterface *model;
    QtGraphicsHeader *horizontalHeader;
    QtGraphicsHeader *verticalHeader;
    QTableHeaderDataProvider *horizontalHeaderDataProvider;
    QTableHeaderDataProvider *verticalHeaderDataProvider;    
    QtTableSelectionManager *selectionManager;

    // experimental editor stuff
    mutable QWidget *cachedEditorWidget;
    mutable int cachedEditorRow;
    mutable int cachedEditorColumn;
    QHash<quint64, QWidget*> editorHash;

    QItemEditorFactory *editorFactory;

    bool scrollPerRow;
    bool scrollPerColumn;
};

QT_END_NAMESPACE

#endif//QTTABLECONTROLLER_P_H
