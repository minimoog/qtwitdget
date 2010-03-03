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

#include "qgraphicstableview.h"
#include "qgraphicstableview_p.h"

#include "qtablemodelinterface.h"
#include "qgraphicsheader.h"
#include "qtablecontroller.h"

#include <qdebug.h>
#include <qstack.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <qevent.h>

#include <qgraphicssceneevent.h>

QT_BEGIN_NAMESPACE

// QtGraphicsTableViewItem

QtGraphicsTableViewItem::QtGraphicsTableViewItem(int row, int column, QtGraphicsTableView *view)
    : QGraphicsWidget(view, 0),
    d_ptr(new QtGraphicsTableViewItemPrivate)
{
    Q_D(QtGraphicsTableViewItem);
    d->q_ptr = this;
    Q_ASSERT(view);
    d->view = view;
    d->row = row;
    d->column = column;
}

QtGraphicsTableViewItem::~QtGraphicsTableViewItem()
{
    delete d_ptr;
}

void QtGraphicsTableViewItem::setCell(int row, int column)
{
    Q_D(QtGraphicsTableViewItem);
    if (d->row != row || d->column != column) {
        d->row = row;
        d->column = column;
        itemChanged(QList<QByteArray>());
    }
}

int QtGraphicsTableViewItem::row() const
{
    Q_D(const QtGraphicsTableViewItem);
    return d->row;
}

int QtGraphicsTableViewItem::column() const
{
    Q_D(const QtGraphicsTableViewItem);
    return d->column;
}

QSizeF QtGraphicsTableViewItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const QtGraphicsTableViewItem);
    d->view->initStyleOption(&d->option);
    initStyleOption(&d->option);
    return sizeHint(d->row, d->column, &d->option, which, constraint);
}

QSizeF QtGraphicsTableViewItem::sizeHint(int row, int column, const QStyleOptionViewItemV4 *option, Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const QtGraphicsTableViewItem);
    switch (which) {
    case Qt::MinimumSize:
        return QSizeF(1, 1);
    case Qt::PreferredSize: {
        const QVariant value = d->view->d_func()->cachedData(row, column, "SizeHintRole");
        if (value.isValid())
            return qvariant_cast<QSizeF>(value);
        return d->view->style()->sizeFromContents(QStyle::CT_ItemViewItem, option, QSize(), option->widget);
    }
    case Qt::MaximumSize:
        return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    default:
        break;
    }
    return QGraphicsWidget::sizeHint(which, constraint);
}

void QtGraphicsTableViewItem::itemChanged(const QList<QByteArray> &roles)
{
    Q_UNUSED(roles);
    // ### if the size changed, we need to let the layout know
    //prepareGeometryChange();
    //update();
}

QHash<QByteArray, QVariant> QtGraphicsTableViewItem::data(const QList<QByteArray> &roles) const
{
    Q_D(const QtGraphicsTableViewItem);
    if (d->view && d->view->model())
        return d->view->model()->data(d->row, d->column, roles);
    return QHash<QByteArray, QVariant>();
}

QtGraphicsTableView *QtGraphicsTableViewItem::view() const
{
    Q_D(const QtGraphicsTableViewItem);
    return d->view;
}

void QtGraphicsTableViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_D(QtGraphicsTableViewItem);
    // draw content
    d->view->initStyleOption(&d->option);
    copyStyleOptionState(option, &d->option);
    initStyleOption(&d->option);
    const QRect rect = QRectF(QPointF(), size()).toRect();
    d->option.rect = rect;
    // ### FIXME: allow text to be drawn outside the item
    style()->drawControl(QStyle::CE_ItemViewItem, &d->option, painter, widget);
    // draw grid
    const QColor gridColor = static_cast<QRgb>(style()->styleHint(QStyle::SH_Table_GridLineColor, &d->option, widget));
    // left
    //painter->fillRect(rect.left(), rect.bottom(), rect.width(), 1, gridColor);
    // top
    //painter->fillRect(right(), d->option.rect.top(), 1, rect.height(), gridColor);
    // right // ### FIXME: if text overflows, don't draw this border
    painter->fillRect(rect.left(), rect.bottom(), rect.width(), 1, gridColor);
    // bottom
    painter->fillRect(rect.right(), rect.top(), 1, rect.height(), gridColor);
}

/*!
 */
void QtGraphicsTableViewItem::initStyleOption(QStyleOptionViewItemV4 *option) const
{
    Q_D(const QtGraphicsTableViewItem);
    Q_ASSERT(option);

    option->rect = QRect();
    option->features = 0;

// FIXME:
    if (d->view->d_func()->isCurrent(d->row, d->column))
        option->state |= QStyle::State_HasFocus;
    else
        option->state &= ~QStyle::State_HasFocus;

    if (d->view->d_func()->isSelected(d->row, d->column))
        option->state |= QStyle::State_Selected;
    else
        option->state &= ~QStyle::State_Selected;
/*
    if (d->view->d_func()->mouseOver(d->row, d->column))
        opt.state |= QStyle::State_MouseOver;
    else
        opt.state &= ~QStyle::State_MouseOver;
*/

    option->rect = QRect();
    option->features = 0;

    QVariant value;

    // FontRole
    value = d->view->d_func()->cachedData(d->row, d->column, "FontRole");
    if (value.isValid()) {
        option->font = qvariant_cast<QFont>(value).resolve(option->font);
        option->fontMetrics = QFontMetrics(option->font);
    } else {
        option->font = QFont();
    }

    // TextAlignmentRole
    value = d->view->d_func()->cachedData(d->row, d->column, "TextAlignmentRole");
    if (value.isValid())
        option->displayAlignment = static_cast<Qt::Alignment>(value.toInt());
    else
        option->displayAlignment = Qt::AlignLeft|Qt::AlignVCenter;

    // ForegroundRole
    value = d->view->d_func()->cachedData(d->row, d->column, "ForegroundRole");
    if (qVariantCanConvert<QBrush>(value))
        option->palette.setBrush(QPalette::Text, qvariant_cast<QBrush>(value));
    else
        option->palette = QPalette();

    // CheckStateRole
    value = d->view->d_func()->cachedData(d->row, d->column, "CheckStateRole");
    if (value.isValid()) {
        option->features |= QStyleOptionViewItemV2::HasCheckIndicator;
        option->checkState = static_cast<Qt::CheckState>(value.toInt());
    } else {
        option->checkState = Qt::Unchecked;
    }

    // DecorationRole
    value = d->view->d_func()->cachedData(d->row, d->column, "DecorationRole");
    if (value.isValid()) {
        option->features |= QStyleOptionViewItemV2::HasDecoration;
    switch (value.type()) {
    case QVariant::Icon:
        option->icon = qvariant_cast<QIcon>(value);
        break;
    case QVariant::Color: {
        QPixmap pixmap(option->decorationSize);
        pixmap.fill(qvariant_cast<QColor>(value));
        option->icon = QIcon(pixmap); }
    default:
            break;
    }
    } else {
        option->icon = QIcon();
    }

    // DisplayRole
    value = d->view->d_func()->cachedData(d->row, d->column, "DisplayRole");
    if (value.isValid()) {
        option->features |= QStyleOptionViewItemV2::HasDisplay;
        switch (value.type()) {
        case QVariant::Double:
            option->text = option->locale.toString(value.toDouble());
            break;
        case QVariant::Int:
        case QVariant::LongLong:
            option->text = option->locale.toString(value.toLongLong());
            break;
        case QVariant::UInt:
        case QVariant::ULongLong:
            option->text = option->locale.toString(value.toULongLong());
            break;
        case QVariant::Date:
            option->text = option->locale.toString(value.toDate(), QLocale::ShortFormat);
            break;
        case QVariant::Time:
            option->text = option->locale.toString(value.toTime(), QLocale::ShortFormat);
            break;
        case QVariant::DateTime:
            option->text = option->locale.toString(value.toDateTime().date(), QLocale::ShortFormat);
            option->text += QLatin1Char(' ');
            option->text += option->locale.toString(value.toDateTime().time(), QLocale::ShortFormat);
            break;
        default: {
            QString text = value.toString();
            for (int i = 0; i < text.count(); ++i)
                if (text.at(i).unicode() == '\n')
                    text[i] = QChar::LineSeparator;
            option->text = text;
            break; }
        }
    } else {
        option->text = QString();
    }

    if (d->view->d_func()->showGrid) {
        // ### FIXME: create and set a flag for each of the sides of the cell to paint the grid lines.
    }
}

/*!
 */
void QtGraphicsTableViewItem::copyStyleOptionState(const QStyleOptionGraphicsItem *source, QStyleOptionViewItemV4 *destination)
{
    if (source && destination) {
        destination->state = source->state;
        destination->direction = source->direction;
        destination->rect = source->rect;
        destination->fontMetrics = source->fontMetrics;
        destination->palette = source->palette;
    }
}

/*!
  \class QtGraphicsTableViewItemCreatorBase
 */

// QtGraphicsTableViewItemCreatorBase

QtGraphicsTableViewItemCreatorBase::~QtGraphicsTableViewItemCreatorBase()
{
}

void QtGraphicsTableViewItemCreatorBase::update(int row, int column, QGraphicsObject *item, const QList<QByteArray> &roles)
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(roles);
    Q_ASSERT(item);
    item->update();
}

void QtGraphicsTableViewItemCreatorBase::recycle(QGraphicsObject *item)
{
    delete item;
}

/*!
  \class QtGraphicsListViewItemCreator
 */

// QtGraphicsTableViewPrivate

QtGraphicsTableViewPrivate::QtGraphicsTableViewPrivate()
    : controller(0),
      model(0),
      selectionManager(0),
      horizontalHeader(0),
      verticalHeader(0),
      textElideMode(Qt::ElideMiddle),
      defaultRowHeight(30),
      defaultColumnWidth(100),
      firstRow(0),
      firstColumn(0),
      horizontalOffset(0),
      verticalOffset(0),
      showGrid(true),
      cachedDataRow(-1),
      cachedDataColumn(-1),
      creator(new QtGraphicsTableViewItemCreator<QtGraphicsTableViewItem>()),
      q_ptr(0)
{
}

QtGraphicsTableViewPrivate::~QtGraphicsTableViewPrivate()
{
}

void QtGraphicsTableViewPrivate::_q_controllerDestroyed()
{
    controller = 0;
}

void QtGraphicsTableViewPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtGraphicsTableViewPrivate::_q_selectionsDestroyed()
{
    selectionManager = 0;
}

void QtGraphicsTableViewPrivate::_q_verticalHeaderDestroyed()
{
    verticalHeader = 0;
}

void QtGraphicsTableViewPrivate::_q_horizontalHeaderDestroyed()
{
    horizontalHeader = 0;
}

