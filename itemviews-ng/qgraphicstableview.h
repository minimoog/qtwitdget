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

#ifndef QTGRAPHICSTABLEVIEW_H
#define QTGRAPHICSTABLEVIEW_H

#include "qitemviewsglobal.h"

#include <QtCore/qsize.h>
#include <QtGui/qtransform.h>
#include <QtGui/qgraphicswidget.h>
#include <QtGui/qstyleoption.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtGraphicsHeader;
class QtTableController;
class QtTableModelInterface;
class QtTableSelectionManager;
class QtTableSelectionRange;
class QtGraphicsTableView;
class QtGraphicsTableViewPrivate;
class QtGraphicsTableViewItemPrivate;
class QtTableOption;

class QPointF;
class QPainter;

class Q_ITEMVIEWSNG_EXPORT QtGraphicsTableViewItem : public QGraphicsWidget
{
public:
    QtGraphicsTableViewItem(int row, int column, QtGraphicsTableView *view);
    ~QtGraphicsTableViewItem();

    void setCell(int row, int column);

    int row() const;
    int column() const;

    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual QSizeF sizeHint(int row, int column, const QStyleOptionViewItemV4 *option, Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void itemChanged(const QList<QByteArray> &roles = QList<QByteArray>());
    virtual void initStyleOption(QStyleOptionViewItemV4 *option) const;

    QHash<QByteArray, QVariant> data(const QList<QByteArray> &roles = QList<QByteArray>()) const;
    QtGraphicsTableView *view() const;

    void copyStyleOptionState(const QStyleOptionGraphicsItem *source, QStyleOptionViewItemV4 *dest);

protected:
    QtGraphicsTableViewItemPrivate *d_ptr;

private:
    Q_DISABLE_COPY(QtGraphicsTableViewItem)
    Q_DECLARE_PRIVATE(QtGraphicsTableViewItem)
};

class QtGraphicsTableViewItemCreatorBase
{
public:
    virtual ~QtGraphicsTableViewItemCreatorBase();
    virtual QGraphicsObject *create(int row, int column, QtGraphicsTableView *view) = 0;
    virtual QGraphicsObject *reassign(int row, int column, QGraphicsObject *item, QtGraphicsTableView *view) = 0;
    virtual void update(int row, int column, QGraphicsObject *item, const QList<QByteArray> &roles);
    virtual void recycle(QGraphicsObject *item);
};

template <class T>
class Q_ITEMVIEWSNG_EXPORT QtGraphicsTableViewItemCreator : public QtGraphicsTableViewItemCreatorBase
{
public:
    inline QGraphicsObject *create(int row, int column, QtGraphicsTableView *view) { return new T(row, column, view); }
    inline QGraphicsObject *reassign(int row, int column, QGraphicsObject *item, QtGraphicsTableView *view)
    {
        Q_UNUSED(view);
        static_cast<T*>(item)->setCell(row, column);
        return item;
    }
    inline void update(int row, int column, QGraphicsObject *item, const QList<QByteArray> &roles)
    {
        Q_UNUSED(row);
        Q_UNUSED(column);
        static_cast<T*>(item)->itemChanged(roles);
    }
};

class Q_ITEMVIEWSNG_EXPORT QtGraphicsTableView : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QtTableModelInterface* model READ model WRITE setModel) // ###
    Q_PROPERTY(int firstRow READ firstRow WRITE setFirstRow)
    Q_PROPERTY(int firstColumn READ firstColumn WRITE setFirstColumn)
    Q_PROPERTY(qreal horizontalOffset READ horizontalOffset WRITE setHorizontalOffset)
    Q_PROPERTY(qreal verticalOffset READ verticalOffset WRITE setVerticalOffset)
    Q_PROPERTY(Qt::TextElideMode textElideMode READ textElideMode WRITE setTextElideMode)
    Q_PROPERTY(QtGraphicsHeader* horizontalHeader READ horizontalHeader WRITE setHorizontalHeader)
    Q_PROPERTY(QtGraphicsHeader* verticalHeader READ verticalHeader WRITE setVerticalHeader)
    Q_PROPERTY(bool isGridShown READ isGridShown WRITE setGridShown)

