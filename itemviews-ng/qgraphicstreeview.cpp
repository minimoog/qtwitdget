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

#include "qgraphicstreeview.h"
#include "qgraphicstreeview_p.h"
#include "qgraphicsheader.h"
#include "qtreecontroller.h"

#include <qdebug.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qstyleoption.h>
#include <qapplication.h>
#include <qdatetime.h>
#include <qevent.h>

#include <qgraphicssceneevent.h>

QT_BEGIN_NAMESPACE

// QtGraphicsTreeViewItem

/*!
  \class QtGraphicsTreeViewItem
 */

/*!
 */
QtGraphicsTreeViewItem::QtGraphicsTreeViewItem(const QtTreeModelIterator &it, QtGraphicsTreeView *view)
    : QGraphicsWidget(view, 0), d_ptr(new QtGraphicsTreeViewItemPrivate)
{
    Q_D(QtGraphicsTreeViewItem);
    Q_ASSERT(view);
    d->q_ptr = this;
    d->view = view;
    d->header = view->header();
    d->it = it;
}

/*!
 */
QtGraphicsTreeViewItem::~QtGraphicsTreeViewItem()
{
    delete d_ptr;
}

/*!
 */
QtTreeModelIterator QtGraphicsTreeViewItem::iterator() const
{
    Q_D(const QtGraphicsTreeViewItem);
    return d->it;
}

/*!
 */
void QtGraphicsTreeViewItem::setIterator(const QtTreeModelIterator &it)
{
    Q_D(QtGraphicsTreeViewItem);
    if (d->it != it) {
        d->it = it;
        update();
    }
}

/*!
 */
QSizeF QtGraphicsTreeViewItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const QtGraphicsTreeViewItem);
    d->view->initStyleOption(&d->option);
    switch (which) {
        case Qt::MinimumSize:
        case Qt::PreferredSize: {
            qreal width = 0;
            qreal height = 0;
            for (int column = 0; column < d->it.columnCount(); ++column) {
                initStyleOption(&d->option, column);
                const QSizeF size = sizeHint(column, &d->option, which, constraint);
                width += size.width();
                height = qMax(height, size.height());
            }
            return QSizeF(width, height);
        }
        case Qt::MaximumSize:
            return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        default:
            break;
    }
    return QGraphicsWidget::sizeHint(which, constraint);
}

/*!
 */
QSizeF QtGraphicsTreeViewItem::sizeHint(int column, const QStyleOptionViewItemV4 *option,
                                        Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const QtGraphicsTreeViewItem);
    Q_UNUSED(constraint);
    switch (which) {
        case Qt::MinimumSize:
        case Qt::PreferredSize:
        case Qt::MaximumSize: {
            const QVariant value = view()->d_func()->cachedData(d->it, column, "SizeHintRole");
            if (value.isValid())
                return qvariant_cast<QSizeF>(value);
            else
                return d->view->style()->sizeFromContents(QStyle::CT_ItemViewItem, option, QSize(), option->widget);
        }
        default:
            break;
    }
    return QSizeF();
}

/*!
 */
void QtGraphicsTreeViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(QtGraphicsTreeViewItem);
    Q_UNUSED(widget);
    // prepare depth - this can be cached
    QStack<QtTreeModelIterator> stack;
    for (QtTreeModelIterator it = d->it; it.isValid(); it.parent())
        stack.prepend(it);
    const int depth = stack.count();
    // draw stuff
    d->view->initStyleOption(&d->option);
    qreal x = 0;
    const QVector<int> &mapping = d->header ? d->header->indexMapping() : QVector<int>();
    const int count = d->header ? qMin(d->header->sectionCount(), d->it.columnCount()) : d->it.columnCount();
    // for each column
    for (int section = 0; section < count; ++section) {
        const int column = mapping.value(section, section);
        copyStyleOptionState(option, &d->option);
        initStyleOption(&d->option, column);
        QSizeF size = sizeHint(column, &d->option, Qt::PreferredSize, QSizeF());
        size.setWidth(d->header ? d->header->sectionSize(section) : d->view->geometry().width());
        if (column == 0) {
            qreal indent = depth * QtGraphicsTreeViewPrivate::indentation();
            size.setWidth(size.width() - indent);
            x += indent;
        }
        d->option.rect = QRect(QPoint(x, 0), size.toSize());
        style()->drawControl(QStyle::CE_ItemViewItem, &d->option, painter, widget);
        x += size.width();
    }
    paintBranches(painter, &d->option, stack);
}

/*!
 */
void QtGraphicsTreeViewItem::paintBranches(QPainter *painter, QStyleOptionViewItemV4 *option, const QStack<QtTreeModelIterator> &stack)
{
    Q_D(QtGraphicsTreeViewItem);
    const int depth = stack.count();
    // paint braches - this can be cached - this is incredibly slow when painting the dotted lines
    for (int i = 0; i < depth; ++i) {
        const QtTreeModelIterator &it = stack.at(i);
        initStyleOptionState(&d->option, it, i == depth - 1 ? 0 : -1);
        const qreal w = QtGraphicsTreeViewPrivate::indentation();
        option->rect = QRectF(i * w, 0, w, size().height()).toRect();
        style()->drawPrimitive(QStyle::PE_IndicatorBranch, option, painter, 0);
    }
}
/*!
 */
void QtGraphicsTreeViewItem::initStyleOption(QStyleOptionViewItemV4 *option, int column) const
{
    Q_D(const QtGraphicsTreeViewItem);
    Q_ASSERT(option);

    option->rect = QRect();
    option->features = 0;

    initStyleOptionState(option, d->it, column);

    QVariant value;

    // FontRole
    value = d->view->d_func()->cachedData(d->it, column, "FontRole");
    if (value.isValid()){
        option->font = qvariant_cast<QFont>(value).resolve(option->font);
        option->fontMetrics = QFontMetrics(option->font);
    }

    // TextAlignmentRole
    value = d->view->d_func()->cachedData(d->it, column, "TextAlignmentRole");
    if (value.isValid())
        option->displayAlignment = (Qt::Alignment)value.toInt();
    else
        option->displayAlignment = 0; // ### default value

    // ForegroundRole
    value = d->view->d_func()->cachedData(d->it, column, "ForegroundRole");
    if (qVariantCanConvert<QBrush>(value))
        option->palette.setBrush(QPalette::Text, qvariant_cast<QBrush>(value));
    // ### default value

    // CheckStateRole
    value = d->view->d_func()->cachedData(d->it, column, "CheckStateRole");
    if (value.isValid()) {
        option->features |= QStyleOptionViewItemV2::HasCheckIndicator;
        option->checkState = static_cast<Qt::CheckState>(value.toInt());
    }

    // DecorationRole
    value = d->view->d_func()->cachedData(d->it, column, "DecorationRole");
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
    }

    // DisplayRole
    value = d->view->d_func()->cachedData(d->it, column, "DisplayRole");
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
            const QChar nl = QLatin1Char('\n');
            for (int i = 0; i < text.count(); ++i)
                if (text.at(i) == nl)
                    text[i] = QChar::LineSeparator;
            option->text = text;
            break;}
        }
    }
}

/*!
 */
void QtGraphicsTreeViewItem::initStyleOptionState(QStyleOptionViewItemV4 *option, const QtTreeModelIterator &it, int column) const
{
    Q_D(const QtGraphicsTreeViewItem);
    Q_ASSERT(option);

    if (d->view->d_func()->isCurrent(it, column)) // ### currentColumn
        option->state |= QStyle::State_HasFocus;
    else
        option->state &= ~QStyle::State_HasFocus;

    if (d->view->d_func()->isSelected(it, column))
        option->state |= QStyle::State_Selected;
    else
        option->state &= ~QStyle::State_Selected;

    if (d->it.hasNextSibling())
        option->state |= QStyle::State_Sibling;
    else
        option->state &= ~QStyle::State_Sibling;

    // ### the column != -1 trick is a bit hacky...

    if (it.hasChildren() && column != -1)
        option->state |= QStyle::State_Children;
    else
        option->state &= ~QStyle::State_Children;

    if (d->view->d_func()->isExpanded(it) && column != -1)
        option->state |= QStyle::State_Open;
    else
        option->state &= ~QStyle::State_Open;

    if (column != -1)
        option->state |= QStyle::State_Item;
    else
        option->state &= ~QStyle::State_Item;
/*
    if (d->view->d_func()->mouseOver(it))
        opt.state |= QStyle::State_MouseOver;
    else
        opt.state &= ~QStyle::State_MouseOver;
*/
}

/*!
 */
void QtGraphicsTreeViewItem::copyStyleOptionState(const QStyleOptionGraphicsItem *source, QStyleOptionViewItemV4 *destination)
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
 */
void QtGraphicsTreeViewItem::itemChanged(const QList<QByteArray> &roles)
{
    Q_UNUSED(roles);
    prepareGeometryChange();
    update();
}

/*!
 */
QHash<QByteArray, QVariant> QtGraphicsTreeViewItem::data(int column, const QList<QByteArray> &roles) const
{
    Q_D(const QtGraphicsTreeViewItem);
    return d->it.data(column, roles);
}

/*!
 */
QtGraphicsTreeView *QtGraphicsTreeViewItem::view() const
{
    Q_D(const QtGraphicsTreeViewItem);
    return d->view;
}

QtGraphicsTreeViewItemCreatorBase::~QtGraphicsTreeViewItemCreatorBase()
{
}

void QtGraphicsTreeViewItemCreatorBase::update(const QtTreeModelIterator &it, QGraphicsObject *item, const QList<QByteArray> &roles)
{
    Q_UNUSED(it);
    Q_UNUSED(roles);
    Q_ASSERT(item);
    item->update();
}

void QtGraphicsTreeViewItemCreatorBase::recycle(QGraphicsObject *item)
{
    delete item;
}

// QtGraphicsTreeViewPrivate

QtGraphicsTreeViewPrivate::QtGraphicsTreeViewPrivate()
    : q_ptr(0),
      controller(0),
      model(0),
      selectionManager(0),
      header(0),
      textElideMode(Qt::ElideMiddle),
      cachedDataColumn(-1),
      verticalOffset(0),
      firstIndex(0),
      cachedFirstIndex(-1),
      cachedVerticalOffset(-1),
      creator(new QtGraphicsTreeViewItemCreator<QtGraphicsTreeViewItem>())
{
}

QtGraphicsTreeViewPrivate::~QtGraphicsTreeViewPrivate()
{
}

void QtGraphicsTreeViewPrivate::_q_controllerDestroyed()
{
    controller = 0;
}

void QtGraphicsTreeViewPrivate::_q_modelDestroyed()
{
    model = 0;
}

void QtGraphicsTreeViewPrivate::_q_selectionsDestroyed()
{
    selectionManager = 0;
}

void QtGraphicsTreeViewPrivate::_q_headerDestroyed()
{
    header = 0;
}

void QtGraphicsTreeViewPrivate::_q_itemsChanged(const QtTreeModelBase::iterator_base &it, int count, const QList<QByteArray> &roles)
{
    Q_UNUSED(it);
    Q_UNUSED(count);
    Q_UNUSED(roles);
    //qDebug() << "QtGraphicsTreeViewPrivate::_q_itemsChanged" << it << count;
    checkCache(it, count);
    checkMapping(it, count);
}

void QtGraphicsTreeViewPrivate::_q_itemsInserted(const QtTreeModelBase::iterator_base &it, int count)
{
    //Q_Q(QtGraphicsTreeView);
    //qDebug() << "QtGraphicsTreeViewPrivate::_q_itemsInserted" << it << count;
    checkCache(it, count);
    checkMapping(it, count);
    checkHeader(it, count);
    //q->updateLayout();
}

void QtGraphicsTreeViewPrivate::_q_itemsRemoved(const QtTreeModelBase::iterator_base &it, int count)
{
    //Q_Q(QtGraphicsTreeView);
    //qDebug() << "QtGraphicsTreeViewPrivate::_q_itemsRemoved" << it << count;
    checkCache(it, count);
    checkMapping(it, count);
    checkHeader(it, count);
    //q->updateLayout(); // delay it
}

void QtGraphicsTreeViewPrivate::_q_reset()
{
    Q_Q(QtGraphicsTreeView);
    //qDebug() << "QtGraphicsTreeViewPrivate::_q_reset";
    checkCache(QtTreeModelBase::iterator_base(), -1);
    checkMapping(QtTreeModelBase::iterator_base(), -1);
    checkHeader(QtTreeModelBase::iterator_base(), -1);
    q->updateLayout(); // delay it
}

void QtGraphicsTreeViewPrivate::_q_selectionsChanged(const QtTreeSelection &selection)
{
    //qDebug() << "QtGraphicsTreeViewPrivate::_q_selectionsChanged" << selection;
    // ### FIXME: it might be more efficient to iterate over the items instead
    QtTreeSelection::const_iterator it = selection.constBegin();
    for (; it != selection.constEnd(); ++it)
        updateItem(*it);
}

void QtGraphicsTreeViewPrivate::_q_currentChanged(const QtTreeModelBase::iterator_base &current, const QtTreeModelBase::iterator_base &previous)
{
    //qDebug() << "QtGraphicsTreeViewPrivate::_q_currentChanged" << current << previous;
    updateItem(current);
    updateItem(previous);
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::checkCache(const QtTreeModelBase::iterator_base &it, int count)
{
    Q_UNUSED(it);
    Q_UNUSED(count);
    // TODO: find out where the change happended
    // and only invalidate the cache if needed
    cachedDataColumn = -1;
    cachedDataItem = QtTreeModelBase::iterator_base();
    cachedDataHash.clear();
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::checkMapping(const QtTreeModelBase::iterator_base &it, int count)
{
    Q_UNUSED(it);
    Q_UNUSED(count);
    // TODO: find out where the change happended
    // and only invalidate the mapping if needed
    cachedFirstIndex = -1;
    cachedFirstItem = QtTreeModelBase::iterator_base();
    cachedVerticalOffset = -1;
    cachedFirstVisibleItem = QtTreeModelBase::iterator_base();
#if MAPPING_ENABLED
    mapIndexToItem.clear();
#endif
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::checkHeader(const QtTreeModelBase::iterator_base &it, int count)
{
    Q_UNUSED(it);
    Q_UNUSED(count);
    if (header)
        header->setSectionCount(model ? model->columnCount() : 1);
}

/*!
  \internal
 */
QVariant QtGraphicsTreeViewPrivate::cachedData(const QtTreeModelBase::iterator_base &it, int column, const QByteArray &role) const
{
    // ### structure this list so the most used items are added first
    static const QList<QByteArray> roles = (QList<QByteArray>()
                                             << "FontRole"
                                             << "TextAlignmentRole"
                                             << "ForegroundRole"
                                             << "CheckStateRole"
                                             << "DecorationRole"
                                             << "DisplayRole");
    if (it != cachedDataItem || column != cachedDataColumn) {
        cachedDataHash = it.data(column, roles);
        cachedDataItem = it;
        cachedDataColumn = column;
    }
    return cachedDataHash.value(role);
}

/*!
  \internal
 */
bool QtGraphicsTreeViewPrivate::isCurrent(const QtTreeModelBase::iterator_base &it, int column) const
{
    if (column != 0)
        return false;// ### FIXME
    return selectionManager ? (selectionManager->currentItem() == it) : false;
}

/*!
  \internal
 */
bool QtGraphicsTreeViewPrivate::isSelected(const QtTreeModelBase::iterator_base &it, int column) const
{
    Q_UNUSED(column);
    return selectionManager ? selectionManager->isSelected(it) : false;
}

/*!
  \internal
 */
bool QtGraphicsTreeViewPrivate::isExpanded(const QtTreeModelBase::iterator_base &it) const
{
    return expandedItems.contains(it);
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::expand(QtTreeModelBase::iterator_base &it)
{
    Q_Q(QtGraphicsTreeView);
    checkMapping(it, -1);
    // count before adding it to expandedItems
    int count = visibleChildCount(it);
    expandedItems.insert(it);
    q->updateLayout();
    emit q->itemExpanded(it, count);
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::collapse(QtTreeModelBase::iterator_base &it)
{
    Q_Q(QtGraphicsTreeView);
    checkMapping(it, -1);
    // count after removing it from expandedItems
    expandedItems.remove(it);
    int count = visibleChildCount(it);
    q->updateLayout();
    emit q->itemCollapsed(it, count);
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::syncStack(const QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const
{
    QtTreeModelBase::iterator_base item = it;
    stack.clear();
    while (item.parent().isValid())
        stack.prepend(item);
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::nextBranch(QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const
{
    // we use the stack instead if calling parent()
    // because parent is potentially expensive
    if (!stack.isEmpty()) {
        do {
            it = stack.pop();
        } while (!stack.isEmpty() && !it.hasNextSibling());
    } else {
        do {
            it = it.parent();
        } while (it.isValid() && !it.hasNextSibling());
    }
    if (it.isValid())
        it.nextSibling();
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::next(QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const
{
    // we use the stack instead if calling parent()
    // because parent is potentially expensive
    if (it.hasChildren() && isExpanded(it)) {
        stack.push(it);
        it.firstChild();
        if (!it.isValid())
            nextBranch(it, stack);
    } else if (it.hasNextSibling()) {
        it.nextSibling();
    } else {
        nextBranch(it, stack);
    }
}

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::previous(QtTreeModelBase::iterator_base &it, QStack<QtTreeModelBase::iterator_base> &stack) const
{
    // we use the stack instead if calling parent()
    // because parent is potentially expensive
    if (it.hasPreviousSibling()) {
        it.previousSibling();
        while (it.isValid() && it.hasChildren() && isExpanded(it)) {
            stack.push(it);
            it.firstChild();
            if (it.isValid()) {
                while (it.hasNextSibling())
                    it.nextSibling();
            } else {
                it = stack.pop();
                break;
            }
        }
    } else if (!stack.isEmpty()) {
        it = stack.pop();
    } else {
        it.parent();
    }
}

/*!
  \internal
 */
QtTreeModelBase::iterator_base QtGraphicsTreeViewPrivate::firstItem(QStack<QtTreeModelBase::iterator_base> &stack, int *flat) const
{
    // the easy case first
    if (firstIndex == 0) {
        if (flat)
            *flat = 0;
        return model->begin();
    }

    // note that it is important to keep the cached values in sync
    if (cachedFirstIndex == firstIndex && cachedFirstItem.isValid()) {
        syncStack(cachedFirstItem, stack);
        if (flat)
            *flat = cachedFirstIndex;
        return cachedFirstItem;
    }

    // start with the cached first index
    int index = cachedFirstIndex;
    QtTreeModelBase::iterator_base it = cachedFirstItem;
    if (!it.isValid() || index < 0) {
        index = 0;
        it = model->begin();
    }

#if MAPPING_ENABLED
    {
        int key = firstIndex - (firstIndex % mapIndexInterval);
        QtTreeModelBase::iterator_base value = mapIndexToItem.value(key);
        if (value.isValid()) {
            index = key;
            it = value;
        }
    }
#endif

    syncStack(it, stack);

    // ### this is where the bottleneck is; optimize here
    if (index < firstIndex) {
        while (index < firstIndex && it.isValid()) {
            next(it, stack);
            ++index;
#if MAPPING_ENABLED
            if (index % mapIndexInterval == 0)
                mapIndexToItem.insert(index, it);
#endif
        }
    } else {
        while (index > firstIndex && it.isValid()) {
            previous(it, stack);
            --index;
#if MAPPING_ENABLED
            if (index % mapIndexInterval == 0)
                mapIndexToItem.insert(index, it);
#endif
        }
    }

    cachedFirstItem = it;
    cachedFirstIndex = index;

    if (flat)
        *flat = index;

    return it;
}

/*!
  \internal
 */
qreal QtGraphicsTreeViewPrivate::itemHeight(QStyleOptionViewItemV4 &option, const QtTreeModelBase::iterator_base &it) const
{
    // ### move to protected ?
    Q_Q(const QtGraphicsTreeView);
    qreal height = 0;
    const int count = it.columnCount();
    for (int column = 0; column < count; ++column) {
        //#### FIXME: q->initStyleOption(&option, it, column);
        const QVariant value = cachedData(it, column, "SizeHintRole");
        const QSizeF size = (value.isValid() ? qvariant_cast<QSize>(value)
                              : q->style()->sizeFromContents(QStyle::CT_ItemViewItem, &option, QSize(), option.widget));
        height = qMax<qreal>(height, size.height());
    }
    return height;
}

/*!
  \internal
 */
qreal QtGraphicsTreeViewPrivate::columnWidth(int column) const
{
    Q_Q(const QtGraphicsTreeView);
    return header ? header->sectionSize(column) : q->geometry().width();
}

/*!
  \internal
 */
qreal QtGraphicsTreeViewPrivate::columnPosition(int column) const
{
    return header ? header->sectionPosition(column) : 0;
}

/*!
  \internal
 */
QRectF QtGraphicsTreeViewPrivate::disclosureRect(const QRectF &rect) const
{
    Q_Q(const QtGraphicsTreeView);
    QStyleOptionViewItemV4 option;
    q->initStyleOption(&option);
    option.rect = rect.toRect();
    option.rect.setWidth(int(indentation())); // FIXME: ### the style just returns the rect
    return q->style()->subElementRect(QStyle::SE_TreeViewDisclosureItem, &option, option.widget);
}

/*!
  \internal
 */
int QtGraphicsTreeViewPrivate::visibleChildCount(const QtTreeModelBase::iterator_base &it) const
{
    int count = 0;
    QStack<QtTreeModelBase::iterator_base> stack;
    // find start and stop items
    QtTreeModelBase::iterator_base child = it;
    child.firstChild();
    QtTreeModelBase::iterator_base sibling = it;
    //syncStack(sibling, stack);
    next(sibling, stack);
    // now count the visible children
    stack.clear();
    //syncStack(child, stack);
    while (child.isValid() && child != sibling) {
        next(child, stack);
        ++count;
    }
    return count;
}

/*!
  \internal

  This function can be run while the application is idle
  to cache the flat index to iterator mapping.
 */
#if MAPPING_ENABLED
void QtGraphicsTreeViewPrivate::_q_mapIndexesToItems()
{
    QStack<QtTreeModelBase::iterator_base> stack;
    int index = 0;
    QtTreeModelBase::iterator_base it = model->begin();
    while (it.isValid()) {
        next(it, stack);
        ++index;
        if (index % mapIndexInterval == 0)
            mapIndexToItem.insert(index, it);
    }
}
#endif

/*!
  \internal
 */
void QtGraphicsTreeViewPrivate::updateItem(const QtTreeModelBase::iterator_base &it) const
{
    // ### FIXME: remove this function
    if (QGraphicsObject *item = items.value(it))
        item->update();
}

// QtGraphicsTreeView

/*!
  \class QtGraphicsTreeView
  \brief A tree controller
*/

/*!
 */
QtGraphicsTreeView::QtGraphicsTreeView(QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(new QtGraphicsTreeViewPrivate)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtGraphicsTreeView::QtGraphicsTreeView(QtGraphicsTreeViewPrivate &dd, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

/*!
 */
QtGraphicsTreeView::~QtGraphicsTreeView()
{
    delete d_ptr;
}

/*!
 */
QtTreeController *QtGraphicsTreeView::controller() const
{
    Q_D(const QtGraphicsTreeView);
    return d->controller;
}

/*!
 */
void QtGraphicsTreeView::setController(QtTreeController *controller)
{
    Q_D(QtGraphicsTreeView);
    if (d->controller == controller)
        return;
    if (d->controller) {
        disconnect(d->controller, SIGNAL(destroyed()), this, SLOT(_q_controllerDestroyed()));
    }
    d->controller = controller;
    if (d->controller) {
        connect(d->controller, SIGNAL(destroyed()), this, SLOT(_q_controllerDestroyed()));
    }
}

/*!
 */
QtTreeModelBase *QtGraphicsTreeView::model() const
{
    Q_D(const QtGraphicsTreeView);
    return d->model;
}

/*!
 */
void QtGraphicsTreeView::setModel(QtTreeModelBase *model)
{
    Q_D(QtGraphicsTreeView);
    if (d->model == model)
        return;
    if (d->model) {
        disconnect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        disconnect(d->model, SIGNAL(itemsChanged(const QtTreeModelBase::iterator_base&,int,const QList<QByteArray>&)),
                   this, SLOT(_q_itemsChanged(const QtTreeModelBase::iterator_base&,int,const QList<QByteArray>&)));
        disconnect(d->model, SIGNAL(itemsInserted(const QtTreeModelBase::iterator_base&,int)),
                   this, SLOT(_q_itemsInserted(const QtTreeModelBase::iterator_base&,int)));
        disconnect(d->model, SIGNAL(itemsRemoved(const QtTreeModelBase::iterator_base&,int)),
                   this, SLOT(_q_itemsRemoved(const QtTreeModelBase::iterator_base&,int)));
    }
    d->model = model;
    if (d->model) {
        connect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        connect(d->model, SIGNAL(itemsChanged(const QtTreeModelBase::iterator_base&,int,const QList<QByteArray>&)),
                this, SLOT(_q_itemsChanged(const QtTreeModelBase::iterator_base&,int,const QList<QByteArray>&)));
        connect(d->model, SIGNAL(itemsInserted(const QtTreeModelBase::iterator_base&,int)),
                this, SLOT(_q_itemsInserted(const QtTreeModelBase::iterator_base&,int)));
        connect(d->model, SIGNAL(itemsRemoved(const QtTreeModelBase::iterator_base&,int)),
                this, SLOT(_q_itemsRemoved(const QtTreeModelBase::iterator_base&,int)));
    }
}

/*!
 */
QtTreeSelectionManager *QtGraphicsTreeView::selectionManager() const
{
    Q_D(const QtGraphicsTreeView);
    return d->selectionManager;
}

/*!
 */
void QtGraphicsTreeView::setSelectionManager(QtTreeSelectionManager *selectionManager)
{
    Q_D(QtGraphicsTreeView);
    if (d->selectionManager == selectionManager)
        return;
    if (d->selectionManager) {
        disconnect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));
        disconnect(d->selectionManager, SIGNAL(selectionsChanged(const QtTreeSelection&)),
                   this, SLOT(_q_selectionsChanged(const QtTreeSelection&)));
        disconnect(d->selectionManager, SIGNAL(currentChanged(const QtTreeModelBase::base_iterator&,const QtTreeModelBase::iterator_base&)),
                   this, SLOT(_q_currentChanged(const QtTreeModelBase::base_iterator&,const QtTreeModelBase::iterator_base&)));
    }
    d->selectionManager = selectionManager;
    if (d->selectionManager) {
        connect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));
        connect(d->selectionManager, SIGNAL(selectionsChanged(const QtTreeSelection&)),
                this, SLOT(_q_selectionsChanged(const QtTreeSelection&)));
        connect(d->selectionManager, SIGNAL(currentChanged(const QtTreeModelBase::iterator_base&,const QtTreeModelBase::iterator_base&)),
                this, SLOT(_q_currentChanged(const QtTreeModelBase::iterator_base&,const QtTreeModelBase::iterator_base&)));
    }
    d->_q_reset();
}

/*!
 */
QtGraphicsHeader *QtGraphicsTreeView::header() const
{
    Q_D(const QtGraphicsTreeView);
    return d->header;
}

/*!
 */
void QtGraphicsTreeView::setHeader(QtGraphicsHeader *header)
{
    Q_D(QtGraphicsTreeView);
    if (d->header == header)
        return;
    if (d->header) {
        disconnect(d->header, SIGNAL(destroyed()), this, SLOT(_q_headerDestroyed()));
    }
    d->header = header;
    if (d->header) {
        connect(d->header, SIGNAL(destroyed()), this, SLOT(_q_headerDestroyed()));
    }
}

/*!
 */
void QtGraphicsTreeView::setTextElideMode(Qt::TextElideMode mode)
{
    Q_D(QtGraphicsTreeView);
    d->textElideMode = mode;

}

/*!
 */
Qt::TextElideMode QtGraphicsTreeView::textElideMode() const
{
    Q_D(const QtGraphicsTreeView);
    return d->textElideMode;
}

/*!
 */
void QtGraphicsTreeView::setFirstIndex(int index)
{
    Q_D(QtGraphicsTreeView);
    d->firstIndex = index;
    emit firstIndexChanged(index);
    updateLayout();
}

/*!
 */
int QtGraphicsTreeView::firstIndex() const
{
    Q_D(const QtGraphicsTreeView);
    return d->firstIndex;
}

/*!
 */
int QtGraphicsTreeView::firstSection() const
{
    Q_D(const QtGraphicsTreeView);
    return d->header ? d->header->firstSection() : 0;
}

/*!
 */
void QtGraphicsTreeView::setHorizontalOffset(qreal offset)
{
    Q_D(QtGraphicsTreeView);
    if (d->header)
        d->header->setOffset(offset);
    updateLayout();
}

/*!
 */
qreal QtGraphicsTreeView::horizontalOffset() const
{
    Q_D(const QtGraphicsTreeView);
    return d->header ? d->header->offset() : 0;
}

/*!
 */
void QtGraphicsTreeView::setVerticalOffset(qreal offset)
{
    Q_D(QtGraphicsTreeView);
    d->verticalOffset = offset;
    updateLayout();
}

/*!
 */
qreal QtGraphicsTreeView::verticalOffset() const
{
    Q_D(const QtGraphicsTreeView);
    return d->verticalOffset;
}

/*!
 */
int QtGraphicsTreeView::maximumFirstIndex(int *count) const
{
    Q_D(const QtGraphicsTreeView);
    int index = 0;
    QStack<QtTreeModelIterator> stack;
    QtTreeModelIterator it = d->model ? d->model->begin() : QtTreeModelIterator();
    QtTreeModelIterator last;
    // go to the bottom of all the visible items
    while (it.isValid()) {
        last = it;
        ++index;
        d->next(it, stack);
    }
    if (count)
        *count = index;
    --index;
    // index is now at the last visible item
    QStyleOptionViewItemV4 option;
    initStyleOption(&option);
    qreal y = -d->verticalOffset;
    // make sure that we can see all of the last item
    qreal height = size().height() - d->itemHeight(option, last);
    for (it = last; y < height && it.isValid(); --index) {
        y += d->itemHeight(option, it);
        d->previous(it, stack);
    }
    if (y > height)
        ++index;
    return qMax(0, index);
}

/*!
 */
int QtGraphicsTreeView::maximumFirstSection() const
{
    Q_D(const QtGraphicsTreeView);
    return d->header ? d->header->maximumFirstSection() : 0;
}

/*!
 */
qreal QtGraphicsTreeView::maximumHorizontalOffset() const
{
    Q_D(const QtGraphicsTreeView);
    return d->header ? d->header->maximumOffset() : 0;
}

/*!
 */
qreal QtGraphicsTreeView::maximumVerticalOffset() const
{
    Q_D(const QtGraphicsTreeView);
    QStyleOptionViewItemV4 option;
    initStyleOption(&option);
    qreal content = 0;
    QStack<QtTreeModelIterator> stack;
    QtTreeModelIterator it = d->firstItem(stack);
    while (it.isValid()) {
        content += d->itemHeight(option, it);
        d->next(it, stack);
    }
    return content - size().height();
}

/*!
 */
void QtGraphicsTreeView::setItemExpanded(QtTreeModelIterator &it, bool expanded)
{
    Q_D(QtGraphicsTreeView);
    if (expanded)
        d->expand(it);
    else
        d->collapse(it);
}

/*!
 */
bool QtGraphicsTreeView::isItemExpanded(const QtTreeModelIterator &it) const
{
    Q_D(const QtGraphicsTreeView);
    return d->isExpanded(it);
}

/*!
 */
QList<QtTreeModelIterator> QtGraphicsTreeView::expandedItems() const
{
    Q_D(const QtGraphicsTreeView);
    return d->expandedItems.toList();
}

/*!
 */
void QtGraphicsTreeView::nextItem(QtTreeModelIterator &it) const
{
    Q_D(const QtGraphicsTreeView);
    QStack<QtTreeModelBase::iterator_base> stack;
    d->next(it, stack);
}

/*!
 */
void QtGraphicsTreeView::previousItem(QtTreeModelIterator &it) const
{
    Q_D(const QtGraphicsTreeView);
    QStack<QtTreeModelBase::iterator_base> stack;
    d->previous(it, stack);
}

/*!
  \reimp
 */
bool QtGraphicsTreeView::event(QEvent *event)
{
    Q_D(QtGraphicsTreeView);
    if (d->controller && d->controller->processEvent(event, transform())) {
        event->accept();
        return true;
    }
    if (event->type() == QEvent::Timer && static_cast<QTimerEvent*>(event)->timerId() == d->layoutTimer.timerId())
        doLayout();
    return QGraphicsWidget::event(event);
}

/*!
  \reimp
 */
QVariant QtGraphicsTreeView::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(QtGraphicsTreeView);
    if (change == QGraphicsItem::ItemChildRemovedChange) {
        QGraphicsItem *item = qvariant_cast<QGraphicsItem*>(value);
        QtTreeModelBase::iterator_base key = d->items.key(static_cast<QGraphicsObject*>(item));
        if (key.isValid())
            d->items.remove(key);
    }
    return QGraphicsWidget::itemChange(change, value);
}

/*!
 */
QtGraphicsTreeViewItemCreatorBase *QtGraphicsTreeView::itemCreator() const
{
    Q_D(const QtGraphicsTreeView);
    return d->creator;
}

/*!
 */
void QtGraphicsTreeView::setItemCreator(QtGraphicsTreeViewItemCreatorBase *creator)
{
    Q_D(QtGraphicsTreeView);
    delete d->creator;
    d->creator = creator;
}

/*!
  Returns the visible item representing the given \a it, if that item exists and is visible.
  Otherwise returns null.
 */
QGraphicsObject *QtGraphicsTreeView::itemForIterator(const QtTreeModelIterator &it) const
{
    Q_D(const QtGraphicsTreeView);
    return d->items.value(it);
}

/*!
 */
void QtGraphicsTreeView::updateLayout()
{
    Q_D(QtGraphicsTreeView);
    if (!d->layoutTimer.isActive())
        d->layoutTimer.start(0, this);
}
/*!
 */
void QtGraphicsTreeView::doLayout()
{
    Q_D(QtGraphicsTreeView);
    d->layoutTimer.stop();

    const QRectF area = QRectF(QPointF(0, 0), size());
    const QSizeF constraint = QSizeF(size().width(), -1);

    qreal y = -d->verticalOffset;
    qreal w = d->header ? qMax(d->header->contentSize(), area.width()) : area.width();
    qreal horizontalOffset = d->header ? d->header->offset() : 0;
    QStack<QtTreeModelIterator> stack;
    QtTreeModelIterator it;
    int index = 0;
#if 0
    y = d->cachedVerticalOffset;
    it = d->cachedFirstVisibleItem;
    index = d->cachedVisibleIndex;
    syncStack(it, stack);
#endif
        // this handles caching too
        it = d->firstItem(stack, &index);
    // offset-based scrolling
    /*
    if (y < area.y()) {
        while (y < area.y() && it.isValid()) {
            y += d->itemHeight(option, it);
            d->next(it, stack);
            ++index;
        }
    } else if (y > area.y()) {
        while (y > area.y() && it.isValid()) {
            y -= d->itemHeight(option, it);
            d->previous(it, stack);
            --index;
        }
    }
    */
#if 0
    d->cachedVerticalOffset = d->verticalOffset;
    d->cachedFirstVisibleItem = it;
    d->cachedVisibleIndex = index;
#endif

    if (!it.isValid())
        return;

    // collect unused items for reassignment
    QHash<QtTreeModelBase::iterator_base, QGraphicsObject*> unused = d->items;
    d->items.clear(); // ### FIXME: find a solution that updates in-place

    // position the visible items
    const qreal x = -horizontalOffset;
    while (y < area.height() && it.isValid()) {
        // update item geometry
        QGraphicsObject *item = unused.take(it);
        if (!item) { // if the item did not exist
            if (unused.isEmpty()) {
                item = d->creator->create(it, this);
            } else { // take another random item (idealy we should take from the bottom, since this kicks in early when scolling up)
                QHash<QtTreeModelBase::iterator_base, QGraphicsObject*>::iterator random = unused.begin();
                item = d->creator->reassign(it, random.value(), this);
                unused.erase(random);
            }
        }
        d->items.insert(it, item);
        qreal height;
        if (item->isWidget()) {
            QSizeF size = static_cast<QGraphicsWidget*>(item)->effectiveSizeHint(Qt::PreferredSize, constraint);
            static_cast<QGraphicsWidget*>(item)->setGeometry(x, y, w, size.height());
            height = size.height();
        } else {
            item->setPos(x, y);
            height = item->boundingRect().height();
        }
        // next item
        y += height;
        d->next(it, stack);
    } // while

    // recycle any unused items left
    QHash<QtTreeModelBase::iterator_base, QGraphicsObject*>::iterator it2 = unused.begin();
    for (; it2 != unused.end(); ++it2)
        d->creator->recycle(it2.value());
}

/*!
 */
QtTreeModelIterator QtGraphicsTreeView::itemAt(const QPointF &position, int *index, QRectF *rect) const
{
    // works only inside the view area
    Q_D(const QtGraphicsTreeView);
    QStyleOptionViewItemV4 option;
    initStyleOption(&option);
    qreal y = -d->verticalOffset;
    int flat = -1;
    QStack<QtTreeModelIterator> stack;
    QtTreeModelIterator it = d->firstItem(stack, &flat);
    const qreal indentation = QtGraphicsTreeViewPrivate::indentation();
    while (y < position.y() && it.isValid()) {
        qreal height = d->itemHeight(option, it);
        if ((y + height) >= position.y()) {
            if (rect) { // if the user asked for the item rect too
                qreal x = stack.count() * indentation;
                qreal width = size().width() - x;
                /*
                if (d->header) {
                    QPair<qreal,qreal> geometry = d->header->sectionGeometryAt(position);
                    x = geometry.first;
                    width = geometry.second;
                }
                */
                rect->setRect(x, y, width, height);
            }
            if (index)
                *index = flat;
            return it;
        }
        // next item
        y += height;
        d->next(it, stack);
        ++flat;
    } // while
    return QtTreeModelIterator();
}

/*!
  Returns the flat index of the item represented by the given \a it.

  Note that this function is potentially slow and should not be used
  in performance critical code.
 */
int QtGraphicsTreeView::indexOf(const QtTreeModelIterator &it) const
{
    // ### optimize - use heuristics like visible items
    Q_D(const QtGraphicsTreeView);
    QtTreeModelIterator iterator = d->model->begin();
    QStack<QtTreeModelIterator> stack;
    int index = 0;
    while (iterator.isValid() && iterator != it) {
        d->next(iterator, stack);
        ++index;
    }
    return index;
}

/*!
 */
void QtGraphicsTreeView::initStyleOption(QStyleOptionViewItemV4 *option) const
{
    Q_ASSERT(option);
    option->widget = 0;
    option->font = QApplication::font();
    option->decorationPosition = QStyleOptionViewItem::Left;
    option->decorationAlignment = Qt::AlignCenter;
    option->displayAlignment = Qt::AlignLeft|Qt::AlignVCenter;
    option->textElideMode = textElideMode();
    option->showDecorationSelected = style()->styleHint(QStyle::SH_ItemView_ShowDecorationSelected, 0, 0);
    int pm = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, 0);
    option->decorationSize = QSize(pm, pm);
    option->decorationPosition = QStyleOptionViewItem::Left;
    option->locale = QLocale(); // ### FIXME
    option->locale.setNumberOptions(QLocale::OmitGroupSeparator);
    option->features |= QStyleOptionViewItemV2::WrapText; // ### if wrap text is enabled
    option->features |= QStyleOptionViewItemV2::Alternate; // ### if alternate is enabled
    if (isActiveWindow())
        option->state |= QStyle::State_Active;
}

/*!
 */
QRectF QtGraphicsTreeView::disclosureRect(const QRectF &rect) const
{
    Q_D(const QtGraphicsTreeView);
    return d->disclosureRect(rect);
}

QT_END_NAMESPACE

#include "moc_qgraphicstreeview.cpp"
