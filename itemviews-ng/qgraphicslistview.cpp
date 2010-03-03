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

#include "qgraphicslistview.h"
#include "qgraphicslistview_p.h"

#include "qlistcontroller.h"
#include "qlistmodelinterface.h"
#include "qlistselectionmanager.h"

#include <qstyle.h>
#include <qstyleoption.h>
#include <qrect.h>
#include <qdatetime.h>
#include <qapplication.h>
#include <qgraphicsscene.h>
#include <qpainter.h>
#include <qdebug.h>
#include <limits.h>

QT_BEGIN_NAMESPACE

/*!
  \class QtGraphicsListViewItem

  \brief The item that visually represents one visible entry in the list.

  The QtGraphicsListView delegates all the drawing and composition of individual
  list-items to this class. There will be one QtGraphicsListViewItem instance for
  each <strong>visible</strong> list item.

  By default the list items will use the current style to determine the looks.
  For altering the appearance of individual list items creating a subclass of
  this one is the preferred solution. The subclass can use various strategies to
  alter the individual list items appearance, for example;
  <ul>
    <li>Reimplement paint() to change the look for just this list.</li>
    <li>Add any number of child QGraphicsObject instances that do the painting instead.</li>
    <!-- we will have to refer to the kinetic items when they are done -->
  </ul>

  The QtGraphicsListView will create and delete QtGraphicsListViewItem instances
  as they are needed based on how many items are visible.  It is possible for an
  item to be reused. When this happens the itemChanged() method will be called
  to make the item aware of this change.  The default implementation will make sure
  that update() will be called.
*/

QtGraphicsListViewItem::QtGraphicsListViewItem(int index, QtGraphicsListView *view)
    : QGraphicsWidget(view, 0), d_ptr(new QtGraphicsListViewItemPrivate)
{
    Q_D(QtGraphicsListViewItem);
    //setCacheMode(QGraphicsObject::ItemCoordinateCache);
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    d->q_ptr = this;
    d->view = view;
    d->index = index;
}

/*!
 */
QtGraphicsListViewItem::~QtGraphicsListViewItem()
{
    delete d_ptr;
}

/*!
  Returns the index of the list model item this
  view tem represents.
  \sa setIndex()
 */
int QtGraphicsListViewItem::index() const
{
    Q_D(const QtGraphicsListViewItem);
    return d->index;
}

/*!
  Sets the index of list model item this
  view item will represent.
 */
void QtGraphicsListViewItem::setIndex(int index)
{
    Q_D(QtGraphicsListViewItem);
    if (d->index != index) {
        d->index = index;
        itemChanged(QList<QByteArray>());
    }
}

/*!
  \reimp
 */

QRectF QtGraphicsListViewItem::boundingRect() const
{
    return QRectF(QPointF(), geometry().size());
}

/*!
  \reimp
 */
QSizeF QtGraphicsListViewItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const QtGraphicsListViewItem);
    Q_UNUSED(constraint);
    if (d->view) {
        d->view->initStyleOption(&d->option);
        initStyleOption(&d->option);
        switch (which) {
        case Qt::MinimumSize:
            return QSizeF(0, 0);
        case Qt::PreferredSize: {
            const QVariant value = d->view->d_func()->cachedData(d->index, "SizeHintRole");
            if (value.isValid())
                return qvariant_cast<QSizeF>(value);
            else
                return d->view->style()->sizeFromContents(QStyle::CT_ItemViewItem, &d->option, QSize(), d->option.widget);
        }
        case Qt::MaximumSize:
            return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        default:
            break;
        }
    }
    return QSizeF();
}

/*!
  \reimp
 */
#include <qlistview.h>
void QtGraphicsListViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_D(QtGraphicsListViewItem);
    // ### this code is less than optimal, but we will hopefully get rid of the style calls
    // ### by moving to a purely scene-graph based approach for items
    if (d->view)
        d->view->initStyleOption(&d->option);
    initStyleOption(&d->option);
    d->option.rect = rect().toRect();
    style()->drawControl(QStyle::CE_ItemViewItem, &d->option, painter, widget);
}

/*!
    This callback method will be called whenever any part of the item has been changed.
    The default implementation always calls update(), reimplemnting this method allows items
    to only update based on the data that has actually changed.
    The \a roles will indicate what has actually changed, if the list is empty then all roles have changed.

    \sa QtListModelInterface::itemsChanged()
 */
void QtGraphicsListViewItem::itemChanged(const QList<QByteArray> &roles)
{
    Q_UNUSED(roles);
    update();
}

/*!
  Returns the data for the given \a roles, for the list model
  item this view item represents.
 */
QHash<QByteArray, QVariant> QtGraphicsListViewItem::data(const QList<QByteArray> &roles) const
{
    Q_D(const QtGraphicsListViewItem);
    if (d->view && d->view->model())
        return d->view->model()->data(d->index, roles);
    return QHash<QByteArray, QVariant>();
}