public:
    explicit QtGraphicsTableView(QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~QtGraphicsTableView();

    QtTableController *controller() const;
    QtTableModelInterface *model() const;
    QtTableSelectionManager *selectionManager() const;

    QtGraphicsHeader *verticalHeader() const;
    void setVerticalHeader(QtGraphicsHeader *header);

    QtGraphicsHeader *horizontalHeader() const;
    void setHorizontalHeader(QtGraphicsHeader *header);

    Qt::TextElideMode textElideMode() const;
    void setTextElideMode(Qt::TextElideMode mode);

    bool isGridShown() const;

    qreal rowHeight(int row) const;
    void setRowHeight(int row, qreal height);

    qreal columnWidth(int column) const;
    void setColumnWidth(int column, qreal width);

    QSizeF cellSize(int row, int column) const;

    bool isRowHidden(int row) const;
    void setRowHidden(int row, bool hide = true);

    bool isColumnHidden(int column) const;
    void setColumnHidden(int column, bool hide = true);

    bool isRowMovable(int row) const;
    void setRowMovable(int row, bool movable = true);

    bool isColumnMovable(int column) const;
    void setColumnMovable(int column, bool movable = true);

    bool isRowClickable(int row) const;
    void setRowClickable(int row, bool clickable = true);

    bool isColumnClickable(int column) const;
    void setColumnClickable(int column, bool clickable = true);

    void moveRow(int from, int to);
    void moveColumn(int from, int to);

    int rowCount() const;
    int columnCount() const;

    int firstRow() const;
    int firstColumn() const;

    qreal horizontalOffset() const;
    qreal verticalOffset() const;

    int maximumFirstRow() const;
    int maximumFirstColumn() const;

    qreal maximumHorizontalOffset() const;
    qreal maximumVerticalOffset() const;

    QtGraphicsTableViewItemCreatorBase *itemCreator() const;
    void setItemCreator(QtGraphicsTableViewItemCreatorBase *creator);

    QGraphicsObject *itemForCell(int row, int column) const;

    virtual void doLayout();
    virtual QRectF cellGeometry(int row, int column) const;
    virtual int rowAt(const QPointF &position, const QTransform &transform = QTransform()) const;
    virtual int columnAt(const QPointF &position, const QTransform &transform = QTransform()) const;
    virtual QPointF mapToCell(int row, int column, const QPointF &position, const QTransform &transform) const;

    virtual void initStyleOption(QStyleOptionViewItemV4 *option) const;

public Q_SLOTS:
    void setGridShown(bool show);
    void setFirstRow(int row);
    void setFirstColumn(int column);
    void setHorizontalOffset(qreal offset);
    void setVerticalOffset(qreal offset);
    void updateLayout();

Q_SIGNALS:
    void firstRowChanged(int row);
    void firstColumnChanged(int column);
    void horizontalOffsetChanged(qreal offset);
    void verticalOffsetChanged(qreal offset);
    void horizontalHeaderChanged(QtGraphicsHeader*);
    void verticalHeaderChanged(QtGraphicsHeader*);
    
protected:
    QtGraphicsTableView(QtGraphicsTableViewPrivate&, QGraphicsWidget *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual bool event(QEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void setModel(QtTableModelInterface *model);
    void setSelectionManager(QtTableSelectionManager *selectionManager);
    void setController(QtTableController *controller);

protected:
    QtGraphicsTableViewPrivate *d_ptr;

private:
    friend class QtPrinterTableView;
    friend class QtTableController;
    friend class QtGraphicsTableViewItem;
    Q_DECLARE_PRIVATE(QtGraphicsTableView)
    Q_DISABLE_COPY(QtGraphicsTableView)
    Q_PRIVATE_SLOT(d_func(), void _q_controllerDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_verticalHeaderDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_horizontalHeaderDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_cellsChanged(int firstRow, int firstColumn, int rowCount, int columnCount, const QList<QByteArray> &roles))
    Q_PRIVATE_SLOT(d_func(), void _q_selectionsChanged(const QList<QtTableSelectionRange> &change))
    Q_PRIVATE_SLOT(d_func(), void _q_currentChanged(int currentRow, int currentColumn, int previousRow, int previousColumn))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsInserted(int row, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsRemoved(int row, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsMoved(int from, int to, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsInserted(int column, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsRemoved(int column, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_columnsMoved(int from, int to, int count))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTGRAPHICSTABLEVIEW_H
