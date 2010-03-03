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

#ifndef QTGRAPHICSHEADER_P_H
#define QTGRAPHICSHEADER_P_H

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

#include "qsectionspans_p.h"

QT_BEGIN_NAMESPACE

class QStyle;
class QEvent;
class QKeyEvent;
class QInputMethodEvent;
class QMouseEvent;
class QDragEnterEvent;
class QDragLeaveEvent;
class QDragMoveEvent;
class QDropEvent;
class QHoverEvent;
class QtGraphicsHeaderDataProvider;

/*
  Notes To The Maintainer
  -----------------------

  1. The span data structure.

  We store section sizes in a list of spans. A size span consists of the section size
  of the span and the number of sections this span represents.

  The total size of the span is then the number of sections multiplied by the section
  size.

  The position of a specific section is found by accumulating the span sizes until we
  find the correct span. We can the find the position of the section within the span
  and add that to the accumulated size of the preceding spans.

  The the reasoning behind this approach is that there will be relatively few sections
  with different sizes, and many adjacent sections with the same size.

  If all sections in the header are the same size, we will only have one span.

  2. Coordinate system.

  The header has to deal with two coordinate systems; external and internal coordinates.
  This is because the header has an offset and a firstVisualIndex that affects the
  position of the sections relative to the external coordinates.
  Use the mapToInternal function to map external to the internal coordinates.

  3. Visual and logical indexes.

  The header operates with two kinds of indexes:
  - The visual index refers to the order in which the section appears in the view.
  - The logical index refers to the order in which the section appears in the model.

  Initially the visual and logical indexes are the same. But if sections are moved,
  the visual and logical indexes become different.

  To map between the visual and logical indexes, we have two vectors visualToLogical
  and logicalToVisual:

  logicalIndex == visualToLogical[visualIndex]
  visualIndex == logicalToVisual[logicalIndex]

  Note that the code always works on visual indexes internally.

  4. Hidden sections. ### may change ###

  Hidden sections are simply sections with no size.
  This means that they can not be interacted with, and are not
  painted. It is possible to show a hidden section by changing
  its size.
*/

class QtGraphicsHeaderPrivate
{
    Q_DECLARE_PUBLIC(QtGraphicsHeader);
public:
    QtGraphicsHeaderPrivate();
    ~QtGraphicsHeaderPrivate();

    qreal mapToInternal(const QPointF &pos, const QTransform &transform) const;
    
    bool mousePressEvent(QMouseEvent *event, const QTransform &transform);
    bool mouseMoveEvent(QMouseEvent *event, const QTransform &transform);
    bool mouseReleaseEvent(QMouseEvent *event, const QTransform &transform);
    bool mouseDoubleClickEvent(QMouseEvent *event, const QTransform &transform);
    bool dragEnterEvent(QDragEnterEvent *event, const QTransform &transform);
    bool dragLeaveEvent(QDragLeaveEvent *event, const QTransform &transform);
    bool dragMoveEvent(QDragMoveEvent *event, const QTransform &transform);
    bool dropEvent(QDropEvent *event, const QTransform &transform);
    bool hoverEnterEvent(QHoverEvent *event, const QTransform &transform);
    bool hoverMoveEvent(QHoverEvent *event, const QTransform &transform);
    bool hoverLeaveEvent(QHoverEvent *event, const QTransform &transform);
    bool resizeEvent(QResizeEvent *event, const QTransform &transform);

    void insertSections(int visualIndex, int count);
    void removeSections(int visualIndex, int count);

    bool isSectionCurrent(int visualIndex) const;
    bool isSectionSelected(int visualIndex) const;
    bool isSectionHighlighted(int visualIndex) const;
    bool isSectionUnderMouse(int visualIndex) const;
    bool isSectionPressed(int visualIndex) const;
    bool isSectionHidden(int visualIndex) const;
    bool isSectionClickable(int visualIndex) const;
    bool isSectionMovable(int visualIndex) const;
    bool isSortIndicatorShown(int visualIndex) const;

    Qt::SortOrder sortIndicatorOrder(int visualIndex) const;
    qreal contentSize() const;
    int sectionAt(qreal position, QPair<qreal,qreal> *geometry = 0) const;
    int sectionHandleAt(qreal position, int visualIndex = -1) const;

    QPair<qreal, qreal> sectionGeometry(int visualIndex) const;
    qreal sectionSize(int visualIndex) const;
    uint sectionMode(int visualIndex) const;

    void setSectionCount(int count);
    void setSectionSize(int visualIndex, qreal size);
    void setSectionMode(int visualIndex, uint mode); // ###

    //void resizeSectionsCascading();
    //void resizeSectionsStretch();
    //void resizeSections();

    QVariant data(int logicalIndex, const QByteArray &role) const;

    QtGraphicsHeaderDataProvider *dataProvider;
    mutable QHash<QByteArray,QVariant> cachedDataHash;
    mutable int cachedDataIndex;

    Qt::Orientation orientation;
    qreal offset;

    int firstVisualIndex;
    int sectionCount;

    QVector<int> logicalToVisual;
    QVector<int> visualToLogical;

    // section spans

    QtSectionSpans<qreal> sectionSizeSpans;
    QtSectionSpans<uint> sectionModeSpans;
    QtSectionSpans<bool> sectionMovableSpans;
    QtSectionSpans<bool> sectionClickableSpans;
    QtSectionSpans<bool> sectionSelectedSpans;

    // internal state

    enum State {
        Idle,
        PressingSection,
        ResizingSection,
        MovingSection,
        SelectingSections
    } state;

    // mouse positions

    qreal mousePressPosition;

    // resizing

    int resizingVisualIndex;
    qreal resizingOriginalSize;

    // moving

    int movingVisualIndex;
    qreal movingSectionSize;
    qreal movingCursorPosition;
    qreal movingCursorOffset;
    int movingCursorVisualIndex;

    // selecting

    int selectingVisualIndex;

    // other

    QtGraphicsHeader *q_ptr;
};

QT_END_NAMESPACE

#endif//QTGRAPHICSHEADER_P_H