void QtGraphicsTableViewPrivate::_q_cellsChanged(int firstRow, int firstColumn, int rowCount, int columnCount, const QList<QByteArray> &roles)
{
    Q_UNUSED(roles);
    // ### check if the headers need resizing
    // ### update the cells
    checkCache(firstRow, firstColumn, rowCount, columnCount);
}

void QtGraphicsTableViewPrivate::_q_selectionsChanged(const QList<QtTableSelectionRange> &changed)
{
    // ### FIXME: naive lookup
    // ### FIXME: check header mappings
    if (!model)
        return;
    const int columnCount = model->columnCount();
    for (int j = 0; j < changed.count(); ++j) {
        QtTableSelectionRange range = changed.at(j);
        if (range.intersects(visibleSections)) {
            for (int row = range.topRow(); row <= range.bottomRow(); ++row) {
                for (int column = range.leftColumn(); column <= range.rightColumn(); ++column) {
                    if (QGraphicsObject *item = items.value((row * columnCount) + column))
                        item->update();
                }
            }
        }
    }
}

void QtGraphicsTableViewPrivate::_q_currentChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if (!model)
        return;
    const int columnCount = model->columnCount();
    if (QGraphicsObject *current = items.value((currentRow * columnCount) + currentColumn))
        current->update();
    if (QGraphicsObject *previous = items.value((previousRow * columnCount) + previousColumn))
        previous->update();
}

void QtGraphicsTableViewPrivate::_q_rowsInserted(int row, int count)
{
    Q_UNUSED(count);
    if (verticalHeader)
        verticalHeader->setSectionCount(model ? model->rowCount() : 0);
    checkCache(row, -1, -1, -1);
    checkItems(row, -1, -1, -1);
}

void QtGraphicsTableViewPrivate::_q_rowsRemoved(int row, int count)
{
    Q_UNUSED(count);
    if (verticalHeader)
        verticalHeader->setSectionCount(model ? model->rowCount() : 0);
    checkCache(row, -1, -1, -1);
}

void QtGraphicsTableViewPrivate::_q_rowsMoved(int from, int to, int count)
{
    // ### update
    checkCache(qMin(from ,to), -1, qMax(from, to) + count, -1);
    checkItems(qMin(from ,to), -1, qMax(from, to) + count, -1);
}

void QtGraphicsTableViewPrivate::_q_columnsInserted(int column, int count)
{
    Q_UNUSED(count);
    if (horizontalHeader)
        horizontalHeader->setSectionCount(model ? model->columnCount() : 0);
    checkCache(-1, column, -1, -1);
    checkItems(-1, column, -1, -1);
}

void QtGraphicsTableViewPrivate::_q_columnsRemoved(int column, int count)
{
    Q_UNUSED(count);
    if (horizontalHeader)
        horizontalHeader->setSectionCount(model ? model->rowCount() : 0);
    checkCache(-1, column, -1, -1);
    checkItems(-1, column, -1, -1);
}

void QtGraphicsTableViewPrivate::_q_columnsMoved(int from, int to, int count)
{
    // ### update
    checkCache(-1, qMin(from ,to), -1, qMax(from, to) + count);
    checkItems(-1, qMin(from ,to), -1, qMax(from, to) + count);
}

bool QtGraphicsTableViewPrivate::isSelected(int row, int column) const
{
    return selectionManager ? selectionManager->isSelected(row, column) : false;
}

bool QtGraphicsTableViewPrivate::isCurrent(int row, int column) const
{
    if (selectionManager) {
        QPair<int,int> current = selectionManager->currentCell();
        return current.first == row && current.second == column;
    }
    return false;
}

void QtGraphicsTableViewPrivate::checkItems(int firstRow, int firstColumn, int rowCount, int columnCount)
{
    Q_UNUSED(firstRow);
    Q_UNUSED(firstColumn);
    Q_UNUSED(rowCount);
    Q_UNUSED(columnCount);
    QHash<qint64, QGraphicsObject*>::iterator it = items.begin();
    while (it != items.end()) {
        creator->recycle(it.value());
        ++it;
    }
    items.clear();
}

void QtGraphicsTableViewPrivate::checkCache(int firstRow, int firstColumn, int rowCount, int columnCount)
{
    Q_UNUSED(firstRow);
    Q_UNUSED(firstColumn);
    Q_UNUSED(rowCount);
    Q_UNUSED(columnCount);
    cachedDataRow = -1;
    cachedDataColumn = -1;
    cachedDataHash.clear();
}

QVariant QtGraphicsTableViewPrivate::cachedData(int row, int column, const QByteArray &role) const
{
    // ### structure this list so the most used items are added first
    static const QList<QByteArray> roles = (QList<QByteArray>()
                                             << "FontRole"
                                             << "TextAlignmentRole"
                                             << "ForegroundRole"
                                             << "CheckStateRole"
                                             << "DecorationRole"
                                             << "DisplayRole"
                                             << "EditRole");
    if (!model)
        return QVariant();
    if (row != cachedDataRow || column != cachedDataColumn) {
        cachedDataHash = model->data(row, column, roles);
        cachedDataRow = row;
        cachedDataColumn = column;
    }
    return cachedDataHash.value(role);
}

// QtGraphicsTableView

/*!
  \class QtGraphicsTableView
  \brief This class displays data in a table.

  The QtGraphicsTableView is meant to be used together with a QtTableController and a QtTableModelInterface
*/

/*!
 */
QtGraphicsTableView::QtGraphicsTableView(QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(new QtGraphicsTableViewPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtGraphicsTableView::QtGraphicsTableView(QtGraphicsTableViewPrivate &dd, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtGraphicsTableView::~QtGraphicsTableView()
{
    delete d_ptr;
}

/*!
 */
QtTableController *QtGraphicsTableView::controller() const
{
    Q_D(const QtGraphicsTableView);
    return d->controller;
}

/*!
 */
void QtGraphicsTableView::setController(QtTableController *controller)
{
    Q_D(QtGraphicsTableView);
    if (d->controller == controller)
        return;
    if (d->controller) {
        QObject::disconnect(d->controller, SIGNAL(destroyed()),
                         this, SLOT(_q_controllerDestroyed()));
    }
    d->controller = controller;
    if (d->controller) {
        QObject::connect(d->controller, SIGNAL(destroyed()),
                         this, SLOT(_q_controllerDestroyed()));
    }
    updateLayout();
}

/*!
 */
QtTableModelInterface *QtGraphicsTableView::model() const
{
    Q_D(const QtGraphicsTableView);
    return d->model;
}

/*!
 */
void QtGraphicsTableView::setModel(QtTableModelInterface *model)
{
    Q_D(QtGraphicsTableView);
    if (d->model == model)
        return;
    if (d->model) {
        QObject::disconnect(d->model, SIGNAL(destroyed()),
                            this, SLOT(_q_modelDestroyed()));
        QObject::disconnect(d->model, SIGNAL(cellsChanged(int,int,int,int,const QList<QByteArray>&)),
                            this, SLOT(_q_cellsChanged(int,int,int,int,const QList<QByteArray>&)));
        QObject::disconnect(d->model, SIGNAL(rowsInserted(int,int)),
                            this, SLOT(_q_rowsInserted(int,int)));
        QObject::disconnect(d->model, SIGNAL(rowsRemoved(int,int)),
                            this, SLOT(_q_rowsRemoved(int,int)));
        QObject::disconnect(d->model, SIGNAL(rowsMoved(int,int,int)),
                            this, SLOT(_q_rowsMoved(int,int,int)));
        QObject::disconnect(d->model, SIGNAL(columnsInserted(int,int)),
                            this, SLOT(_q_columnsInserted(int,int)));
        QObject::disconnect(d->model, SIGNAL(columnsRemoved(int,int)),
                            this, SLOT(_q_columnsRemoved(int,int)));
        QObject::disconnect(d->model, SIGNAL(columnsMoved(int,int,int)),
                            this, SLOT(_q_columnsMoved(int,int,int)));
    }
    d->model = model;
    if (d->model) {
        QObject::connect(d->model, SIGNAL(destroyed()),
                         this, SLOT(_q_modelDestroyed()));
        QObject::connect(d->model, SIGNAL(cellsChanged(int,int,int,int,const QList<QByteArray>&)),
                         this, SLOT(_q_cellsChanged(int,int,int,int,const QList<QByteArray>&)));
        QObject::connect(d->model, SIGNAL(rowsInserted(int,int)),
                         this, SLOT(_q_rowsInserted(int,int)));
        QObject::connect(d->model, SIGNAL(rowsRemoved(int,int)),
                         this, SLOT(_q_rowsRemoved(int,int)));
        QObject::connect(d->model, SIGNAL(rowsMoved(int,int,int)),
                         this, SLOT(_q_rowsMoved(int,int,int)));
        QObject::connect(d->model, SIGNAL(columnsInserted(int,int)),
                         this, SLOT(_q_columnsInserted(int,int)));
        QObject::connect(d->model, SIGNAL(columnsRemoved(int,int)),
                         this, SLOT(_q_columnsRemoved(int,int)));
        QObject::connect(d->model, SIGNAL(columnsMoved(int,int,int)),
                         this, SLOT(_q_columnsMoved(int,int,int)));
    }
    updateLayout();
}

/*!
 */
QtTableSelectionManager *QtGraphicsTableView::selectionManager() const
{
    Q_D(const QtGraphicsTableView);
    return d->selectionManager;
}

/*!
 */
void QtGraphicsTableView::setSelectionManager(QtTableSelectionManager *selectionManager)
{
    Q_D(QtGraphicsTableView);
    if (d->selectionManager == selectionManager)
        return;
    if (d->selectionManager) {
        disconnect(d->selectionManager, SIGNAL(destroyed()),
                   this, SLOT(_q_selectionsDestroyed()));
        disconnect(d->selectionManager, SIGNAL(selectionsChanged(const QList<QtTableSelectionRange>&)),
                   this, SLOT(_q_selectionsChanged(const QList<QtTableSelectionRange>&)));
        disconnect(d->selectionManager, SIGNAL(currentChanged(int,int,int,int)),
                   this, SLOT(_q_currentChanged(int,int,int,int)));
    }
    d->selectionManager = selectionManager;
    if (d->selectionManager) {
        connect(d->selectionManager, SIGNAL(destroyed()),
                this, SLOT(_q_selectionsDestroyed()));
        connect(d->selectionManager, SIGNAL(selectionsChanged(const QList<QtTableSelectionRange>&)),
                this, SLOT(_q_selectionsChanged(const QList<QtTableSelectionRange>&)));
        connect(d->selectionManager, SIGNAL(currentChanged(int,int,int,int)),
                this, SLOT(_q_currentChanged(int,int,int,int)));
    }
}

/*!
 */
QtGraphicsHeader *QtGraphicsTableView::verticalHeader() const
{
    Q_D(const QtGraphicsTableView);
    return d->verticalHeader;
}

/*!
 */
void QtGraphicsTableView::setVerticalHeader(QtGraphicsHeader *header)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader == header)
        return;
    if (d->verticalHeader) {
        disconnect(d->verticalHeader, SIGNAL(destroyed()),
                   this, SLOT(_q_verticalHeaderDestroyed()));
    }
    d->verticalHeader = header;
    if (d->verticalHeader) {
        connect(d->verticalHeader, SIGNAL(destroyed()),
                this, SLOT(_q_verticalHeaderDestroyed()));
    }
    updateLayout();
}

/*!
 */
QtGraphicsHeader *QtGraphicsTableView::horizontalHeader() const
{
    Q_D(const QtGraphicsTableView);
    return d->horizontalHeader;
}

/*!
 */
void QtGraphicsTableView::setHorizontalHeader(QtGraphicsHeader *header)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader == header)
        return;
    if (d->horizontalHeader) {
        disconnect(d->horizontalHeader, SIGNAL(destroyed()),
                   this, SLOT(_q_horizontalHeaderDestroyed()));
    }
    d->horizontalHeader = header;
    if (d->horizontalHeader) {
        connect(d->horizontalHeader, SIGNAL(destroyed()),
                this, SLOT(_q_horizontalHeaderDestroyed()));
    }
    updateLayout();
}