/*!
  Initialize the given \a option with the settings and data
  specific for the item at the given \a index.
 */
void QtGraphicsListViewItem::initStyleOption(QStyleOptionViewItemV4 *option) const
{
    Q_D(const QtGraphicsListViewItem);
    Q_ASSERT(option);

    if (!d->view)
        return;

    if (d->view->selectionManager()) {
        // Current Item
        if (d->view->selectionManager()->currentItem() == d->index)
            option->state |= QStyle::State_HasFocus;
        else
            option->state &= ~QStyle::State_HasFocus;
        // selectionManager
        if (!d->view->highlight() && d->view->selectionManager()->isSelected(d->index))
            option->state |= QStyle::State_Selected;
        else
            option->state &= ~QStyle::State_Selected;
    }

    // Data
    QVariant value;

    // FontRole
    value = d->view->d_func()->cachedData(d->index, "FontRole");
    if (value.isValid() && !value.isNull()) {
        option->font = qvariant_cast<QFont>(value).resolve(option->font);
        option->fontMetrics = QFontMetrics(option->font);
    }

    // TextAlignmentRole
    value = d->view->d_func()->cachedData(d->index, "TextAlignmentRole");
    if (value.isValid() && !value.isNull())
        option->displayAlignment = (Qt::Alignment)value.toInt();

    // ForegroundRole
    value = d->view->d_func()->cachedData(d->index, "ForegroundRole");
    if (qVariantCanConvert<QBrush>(value))
        option->palette.setBrush(QPalette::Text, qvariant_cast<QBrush>(value));

    // BackgroundRole
    value = d->view->d_func()->cachedData(d->index, "BackgroundRole");
    if (qVariantCanConvert<QBrush>(value))
        option->palette.setBrush(QPalette::Background, qvariant_cast<QBrush>(value));

    // CheckStateRole
    value = d->view->d_func()->cachedData(d->index, "CheckStateRole");
    if (value.isValid()) {
        option->features |= QStyleOptionViewItemV2::HasCheckIndicator;
        option->checkState = static_cast<Qt::CheckState>(value.toInt());
    }

    // DecorationRole
    value = d->view->d_func()->cachedData(d->index, "DecorationRole");
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
    value = d->view->d_func()->cachedData(d->index, "DisplayRole");
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
    }
}

/*!
  Returns the view where this item is shown.
 */
QtGraphicsListView *QtGraphicsListViewItem::view() const
{
    Q_D(const QtGraphicsListViewItem);
    return d->view;
}

/*!
  \class QtGraphicsListViewItemCreatorBase
  \brief Creates, reassignes and recycles view items.
 */

/*!
  \class QtGraphicsListViewItemCreator
  \brief an easy to use factory class to create QtGraphicsListViewItem instances for the QtGraphicsListView
 */

/*!
  Destructor.
 */
QtGraphicsListViewItemCreatorBase::~QtGraphicsListViewItemCreatorBase()
{
}

/*!
  \fn QtGraphicsListViewItemCreatorBase::create(int index, QtGraphicsListView *view)
 */

/*!
  \fn QGraphicsObject *QtGraphicsListViewItemCreatorBase::reassign(int index, QGraphicsObject *item, QtGraphicsListView *view)
  Assigns a new \a index to the given \a item in the given \a view, and returns it to the view.
*/

/*!
  Updates the given \a item, with the given \a index, using the given \a roles.
*/
void QtGraphicsListViewItemCreatorBase::update(int index, QGraphicsObject *item, const QList<QByteArray> &roles)
{
    Q_UNUSED(index);
    Q_UNUSED(roles);
    Q_ASSERT(item);
    item->update();
}

/*!
  Discart the given \a item. The creator may choose to reuse the item later.
*/
void QtGraphicsListViewItemCreatorBase::recycle(QGraphicsObject *item)
{
    delete item;
}

/*!
  \fn QtGraphicsListViewItem *QtGraphicsListViewItemCreatorBase::create(int index, QtGraphicsListView *view)
  The view will call the create() method for every visible item.

  The item will be a child of the given \a view and will display the data referred to by the given \a index.
*/

// QtGraphicsListViewPrivate

/*!
  \internal
 */
QtGraphicsListViewPrivate::QtGraphicsListViewPrivate()
    : q_ptr(0), controller(0), model(0), selectionManager(0),
      orientation(Qt::Horizontal), textElideMode(Qt::ElideMiddle),
      highlight(0), firstIndex(0), offset(0),
#if CACHING_ENABLED
      cachedIndexOffset(0), cachedCoordinateOffset(0),
#endif
      cachedDataIndex(-1),
      creator(new QtGraphicsListViewItemCreator<QtGraphicsListViewItem>())
{
}

/*!
  \internal
 */
QtGraphicsListViewPrivate::~QtGraphicsListViewPrivate()
{
    delete creator;
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_controllerDestroyed()
{
    controller = 0;
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_modelDestroyed()
{
    model = 0;
    checkCache(0, 0);
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_selectionsDestroyed()
{
    selectionManager = 0;
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_itemsChanged(int index, int count, const QList<QByteArray> &roles)
{
    //Q_Q(QtGraphicsListView);
    checkCache(index, count);
    //qDebug() << "QtGraphicsListViewPrivate::_q_itemsChanged" << index << count;
    for (int i = 0; i < items.count(); ++i) {
        int itemIndex = items.at(i).first;
        if (itemIndex <= index) // item was changed
            creator->update(itemIndex, items.at(i).second, roles);
        if (itemIndex >= index + count) // change happened above
            break;
    }
    // update the layout only if the items geometries change
    //q->updateLayout(); // ### FIXME: be more discriminate
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_itemsInserted(int index, int count)
{
    Q_Q(QtGraphicsListView);
    checkCache(index, count);
    //qDebug() << "QtGraphicsListViewPrivate::_q_itemsInserted" << index << count;
    q->updateLayout(); // ### FIXME: be more discriminate
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_itemsRemoved(int index, int count)
{
    Q_Q(QtGraphicsListView);
    checkCache(index, count);
    //qDebug() << "QtGraphicsListViewPrivate::_q_itemsRemoved" << index << count;
    q->updateLayout(); // ### FIXME: be more discriminate
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_reset()
{
    //Q_Q(QtGraphicsListView);
    checkCache(0, 0);
    //qDebug() << "QtGraphicsListViewPrivate::_q_reset";
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::_q_selectionsChanged(const QtListSelectionChange &change)
{
    if (items.isEmpty())
        return;
    const int from = items.first().first;
    const int to = items.last().first;
    if (from <= change.index() && (change.index() + change.count() - 1) <= to) {
        const QList<int> changeIndexes = change.indexes();
        for (int i = 0; i < changeIndexes.count(); ++i) {
            const int index = changeIndexes.at(i);
            if (from <= index && index <= to)
                items.at(index - from).second->update();
         }
    }
}

/*!
  \internal
*/
void QtGraphicsListViewPrivate::_q_currentChanged(int current, int previous)
{
    if (items.isEmpty())
        return;
    //qDebug() << "QtGraphicsListViewPrivate::_q_currentChanged" << current << previous;
    if (highlight) {
        updateHighlightGeometry();
    } else {
        const int from = items.first().first;
        const int to = items.last().first;
        if (from <= current && current <= to)
            items.at(current - from).second->update();
        if (from <= previous && previous <= to)
            items.at(previous - from).second->update();
    }
}

/*!
 \internal
*/
void QtGraphicsListViewPrivate::updateHighlightGeometry()
{
    const int from = items.first().first;
    const int to = items.last().first;
    const int current = currentItem();
    if (from <= current && current <= to) {
        highlight->show();
        highlight->setZValue(-1);
        const QGraphicsObject *currentObject = items.at(current - from).second;
        if (highlight->isWidget() && currentObject->isWidget()) {
            const QRectF geometry = static_cast<const QGraphicsWidget*>(currentObject)->geometry();
            static_cast<QGraphicsWidget*>(highlight)->setGeometry(geometry);
        } else {
            highlight->setPos(currentObject->pos());
        }
    } else {
        highlight->hide();
    }
}

/*!
  \internal
 */
void QtGraphicsListViewPrivate::checkCache(int index, int count)
{
    Q_UNUSED(count);
    if (cachedDataIndex >= index) {
        cachedDataHash.clear();
        cachedDataIndex = -1;
    }
#if CACHING_ENABLED
    if (cachedIndexOffset >= index) {
        cachedIndexOffset = 0;
        cachedCoordinateOffset = 0;
    }
#endif
}

/*!
  \internal
 */
QVariant QtGraphicsListViewPrivate::cachedData(int index, const QByteArray &role) const
{
    // ### structure this list so the most used items are added first
    static const QList<QByteArray> roles = (QList<QByteArray>()
                                            << "FontRole"
                                            << "TextAlignmentRole"
                                            << "ForegroundRole"
                                            << "CheckStateRole"
                                            << "DecorationRole"
                                            << "DisplayRole"
                                            << "SizeHintRole");
    if (index != cachedDataIndex && model) {
        cachedDataHash = model->data(index, roles);
        cachedDataIndex = index;
    }
    return cachedDataHash.value(role);
}

/*!
  \internal
 */
bool QtGraphicsListViewPrivate::isSelected(int index) const
{
    return selectionManager ? selectionManager->isSelected(index) : false;
}

/*!
  \internal
 */
int QtGraphicsListViewPrivate::currentItem() const
{
    return selectionManager ? selectionManager->currentItem() : -1;
}

/*!
  \internal
 */
QSizeF QtGraphicsListViewPrivate::itemSize(int index) const
{
    Q_Q(const QtGraphicsListView);
    const QSizeF constraint = (orientation == Qt::Horizontal
                                ? QSizeF(-1, q->size().height())
                                : QSizeF(q->size().width(), -1));
    if (true || items.isEmpty()) { // ### FIXME: always construct an item
        if (QGraphicsObject *item = creator->create(index, const_cast<QtGraphicsListView*>(q))) {
            const QSizeF size = (item->isWidget()
                                  ? qobject_cast<QGraphicsWidget*>(item)->preferredSize()
                                  : item->boundingRect().size());
            creator->recycle(item); // ### FIXME: save it for re-use later
            return size;
        }
        qWarning() << "QtGraphicsListView: could not create a new item";
        return QSizeF();
    }
    // ### FIXME: average size of visible items - doesn't work
    QSizeF size;
    for (int i = 0; i < items.count(); ++i) // ### FIXME: don't iterate every time
        size += items.at(i).second->boundingRect().size();
    return size / items.count();
}

// QtGraphicsListView

/*!
  \class QtGraphicsListView
  \brief This class positions the content of the list using graphicsItems to determine the way that the list looks.

  The QtGraphicsListView is meant to be used together with a QtListController and a QtListModelInterface.

  \sa QtListController
*/

/*!
  Constructs a QtGraphicsListView with the given \a orientation, \a parent and \a wFlags.
 */
QtGraphicsListView::QtGraphicsListView(Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(new QtGraphicsListViewPrivate)
{
    Q_D(QtGraphicsListView);
    //setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    d->q_ptr = this;
    d->orientation = orientation;
}

/*!
  \internal
 */
QtGraphicsListView::QtGraphicsListView(QtGraphicsListViewPrivate &dd, Qt::Orientation orientation, QGraphicsWidget *parent, Qt::WindowFlags wFlags)
    : QGraphicsWidget(parent, wFlags), d_ptr(&dd)
{
    Q_D(QtGraphicsListView);
    //setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    d->q_ptr = this;
    d->orientation = orientation;
}

/*!
  Returns a pointer to the controller for the view.

  \sa setController()
 */
QtListController *QtGraphicsListView::controller() const
{
    Q_D(const QtGraphicsListView);
    return d->controller;
}

/*!
  Sets the controller for the view to be the given \a controller.

  \sa controller()
 */
void QtGraphicsListView::setController(QtListController *controller)
{
    Q_D(QtGraphicsListView);
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
  Returns a pointer to the model for the view.

  \sa setModel()
 */
QtListModelInterface *QtGraphicsListView::model() const
{
    Q_D(const QtGraphicsListView);
    return d->model;
}

/*!
  Sets the model for the view to be the given \a model.

  \sa model()
 */
void QtGraphicsListView::setModel(QtListModelInterface *model)
{
    Q_D(QtGraphicsListView);
    if (d->model == model)
        return;
    if (d->model) {
        disconnect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        disconnect(d->model, SIGNAL(itemsChanged(int,int,const QList<QByteArray>&)), this, SLOT(_q_itemsChanged(int,int,const QList<QByteArray>&)));
        disconnect(d->model, SIGNAL(itemsInserted(int,int)), this, SLOT(_q_itemsInserted(int,int)));
        disconnect(d->model, SIGNAL(itemsRemoved(int,int)), this, SLOT(_q_itemsRemoved(int,int)));
    }
    d->model = model;
    if (d->model) {
        connect(d->model, SIGNAL(destroyed()), this, SLOT(_q_modelDestroyed()));
        connect(d->model, SIGNAL(itemsChanged(int,int,const QList<QByteArray>&)), this, SLOT(_q_itemsChanged(int,int,const QList<QByteArray>&)));
        connect(d->model, SIGNAL(itemsInserted(int,int)), this, SLOT(_q_itemsInserted(int,int)));
        connect(d->model, SIGNAL(itemsRemoved(int,int)), this, SLOT(_q_itemsRemoved(int,int)));
    }
}

/*!
  Returns a pointer to the selection manager for the view.

  \sa setSelectionManager()
 */
QtListSelectionManager *QtGraphicsListView::selectionManager() const
{
    Q_D(const QtGraphicsListView);
    return d->selectionManager;
}

/*!
  Sets the selectionManager for the view to be the given \a selectionManager.

  \sa selectionManager()
 */
void QtGraphicsListView::setSelectionManager(QtListSelectionManager *selectionManager)
{
    Q_D(QtGraphicsListView);
    if (d->selectionManager == selectionManager)
        return;
    if (d->selectionManager) {
        disconnect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));
        disconnect(d->selectionManager, SIGNAL(selectionsChanged(const QtListSelectionChange&)),
                   this, SLOT(_q_selectionsChanged(const QtListSelectionChange&)));
        disconnect(d->selectionManager, SIGNAL(currentChanged(int,int)), this, SLOT(_q_currentChanged(int,int)));
    }
    d->selectionManager = selectionManager;
    if (d->selectionManager) {
        connect(d->selectionManager, SIGNAL(destroyed()), this, SLOT(_q_selectionsDestroyed()));
        connect(d->selectionManager, SIGNAL(selectionsChanged(const QtListSelectionChange&)),
                this, SLOT(_q_selectionsChanged(const QtListSelectionChange&)));
        connect(d->selectionManager, SIGNAL(currentChanged(int,int)), this, SLOT(_q_currentChanged(int,int)));
    }
}

/*!
  Returns the layout orientation.

  \sa setOrientation()
 */
Qt::Orientation QtGraphicsListView::orientation() const
{
    Q_D(const QtGraphicsListView);
    return d->orientation;
}

/*!
  Sets the orientation of the list layout to be
  the given \a orientation.
  The orientation defines the scroll direction
  of the list.

  \sa orientation()
 */
void QtGraphicsListView::setOrientation(Qt::Orientation orientation)
{
    Q_D(QtGraphicsListView);
    d->orientation = orientation;
    //d->checkCache(0, 0); // TODO
    emit orientationChanged(orientation);
    doLayout(); // don't wait
}

/*!
  Returns the text elide mode.

  \sa setTextElideMode()
 */
Qt::TextElideMode QtGraphicsListView::textElideMode() const
{
    Q_D(const QtGraphicsListView);
    return d->textElideMode;
}

/*!
  Sets the text elide mode to the specified \a mode.
  The default value is Qt::ElideMiddle.

  \sa textElideMode()
 */
void QtGraphicsListView::setTextElideMode(Qt::TextElideMode mode)
{
    Q_D(QtGraphicsListView);
    d->textElideMode = mode;
    //d->checkCache(0, 0);
}

/*!
 */
QGraphicsObject *QtGraphicsListView::highlight() const
{
    Q_D(const QtGraphicsListView);
    return d->highlight;
}

/*!
 */
void QtGraphicsListView::setHighlight(QGraphicsObject *highlight)
{
    Q_D(QtGraphicsListView);
    d->highlight = highlight;
    // update the highlight geometry
    highlight->setParentItem(this);
}

/*!
 */
QtGraphicsListView::~QtGraphicsListView()
{
    delete d_ptr;
}

/*!
  Returns the geometry given to the item referred to by the given \a index.

  Note that this function is potentially slow for non-visible items.

  \sa updateLayout()
 */
QRectF QtGraphicsListView::itemGeometry(int index) const
{
    Q_D(const QtGraphicsListView);
    if (!d->model)
        return QRectF();
    if (QGraphicsObject *item = itemForIndex(index))
        return QRectF(item->pos(), item->boundingRect().size());
    const QSizeF constraint = (d->orientation == Qt::Horizontal
                                ? QSizeF(-1, size().height())
                                : QSizeF(size().width(), -1));
    const int count = d->model->count();
    if (d->orientation == Qt::Vertical) {
        qreal y = -d->offset;
        for (int i = d->firstIndex; i < count; ++i) {
            const QSizeF size = d->itemSize(i);
            if (i == index)
                return QRectF(QPointF(0, y), size);
            y += size.height();
        }
    } else { // Horizontal
        qreal x = -d->offset;
        for (int i = d->firstIndex; i < count; ++i) {
            const QSizeF size = d->itemSize(i);
            if (i == index)
                return QRectF(QPointF(x, 0), size);
            x += size.width();
        }
    }
    return QRectF();
}

/*!
  Returns the index of the item at the given \a position.

  \sa updateLayout()
*/
int QtGraphicsListView::itemAt(const QPointF &position) const
{
    Q_D(const QtGraphicsListView);
    if (!d->model)
        return -1;
    // ### FIXME: check the visible items first
    const QSizeF constraint = (d->orientation == Qt::Horizontal
                                ? QSizeF(-1, size().height())
                                : QSizeF(size().width(), -1));
    const int count = d->model->count();
    int index = d->firstIndex;
    if (d->orientation == Qt::Vertical) {
        qreal y = -d->offset;
        while (y <= position.y() && index < count) {
            const QSizeF size = d->itemSize(index);
            y += size.height();
            if (y >= position.y())
                return index;
            ++index;
        }
    } else {
        qreal x = -d->offset;
        while (x <= position.x() && index < count) {
            const QSizeF size = d->itemSize(index);
            x += size.width();
            if (x >= position.x())
                return index;
            ++index;
        }
    }
    return -1;
}

/*!
  Schedules an update of the item geometries.
 */
void QtGraphicsListView::updateLayout()
{
    Q_D(QtGraphicsListView);
    if (!d->layoutTimer.isActive())
        d->layoutTimer.start(0, this);
}

/*!
  Updates the item geometries.

  \sa itemAt()
 */
void QtGraphicsListView::doLayout()
{
    Q_D(QtGraphicsListView);
    d->layoutTimer.stop();

    const QRectF area = geometry();
    const QSizeF constraint = (d->orientation == Qt::Horizontal
                               ? QSizeF(-1, size().height())
                               : QSizeF(size().width(), -1));
    const int count = d->model ? d->model->count() : 0;
    const bool vertical = (d->orientation == Qt::Vertical);
    const qreal areaStart = (vertical ? area.y() : area.x());
    const qreal areaEnd = (vertical ? area.bottom() : area.right());

    qreal coordinate = -d->offset;
    int index = d->firstIndex;

    //qDebug() << "---------------------------";
    //qDebug() << "coordinate" << coordinate;
    //qDebug() << "index" << index;

    // find the visible items; caching helps us skip this most of the time
    if (coordinate < 0 && !d->items.isEmpty()) {
#if CACHING_ENABLED
        // optimization: use the cached index and offset as starting points
        // The cached values are offsets from the start of the _contents_
        // and the to index found at that offset.
        index += d->cachedIndexOffset;
        coordinate += d->cachedCoordinateOffset;

        //qDebug() << "cached index offset" << d->cachedIndexOffset;
        //qDebug() << "cached coordinate offset" << d->cachedCoordinateOffset;
#endif
        //qDebug() << "actual item offset" <<  d->items.first().second->pos().y();
        //qDebug() << "actual item index" << d->items.first().first;

        // the visible area starts at coordinate == 0
    if (coordinate < 0) { // the cached offset was above or to the left of the visible area
            while (index < count) {
                const QSizeF hint = d->itemSize(index);
                const qreal size = vertical ? hint.height() : hint.width();
                if (coordinate + size > areaStart)
                    break;
                coordinate += size;
                ++index;
            }
        } else if (coordinate > 0) { // the cached offset was below or to the right
            //qDebug() << "ABOVE";
            while (index >= 0 && coordinate > 0) {
                const QSizeF hint = d->itemSize(index);
                const qreal size = vertical ? hint.height() : hint.width();
                coordinate -= size;
                --index;
            }
        }
#if CACHING_ENABLED
        //qDebug() << "new cached index offset" << (index - d->firstIndex);
        //qDebug() << "new cached coordinate offset" << (coordinate + d->offset);
        d->cachedIndexOffset = index - d->firstIndex;
        d->cachedCoordinateOffset = coordinate + d->offset;
#endif
    }

    // move existing items around in the list
    const int firstVisibleIndex = index;
    if (!d->items.isEmpty()) {
         // scrolling down or right - move from first to last
        while (d->items.first().first < firstVisibleIndex) {
            const int index = d->items.last().first + 1;
            QGraphicsObject *item = d->items.takeFirst().second;
            d->items.append(QPair<int,QGraphicsObject*>(index, d->creator->reassign(index, item, this)));
            const bool isWidget = item->isWidget();
            const QSizeF size = (isWidget
                                  ? static_cast<QGraphicsWidget*>(item)->effectiveSizeHint(Qt::PreferredSize, constraint)
                                  : item->boundingRect().size());
            if (vertical) {
                if (isWidget)
                    static_cast<QGraphicsWidget*>(item)->resize(area.width(), size.height());
            } else {
                if (isWidget)
                    static_cast<QGraphicsWidget*>(item)->resize(size.width(), area.height());
            }
        }
        // scrolling up or left - move from last to first (assuming same view size)
        const int firstNonVisibleIndex = firstVisibleIndex + d->items.count();
        while (d->items.last().first >= firstNonVisibleIndex) {
            const int index = d->items.first().first - 1;
            QGraphicsObject *item = d->items.takeLast().second;
            d->items.prepend(QPair<int,QGraphicsObject*>(index, d->creator->reassign(index, item, this)));
            const bool isWidget = item->isWidget();
            const QSizeF size = (isWidget
                                  ? static_cast<QGraphicsWidget*>(item)->effectiveSizeHint(Qt::PreferredSize, constraint)
                                  : item->boundingRect().size());
            if (vertical) {
                if (isWidget)
                    static_cast<QGraphicsWidget*>(item)->resize(area.width(), size.height());
            } else {
                if (isWidget)
                    static_cast<QGraphicsWidget*>(item)->resize(size.width(), area.height());
            }
        }
    }

    // set existing items geometry
    for (int i = 0; i < d->items.count() && index < count && coordinate < areaEnd; ++i) {
        QGraphicsObject *item = d->items.at(i).second;
        const bool isWidget = item->isWidget();
        const QSizeF size = item->boundingRect().size(); // use existing size
        if (vertical) {
            if (isWidget && size.width() != area.width()) // only change size if with has changed
                static_cast<QGraphicsWidget*>(item)->setGeometry(0, coordinate, area.width(), size.height());
            else
                item->setPos(0, coordinate);
            coordinate += size.height();
        } else {
            if (isWidget && size.height() != area.height()) // only change size if height has changed
                static_cast<QGraphicsWidget*>(item)->setGeometry(coordinate, 0, size.width(), area.height());
            else
                item->setPos(coordinate, 0);
            coordinate += size.width();
        }
        ++index;
    }

    // add more items and set geometry
    while (index < count && coordinate < areaEnd) {
        if (QGraphicsObject *item = d->creator->create(index, this)) {
            d->items.append(QPair<int, QGraphicsObject*>(index, item));
            const bool isWidget = item->isWidget();
            const QSizeF size = (isWidget
                                  ? static_cast<QGraphicsWidget*>(item)->effectiveSizeHint(Qt::PreferredSize, constraint)
                                  : item->boundingRect().size());
            if (vertical) {
                if (isWidget)
                    static_cast<QGraphicsWidget*>(item)->setGeometry(0, coordinate, area.width(), size.height());
                else
                    item->setPos(0, coordinate);
                coordinate += size.height();
            } else {
                if (isWidget)
                    static_cast<QGraphicsWidget*>(item)->setGeometry(coordinate, 0, size.width(), area.height());
                else
                    item->setPos(coordinate, 0);
                coordinate += size.width();
            }
        }
        ++index;
    }

    // remove unused items
    const int visibleCount = index - firstVisibleIndex;
    while (visibleCount < d->items.count())
        d->creator->recycle(d->items.takeLast().second);

    // update highlight
    if (d->highlight)
        d->updateHighlightGeometry();
}

/*!
  Returns the maximum index that can
  be set to be the first visible item index.

  \sa setFirstIndex()
 */
int QtGraphicsListView::maximumFirstIndex() const
{
    // ### cache this value
    Q_D(const QtGraphicsListView);
    const QSizeF constraint = (d->orientation == Qt::Horizontal
                                ? QSizeF(-1, size().height())
                                : QSizeF(size().width(), -1));
    int last = (d->model ? d->model->count() : 0) - 1;
    int index = last;
    if (orientation() == Qt::Vertical) {
        qreal height = size().height() + d->offset;
        for (; index >= 0; --index) {
            height -= d->itemSize(index).height();
            if (height < 0)
                break;
        }
    } else {
        qreal width = size().width() + d->offset;
        for (; index >= 0; --index) {
            width -= d->itemSize(index).width();
            if (width < 0)
                break;
        }
    }
    // the last item may be partially visible or
    // there may have been no items in the model
    if (index < last || index == -1)
        ++index;
    return index;
}

/*!
  Returns the maximum offset value that can be
  set on the view.

  \sa setOffset()
 */
qreal QtGraphicsListView::maximumOffset() const
{
    // ### cache this value
    Q_D(const QtGraphicsListView);
    const QSizeF constraint = (d->orientation == Qt::Horizontal
                                ? QSizeF(-1, size().height())
                                : QSizeF(size().width(), -1));
    qreal max = 0;
    qreal content = 0;
    int count = d->model ? d->model->count() : 0;
    if (orientation() == Qt::Vertical) {
        for (int index = 0; index < count; ++index) {
            const QSizeF size = d->itemSize(index);
            content += size.height();
            max = qMax(max, size.height());
        }
        return qMax<qreal>(content - size().height(), 0);
    } else { // Horizontal
        for (int index = 0; index < count; ++index) {
            const QSizeF size = d->itemSize(index);
            content += size.width();
            max = qMax(max, size.width());
        }
        return qMax<qreal>(content - size().width(), 0);
    }
}

/*!
  Sets the first index to be painted and
  that can be interacted with to be the
  item at the given \a index.

  \sa firstIndex()
 */
void QtGraphicsListView::setFirstIndex(int index)
{
    Q_D(QtGraphicsListView);
    if (d->firstIndex == index || index < 0)
        return;
    d->firstIndex = index;
    emit firstIndexChanged(index);
    doLayout(); // don't wait
}

/*!
  Returns the horizontal offset value.

  \sa setOffset()
 */
qreal QtGraphicsListView::offset() const
{
    Q_D(const QtGraphicsListView);
    return d->offset;
}

/*!
  Sets the offset value. The offset value is used when
  painting and interacting with the contents of the list.

  \sa offset()
 */
void QtGraphicsListView::setOffset(qreal offset)
{
    Q_D(QtGraphicsListView);
    d->offset = offset;
    emit offsetChanged(offset);
    doLayout(); // don't wait
}

/*!
  Returns the index of the first item
  in the list that is painted and that
  can be interacted with.
 */
int QtGraphicsListView::firstIndex() const
{
    Q_D(const QtGraphicsListView);
    return d->firstIndex;
}

/*!
  Returns an offset value that will ensure that the given \a index
  is visible in the view.
 */
qreal QtGraphicsListView::offsetToEnsureIndexIsVisible(int index) const
{
    Q_D(const QtGraphicsListView);
    const QRectF item = itemGeometry(index); // potentially slow
    const QRectF view = geometry();
    if (orientation() == Qt::Vertical) {
        if (item.bottom() < view.top())
            return d->offset + item.top();
        else if (item.top() > view.bottom())
            return d->offset + item.bottom() - view.height();
    } else {
        if (item.right() < view.left())
            return d->offset + item.x();
        else if (item.left() > view.right())
            return d->offset + item.right() - view.width();
    }
    return d->offset;
}

/*!
  Returns an first index value that will ensure that the given \a index
  is visible in the view.
 */
int QtGraphicsListView::firstIndexToEnsureIndexIsVisible(int index) const
{
    Q_D(const QtGraphicsListView);
    if (index < d->firstIndex)
        return index;
    else if (index >= (d->firstIndex + d->items.count()))
        return qBound(0, index - d->items.count() + 1, maximumFirstIndex()); // slow
    return d->firstIndex;
}

/*!
  Initialize the given \a option struct with settings
  for this list view and the given \a device.
 */
void QtGraphicsListView::initStyleOption(QStyleOptionViewItemV4 *option) const
{
    Q_ASSERT(option);
    option->widget = 0;
    option->palette = palette();
    option->font = font();
    option->fontMetrics = QFontMetrics(option->font);
    option->textElideMode = textElideMode();
    option->showDecorationSelected = style()->styleHint(QStyle::SH_ItemView_ShowDecorationSelected);
    int pm = style()->pixelMetric(QStyle::PM_ListViewIconSize);
    //int pm = style()->pixelMetric(QStyle::PM_IconViewIconSize);
    option->decorationSize = QSize(pm, pm);
    bool vertical = (orientation() == Qt::Vertical);
    option->decorationPosition = vertical ? QStyleOptionViewItem::Left : QStyleOptionViewItem::Top;
    option->decorationAlignment = Qt::AlignCenter;
    option->displayAlignment = vertical ? Qt::AlignLeft|Qt::AlignVCenter : Qt::AlignCenter;
    option->direction = layoutDirection();
    //option->locale = QLocale(); // ### FIXME
    option->locale.setNumberOptions(QLocale::OmitGroupSeparator);
    //if (d->wrapText) option->features |= QStyleOptionViewItemV2::WrapText;
    //if (d->alternate) option->features |= QStyleOptionViewItemV2::Alternate;
    if (isActiveWindow())
        option->state |= QStyle::State_Active;
    if (isEnabled())
        option->state |= QStyle::State_Enabled;
}

/*!
  \reimp
 */
bool QtGraphicsListView::event(QEvent *event)
{
    Q_D(QtGraphicsListView);
    // forward events to the controller
    if (d->controller && d->controller->processEvent(event, transform())) {
        event->accept();
        return true;
    }
    // delayed layout mechanism
    if (event->type() == QEvent::Timer && static_cast<QTimerEvent*>(event)->timerId() == d->layoutTimer.timerId())
        doLayout();
    return QGraphicsWidget::event(event);
}

/*!
  \reimp
 */
QVariant QtGraphicsListView::itemChange(GraphicsItemChange change, const QVariant &value)
{
    Q_D(QtGraphicsListView);
    if (change == QGraphicsObject::ItemChildRemovedChange) {
        void *ptr = qvariant_cast<void*>(value);
        for (int i = 0; i < d->items.count(); ++i) {
            if (d->items.at(i).second == ptr) {
                d->items.removeAt(i);
                break;
            }
        }
    }
    return QGraphicsWidget::itemChange(change, value);
}

/*!
  Returns the item creator for this view.
 */
QtGraphicsListViewItemCreatorBase *QtGraphicsListView::itemCreator() const
{
    Q_D(const QtGraphicsListView);
    return d->creator;
}

/*!
  Sets the given item \a creator on the view.
  The view will take ownership of the creator.
 */
void QtGraphicsListView::setItemCreator(QtGraphicsListViewItemCreatorBase *creator)
{
    Q_D(QtGraphicsListView);
    if (d->creator)
        delete d->creator;
    d->creator = creator;
    d->items.clear();
}

/*!
  Returns a view item for the given \a index if such an item exists,
  otherwise returns null.
 */
QGraphicsObject* QtGraphicsListView::itemForIndex(int index) const
{
    Q_D(const QtGraphicsListView);
    if (!d->items.isEmpty()) {
        const int firstIndex = d->items.first().first;
        const int lastIndex = d->items.last().first;
        if (firstIndex <= index && index <= lastIndex) {
            QGraphicsObject *item = d->items.at(index - firstIndex).second;
            Q_ASSERT(item);
            return item;
        }
    }
    return 0;
}

QT_END_NAMESPACE

#include "moc_qgraphicslistview.cpp"
