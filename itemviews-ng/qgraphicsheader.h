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

#ifndef QTGRAPHICSHEADER_H
#define QTGRAPHICSHEADER_H

#include "qitemviewsglobal.h"

#include <QtCore/qsize.h>
#include <QtGui/qtransform.h>
#include <QtGui/qgraphicswidget.h>
#include <QtGui/qstyleoption.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtGraphicsHeaderPrivate;

class QRegion;
class QPainter;
class QWidget;

class Q_ITEMVIEWSNG_EXPORT QtGraphicsHeaderDataProvider
{
public:
    virtual ~QtGraphicsHeaderDataProvider();
    virtual QHash<QByteArray,QVariant> data(int logicalIndex, const QList<QByteArray> &roles) const = 0;
};

class Q_ITEMVIEWSNG_EXPORT QtGraphicsHeader : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit QtGraphicsHeader(Qt::Orientation orientation = Qt::Horizontal, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~QtGraphicsHeader();

    void setDataProvider(QtGraphicsHeaderDataProvider *provider);
    QtGraphicsHeaderDataProvider *dataProvider() const;

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    qreal offset() const;
    qreal maximumOffset() const;

    qreal perpendicularSize() const;
    qreal contentSize() const;
    int sectionCount() const;
    int visibleSectionCount(qreal visibleLength) const;

    int firstSection() const;
    int maximumFirstSection() const;

    int mapToVisualIndex(int logicalIndex) const;
    int mapToLogicalIndex(int visualIndex) const;

    void setIndexMapping(const QVector<int> &visualToLogical);
    const QVector<int> &indexMapping() const;

    Qt::SortOrder sortIndicatorOrder(int logicalIndex) const;
    bool isSortIndicatorShown(int logicalIndex) const;

    qreal sectionSize(int logicalIndex) const;
    qreal sectionPosition(int logicalIndex) const;
    int sectionAt(const QPointF &position, const QTransform &transform = QTransform()) const;
    QPair<qreal,qreal> sectionGeometryAt(const QPointF &position, const QTransform &transform = QTransform()) const;

    bool isSectionHidden(int logicalIndex) const;
    bool isSectionMovable(int logicalIndex) const;
    bool isSectionClickable(int logicalIndex) const;
    bool isSectionSelected(int logicalIndex) const;

    bool isIdle() const; // ### FIXME not needed

    virtual bool processEvent(QEvent *event, const QTransform &transform = QTransform()); // ### FIXME

public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *graphicsOption = 0, QWidget *widget = 0);
    virtual void paintSection(QPainter *painter, QStyleOptionHeader *option, int logicalIndex) const;
    virtual void paintEmptySection(QPainter *painter, QStyleOptionHeader *option) const;
    virtual void paintDragIndicator(QPainter *painter, QStyleOptionHeader *option, int logicalIndex) const;
    virtual QSizeF sectionSizeHint(QStyleOptionHeader *option, int logicalIndex) const;
    virtual void initStyleOption(QStyleOptionHeader *option, QWidget *widget) const;
    virtual void initStyleOption(QStyleOptionHeader *option, int logicalIndex) const;
    //virtual void autoResizeSections();
    void copyStyleOptionState(const QStyleOptionGraphicsItem *source, QStyleOptionHeader *dest);
    void copySections(const QtGraphicsHeader &other);

public Q_SLOTS:
    void setOffset(qreal offset);
    void setSectionCount(int count);
    void setFirstSection(int visualIndex);
    void setSectionSize(int logicalIndex, qreal size);
    void setSectionHidden(int logicalIndex, bool hidden = true);
    void setSectionMovable(int logicalIndex, bool movable = true);
    void setSectionClickable(int logicalIndex, bool clickable = true);
    void setSectionSelected(int logicalIndex, bool selected = true);
    void updateSections(int logicalIndex, int count);
    void insertSections(int logicalIndex, int count);
    void removeSections(int logicalIndex, int count);
    void moveSection(int visualIndexFrom, int visualIndexTo);
    void setSortIndicatorOrder(int logicalIndex, Qt::SortOrder order);
    void setSortIndicatorShown(int logicalIndex, bool show = true);
    //void setSectionResizeMode(int logicalIndex, int mode);
    
Q_SIGNALS:
    void sectionMoved(int logicalIndex, int visualIndexTo, int visualIndexFrom);
    void sectionResized(int logicalIndex, qreal newSize, qreal oldSize);
    void sectionPressed(int logicalIndex, Qt::KeyboardModifiers modifiers);
    void sectionReleased(int logicalIndex, Qt::KeyboardModifiers modifiers);
    void sectionClicked(int logicalIndex, Qt::KeyboardModifiers modifiers);
    void sectionEntered(int logicalIndex, Qt::KeyboardModifiers modifiers);
    void sectionDoubleClicked(int logicalIndex, Qt::KeyboardModifiers modifiers);
    void sectionDragSelected(int logicalIndex, Qt::KeyboardModifiers modifiers);
    void sectionCountChanged(int newCount, int oldCount);
    void sectionHandleDoubleClicked(int logicalIndex);
    void sortIndicatorOrderChanged(int logicalIndex, Qt::SortOrder order);
    void sortIndicatorShownChanged(int logicalIndex, bool shown);
    void firstSectionChanged(int visualIndex);
    void offsetChanged(qreal offset);

protected:
    QtGraphicsHeader(QtGraphicsHeaderPrivate &, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual bool event(QEvent *event);

protected:
    QtGraphicsHeaderPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtGraphicsHeader)
    Q_DISABLE_COPY(QtGraphicsHeader)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTGRAPHICSHEADER_H