/*!
 */
Qt::TextElideMode QtGraphicsTableView::textElideMode() const
{
    Q_D(const QtGraphicsTableView);
    return d->textElideMode;
}

/*!
 */
void QtGraphicsTableView::setTextElideMode(Qt::TextElideMode mode)
{
    Q_D(QtGraphicsTableView);
    d->textElideMode = mode;

    updateLayout();
}

/*!
 */
bool QtGraphicsTableView::isGridShown() const
{
    Q_D(const QtGraphicsTableView);
    return d->showGrid;
}
/*!
 */
void QtGraphicsTableView::setGridShown(bool show)
{
    Q_D(QtGraphicsTableView);
    if (d->showGrid == show)
        return;
    d->showGrid = show;
}

/*!
  Returns the heigth of the given logical \a row.
 */
qreal QtGraphicsTableView::rowHeight(int row) const
{
    Q_D(const QtGraphicsTableView);
    return d->verticalHeader ? d->verticalHeader->sectionSize(row) : d->defaultRowHeight;
}

/*!
  Sets the height of the given logical \a row to the given \a height.
 */
void QtGraphicsTableView::setRowHeight(int row, qreal height)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader)
        d->verticalHeader->setSectionSize(row, height);
    updateLayout();
}

/*!
  Returns the width of the given logical \a column.
 */
qreal QtGraphicsTableView::columnWidth(int column) const
{
    Q_D(const QtGraphicsTableView);
    return d->horizontalHeader ? d->horizontalHeader->sectionSize(column) : d->defaultColumnWidth;
}

/*!
  Sets the width of the given logical \a column to the given \a width.
 */
void QtGraphicsTableView::setColumnWidth(int column, qreal width)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader)
        d->horizontalHeader->setSectionSize(column, width);
    updateLayout();
}

/*!
  Returns the size of the cell indicated by the given logical
  \a row and \a column.
 */
QSizeF QtGraphicsTableView::cellSize(int row, int column) const
{
    return QSizeF(columnWidth(row), rowHeight(column));
}

/*!
  Returns true if the given logical \a row is hidden, otherwise returns false.
 */
bool QtGraphicsTableView::isRowHidden(int row) const
{
    Q_D(const QtGraphicsTableView);
    return d->verticalHeader ? d->verticalHeader->isSectionHidden(row) : false;
}

/*!
  Hides the given logical \a row to be hidden if the given
  value \a hide is true, otherwise the row is shown.
 */
void QtGraphicsTableView::setRowHidden(int row, bool hide)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader)
        d->verticalHeader->setSectionHidden(row, hide);
    updateLayout();
}

/*!
  Returns true if the given logical \a column is hidden, otherwise returns false.
 */
bool QtGraphicsTableView::isColumnHidden(int column) const
{
    Q_D(const QtGraphicsTableView);
    return d->horizontalHeader ? d->horizontalHeader->isSectionHidden(column) : false;
}

/*!
  Hides the given logical \a column to be hidden if the given
  value \a hide is true, otherwise the column is shown.
 */
void QtGraphicsTableView::setColumnHidden(int column, bool hide)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader)
        d->horizontalHeader->setSectionHidden(column, hide);
    updateLayout();
}

/*!
 */
bool QtGraphicsTableView::isRowMovable(int row) const
{
    Q_D(const QtGraphicsTableView);
    return d->verticalHeader ? d->verticalHeader->isSectionMovable(row) : false;
}

/*!
 */
void QtGraphicsTableView::setRowMovable(int row, bool movable)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader)
        d->verticalHeader->setSectionMovable(row, movable);
}

/*!
 */
bool QtGraphicsTableView::isColumnMovable(int column) const
{
    Q_D(const QtGraphicsTableView);
    return d->horizontalHeader ? d->horizontalHeader->isSectionMovable(column) : false;
}

/*!
 */
void QtGraphicsTableView::setColumnMovable(int column, bool movable)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader)
        d->horizontalHeader->setSectionMovable(column, movable);
}

/*!
 */
bool QtGraphicsTableView::isRowClickable(int row) const
{
    Q_D(const QtGraphicsTableView);
    return d->verticalHeader ? d->verticalHeader->isSectionClickable(row) : false;
}

/*!
 */
void QtGraphicsTableView::setRowClickable(int row, bool clickable)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader)
        d->verticalHeader->setSectionClickable(row, clickable);
}

/*!
 */
bool QtGraphicsTableView::isColumnClickable(int column) const
{
    Q_D(const QtGraphicsTableView);
    return d->horizontalHeader ? d->horizontalHeader->isSectionClickable(column) : false;
}

/*!
 */
void QtGraphicsTableView::setColumnClickable(int column, bool clickable)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader)
        d->horizontalHeader->setSectionClickable(column, clickable);
}

/*!
 */
void QtGraphicsTableView::moveRow(int from, int to)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader)
        d->verticalHeader->moveSection(from, to);
}

/*!
 */
void QtGraphicsTableView::moveColumn(int from, int to)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader)
        d->horizontalHeader->moveSection(from, to);
}

/*!
 */
int QtGraphicsTableView::rowCount() const
{
    Q_D(const QtGraphicsTableView);
    if (d->verticalHeader)
        return d->verticalHeader->sectionCount();
    return d->model ? d->model->rowCount() : 0;
}

/*!
 */
int QtGraphicsTableView::columnCount() const
{
    Q_D(const QtGraphicsTableView);
    if (d->horizontalHeader)
        return d->horizontalHeader->sectionCount();
    return d->model ? d->model->columnCount() : 0;
}

/*!
 */
int QtGraphicsTableView::firstRow() const
{
    Q_D(const QtGraphicsTableView);
    return d->verticalHeader ? d->verticalHeader->firstSection() : d->firstRow;
}

/*!
 */
void QtGraphicsTableView::setFirstRow(int row)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader)
        d->verticalHeader->setFirstSection(row);
    d->firstRow = row;
    updateLayout();
}

/*!
 */
int QtGraphicsTableView::firstColumn() const
{
    Q_D(const QtGraphicsTableView);
    return d->horizontalHeader ? d->horizontalHeader->firstSection() : d->firstColumn;
}

/*!
 */
void QtGraphicsTableView::setFirstColumn(int column)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader)
        d->horizontalHeader->setFirstSection(column);
    d->firstColumn = column;
    updateLayout();
}

/*!
 */
qreal QtGraphicsTableView::horizontalOffset() const
{
    Q_D(const QtGraphicsTableView);
    return d->horizontalHeader ? d->horizontalHeader->offset() : d->horizontalOffset;
}

/*!
 */
void QtGraphicsTableView::setHorizontalOffset(qreal offset)
{
    Q_D(QtGraphicsTableView);
    if (d->horizontalHeader)
        d->horizontalHeader->setOffset(offset);
    d->horizontalOffset = offset;
    updateLayout();
}

/*!
 */
qreal QtGraphicsTableView::verticalOffset() const
{
    Q_D(const QtGraphicsTableView);
    return d->verticalHeader ? d->verticalHeader->offset() : d->verticalOffset;
}

/*!
 */
void QtGraphicsTableView::setVerticalOffset(qreal offset)
{
    Q_D(QtGraphicsTableView);
    if (d->verticalHeader)
        d->verticalHeader->setOffset(offset);
    d->verticalOffset = offset;
    updateLayout();
}

/*!
 */
int QtGraphicsTableView::maximumFirstRow() const
{
    Q_D(const QtGraphicsTableView);
    if (d->verticalHeader)
        return d->verticalHeader->maximumFirstSection();
    if (d->model)
        return d->model->rowCount() - (geometry().height() / d->defaultRowHeight) + 1;
    return 0;
}

/*!
 */
int QtGraphicsTableView::maximumFirstColumn() const
{
    Q_D(const QtGraphicsTableView);
    if (d->horizontalHeader)
        return d->horizontalHeader->maximumFirstSection();
    if (d->model)
        return d->model->columnCount() - (geometry().width() / d->defaultColumnWidth) + 1;
    return 0;
}

/*!
 */
qreal QtGraphicsTableView::maximumHorizontalOffset() const
{
    Q_D(const QtGraphicsTableView);
    if (d->verticalHeader)
        return d->verticalHeader->maximumOffset();
    if (d->model)
        return (d->model->columnCount() * d->defaultColumnWidth) - geometry().width();
    return 0;
}

/*!
 */
qreal QtGraphicsTableView::maximumVerticalOffset() const
{
    Q_D(const QtGraphicsTableView);
    if (d->horizontalHeader)
        return d->horizontalHeader->maximumOffset();
    if (d->model)
        return (d->model->rowCount() * d->defaultRowHeight) - geometry().height();
    return 0;
}
/*!
 */
QtGraphicsTableViewItemCreatorBase *QtGraphicsTableView::itemCreator() const
{
    Q_D(const QtGraphicsTableView);
    return d->creator;
}

/*!
 */
void QtGraphicsTableView::setItemCreator(QtGraphicsTableViewItemCreatorBase *creator)
{
    Q_D(QtGraphicsTableView);
    if (d->creator)
        delete d->creator;
    d->creator = creator;
}

/*!
  Returns the visible item for the given \a row and \a column, or null if the item
  is not visible.
 */
QGraphicsObject *QtGraphicsTableView::itemForCell(int row, int column) const
{
    Q_D(const QtGraphicsTableView);
    const int columnCount = d->model ? d->model->columnCount() : 0;
    return d->items.value((row * columnCount) + column);
}

/*!
 */
void QtGraphicsTableView::updateLayout()
{
    // ### FIXME: do delayed layout
    doLayout();
}

/*!
 */
void QtGraphicsTableView::doLayout()
{
    Q_D(QtGraphicsTableView);
    if (d->model == 0)
        return;

    const QRectF area = boundingRect();
    if (area.isEmpty())
        return;

    const int columnCount = d->model->columnCount();
    const int rowCount = d->model->rowCount();

    int horizontalCount;
    qreal horizontalOffset;
    QVector<int> horizontalMapping;

    if (d->horizontalHeader) {
        horizontalCount = qMin(d->horizontalHeader->sectionCount(), d->horizontalHeader->visibleSectionCount(area.width()));
        horizontalOffset = d->horizontalHeader->offset();
        horizontalMapping = d->horizontalHeader->indexMapping();
    } else {
        horizontalCount = qMin(columnCount, int(area.width() / d->defaultColumnWidth));
        horizontalOffset = d->horizontalOffset;
    }

    int verticalCount;
    qreal verticalOffset;
    QVector<int> verticalMapping;

    if (d->verticalHeader) {
        verticalCount = qMin(d->verticalHeader->sectionCount(), d->verticalHeader->visibleSectionCount(area.height()));
        verticalOffset = d->verticalHeader->offset();
        verticalMapping = d->verticalHeader->indexMapping();
    } else {
        verticalCount = qMin(rowCount, int(area.height() / d->defaultRowHeight));
        verticalOffset = d->verticalOffset;
    }

    const int firstHorizontalSection = d->firstColumn;
    const int firstVerticalSection = d->firstRow;
    const int lastHorizontalSection = qMax(d->firstColumn + horizontalCount - 1, -1);
    const int lastVerticalSection = qMax(d->firstRow + verticalCount - 1, -1);

    const int oldFirstVerticalSection = d->visibleSections.topRow();
    const int oldFirstHorizontalSection = d->visibleSections.leftColumn();
    const int oldLastVerticalSection = d->visibleSections.bottomRow();
    const int oldLastHorizontalSection = d->visibleSections.rightColumn();

    // find unused items for reassignment
    QStack<QGraphicsObject*> unused;

    /*
    --------------
    |            |
    --------------
    |  |      |  |
    |  |      |  |
    |  |      |  |
    --------------
    |            |
    --------------
    */

    // top edge
    for (int verticalSection = oldFirstVerticalSection; verticalSection < firstVerticalSection; ++verticalSection) {
        const int row = verticalMapping.value(verticalSection, verticalSection);
        for (int horizontalSection = oldFirstHorizontalSection; horizontalSection <= oldLastHorizontalSection; ++horizontalSection) {
            const int column = horizontalMapping.value(horizontalSection, horizontalSection);
            const qint64 index = (row * columnCount) + column;
            if (QGraphicsObject *item = d->items.take(index))
                unused.push(item);
        }
    }
    // left edge
    for (int horizontalSection = oldFirstHorizontalSection; horizontalSection < firstHorizontalSection; ++horizontalSection) {
        int column = horizontalMapping.value(horizontalSection, horizontalSection);
        for (int verticalSection = firstVerticalSection; verticalSection <= lastVerticalSection; ++verticalSection) {
            const int row = verticalMapping.value(verticalSection, verticalSection);
            const qint64 index = (row * columnCount) + column;
            if (QGraphicsObject *item = d->items.take(index))
                unused.push(item);
        }
    }
    // bottom edge
    for (int verticalSection = lastVerticalSection + 1; verticalSection <= oldLastVerticalSection; ++verticalSection) {
        const int row = verticalMapping.value(verticalSection, verticalSection);
        for (int horizontalSection = oldFirstHorizontalSection; horizontalSection <= oldLastHorizontalSection; ++horizontalSection) {
            const int column = horizontalMapping.value(horizontalSection, horizontalSection);
            const qint64 index = (row * columnCount) + column;
            if (QGraphicsObject *item = d->items.take(index))
                unused.push(item);
        }
    }
    // right edge
    for (int horizontalSection = lastHorizontalSection + 1; horizontalSection <= oldLastHorizontalSection; ++horizontalSection) {
        int column = horizontalMapping.value(horizontalSection, horizontalSection);
        for (int verticalSection = firstVerticalSection; verticalSection <= lastVerticalSection; ++verticalSection) {
            const int row = verticalMapping.value(verticalSection, verticalSection);
            const qint64 index = (row * columnCount) + column;
            if (QGraphicsObject *item = d->items.take(index))
                unused.push(item);
        }
    }

    // move overlapping items
    qreal y = -verticalOffset;
    for (int verticalSection = firstVerticalSection; verticalSection <= lastVerticalSection; ++verticalSection) {
        const int row = verticalMapping.value(verticalSection, verticalSection);
        const qreal height = rowHeight(row);
        qreal x = -horizontalOffset;
        for (int horizontalSection = firstHorizontalSection; horizontalSection <= lastHorizontalSection; ++horizontalSection) {
            const int column = horizontalMapping.value(horizontalSection, horizontalSection);
            const qreal width = columnWidth(column);
            const qint64 index = (row * columnCount) + column;
            QGraphicsObject *item = d->items.value(index);
            if (!item) {
                if (unused.isEmpty())
                    item = d->creator->create(row, column, this);
                else
                    item = d->creator->reassign(row, column, unused.pop(), this);
                d->items.insert(index, item);
            }
            if (item->isWidget())
                static_cast<QGraphicsWidget*>(item)->setGeometry(x, y, width, height);
            else
                item->setPos(x, y);
            x += width;
        }
        y += height;
    }

    // update internal structures
    d->visibleSections.setRange(firstVerticalSection, firstHorizontalSection, lastVerticalSection, lastHorizontalSection);

    // recycle any unused items left
    for (int i = 0; i < unused.count(); ++i)
        d->creator->recycle(unused.at(i));

    Q_ASSERT(d->items.count() == (horizontalCount * verticalCount));
    //Q_ASSERT(d->items.count() == childItems().count());
}

/*!
  Returns the geometry of the cell referred to by
  the given \a row and \a column.
 */
QRectF QtGraphicsTableView::cellGeometry(int row, int column) const
{
    Q_D(const QtGraphicsTableView);
    QRectF geometry;
    if (d->horizontalHeader) {
        geometry.setX(d->horizontalHeader->sectionPosition(column));
        geometry.setWidth(d->horizontalHeader->sectionSize(column));
    } else {
        geometry.setX(d->defaultColumnWidth * column);
        geometry.setWidth(d->defaultColumnWidth);
    }
    if (d->verticalHeader) {
        geometry.setY(d->verticalHeader->sectionPosition(row));
        geometry.setHeight(d->verticalHeader->sectionSize(row));
    } else {
        geometry.setY(d->defaultRowHeight * row);
        geometry.setHeight(d->defaultRowHeight);
    }
    return geometry;
}

/*!
  Returns the row in the given \a position transformed using the given \a transform.
  \sa columnAt()
 */
int QtGraphicsTableView::rowAt(const QPointF &position, const QTransform &transform) const
{
    Q_D(const QtGraphicsTableView);
    return (d->verticalHeader
            ? d->verticalHeader->sectionAt(position, transform)
            : (position.y() / d->defaultRowHeight));
}

/*!
  Returns the column in the given \a position transformed using the given \a transform.
  \sa rowAt()
 */
int QtGraphicsTableView::columnAt(const QPointF &position, const QTransform &transform) const
{
    Q_D(const QtGraphicsTableView);
    return (d->horizontalHeader
            ? d->horizontalHeader->sectionAt(position, transform)
            : (position.x() / d->defaultColumnWidth));
}

/*!
  Returns the given \a position mapped to the local coordinates of the cell
  indicated by the give \a row and \a column, using the given \a transform.
 */
QPointF QtGraphicsTableView::mapToCell(int row, int column, const QPointF &position, const QTransform &transform) const
{
    Q_D(const QtGraphicsTableView);
    const qreal y = d->verticalHeader ? d->verticalHeader->sectionPosition(row) : d->defaultRowHeight * row;
    const qreal x = d->horizontalHeader ? d->horizontalHeader->sectionPosition(column) : d->defaultColumnWidth * column;
    const qreal h = d->horizontalHeader ? d->horizontalHeader->geometry().height() : d->defaultRowHeight;
    const qreal w = d->verticalHeader ? d->verticalHeader->geometry().width() : d->defaultColumnWidth;
    return transform.map(position) - QPointF(x, y) - QPointF(w, h);
}

/*!
 */
void QtGraphicsTableView::initStyleOption(QStyleOptionViewItemV4 *option) const
{
    Q_ASSERT(option);
    option->widget = 0;
    option->font = QApplication::font();
    option->fontMetrics = QFontMetrics(option->font);
    option->decorationPosition = QStyleOptionViewItem::Left;
    option->decorationAlignment = Qt::AlignCenter;
    //option->displayAlignment = Qt::AlignLeft|Qt::AlignVCenter;
    option->textElideMode = Qt::ElideNone; // d->textElideMode;
    option->showDecorationSelected = true; // always true in QtGraphicsTableView
    int pm = style()->pixelMetric(QStyle::PM_SmallIconSize);
    option->decorationSize = QSize(pm, pm);
    option->decorationPosition = QStyleOptionViewItem::Left;
    option->locale = QLocale(); // ### FIXME
    option->locale.setNumberOptions(QLocale::OmitGroupSeparator);
    //if (d->wrapText) option->features |= QStyleOptionViewItemV2::WrapText;
    //if (d->alternate) option->features |= QStyleOptionViewItemV2::Alternate;
    //option->grid = d->showGrid; // ### FIXME
    if (isActiveWindow())
        option->state |= QStyle::State_Active;
}


/*!
  \reimp
 */
bool QtGraphicsTableView::event(QEvent *event)
{
    Q_D(QtGraphicsTableView);
    if (d->controller && d->controller->processEvent(event, transform())) {
        event->accept();
        return true;
    }
    return QObject::event(event);
}

/*!
  \reimp
 */
QVariant QtGraphicsTableView::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(QtGraphicsTableView);
    if (change == QGraphicsItem::ItemChildRemovedChange) { // ### FIXME
        QGraphicsItem *item = qvariant_cast<QGraphicsItem*>(value);
        qint64 key = d->items.key(static_cast<QGraphicsObject*>(item), -1);
        if (key != -1)
            d->items.remove(key);
    }
    return QGraphicsWidget::itemChange(change, value);
}

QT_END_NAMESPACE

#include "moc_qgraphicstableview.cpp"
