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

#include "qprinterlistview.h"
#include "qprinterlistview_p.h"

#ifndef QT_NO_PRINTER

#include "../qlistcontroller.h"
#include "../qlistmodelinterface.h"
#include "../qgraphicslistview.h"

#include <qstyleoption.h>
#include <qpainter.h>
#include <qtextdocument.h>
#include <qabstracttextdocumentlayout.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

QtPrinterListViewPrivate::QtPrinterListViewPrivate(QPrinter *p)
    : q_ptr(0),
    ownsPrinter(p == 0),
    printer(ownsPrinter ? new QPrinter : p),
    model(0),
    orientation(Qt::Vertical),
    header(0),
    footer(0),
    creator(0)
{
    headerFooterSpacing = 5; // in mm
}

QtPrinterListViewPrivate::~QtPrinterListViewPrivate()
{
    delete header;
    delete footer;
    delete creator;
    if (ownsPrinter)
        delete printer;
}

qreal QtPrinterListViewPrivate::printHeader(QPainter &painter)
{
    if (header == 0)
        return 0;
    header->drawContents(&painter);
    return header->size().height() + headerFooterSpacingInPixels();
}

qreal QtPrinterListViewPrivate::printFooter(QPainter &painter)
{
    if (footer == 0)
        return 0;
    qreal pageHeight = printer->pageRect().height();
    const qreal footerSize = footer->size().height();
    painter.translate(0, pageHeight - footerSize);
    footer->drawContents(&painter);
    painter.translate(0, - (pageHeight - footerSize));
    return footerSize + headerFooterSpacingInPixels();
}

QTextDocument *QtPrinterListViewPrivate::getOrCreateHeader() const
{
    if (header == 0) {
        header = new QTextDocument();
        header->setUndoRedoEnabled(false);
        header->setDocumentMargin(0);
    }
    return header;
}

QTextDocument *QtPrinterListViewPrivate::getOrCreateFooter() const
{
    if (footer == 0) {
        footer = new QTextDocument();
        footer->setUndoRedoEnabled(false);
        footer->setDocumentMargin(0);
    }
    return footer;
}

/*!
  \class QtPrinterListView
  \brief QtPrinterListView allows you to print a list model.

  Usage;
  \code
  QtPrinterListView printerView(myListModel);
  QPrintDialog dialog(printerView.printer());
  if (dialog.exec() == QPrintDialog::Accepted) {
    printerView.print();
  }
  \endcode
*/

/*!
  This is an overloaded member function, provided for convenience. It
  differs from the above function only in what argument(s) it accepts. This
  will cause QtPrinterListView to create an internal, default constructed
  QPrinter object, which will be used to print the list.
*/
QtPrinterListView::QtPrinterListView(QtListModelInterface *model)
    : d_ptr(new QtPrinterListViewPrivate(0))
{
    Q_ASSERT(model);
    d_ptr->q_ptr = this;
    d_ptr->model = model;
}

/*!
  Constructs a QtPrinterListView based on printer and with \a model as the
  model that will be printed
*/
QtPrinterListView::QtPrinterListView(QPrinter *printer, QtListModelInterface *model)
    : d_ptr(new QtPrinterListViewPrivate(printer))
{
    Q_ASSERT(model);
    d_ptr->q_ptr = this;
    d_ptr->model = model;
}

/*!
  Destructor
*/
QtPrinterListView::~QtPrinterListView()
{
    delete d_ptr;
}

/*!
  Returns the printer used by this view to print to to allow setting of printer properties.
*/
QPrinter *QtPrinterListView::printer()
{
    Q_D(QtPrinterListView);
    return d->printer;
}

/*!
  Sets the orientation of the list layout to be the given orientation.
  The orientation defines the print direction of the list.
*/
void QtPrinterListView::setOrientation(Qt::Orientation orientation)
{
    Q_D(QtPrinterListView);
    d->orientation = orientation;
}

/*!
  Returns the orientation for this list.
  The orientation defines the print direction of the list.
 */
Qt::Orientation QtPrinterListView::orientation() const
{
    Q_D(const QtPrinterListView);
    return d->orientation;
}

/*!
  Returns the text document used for each page header.
  \sa setHeaderText()
*/
QTextDocument *QtPrinterListView::headerDocument()
{
    Q_D(QtPrinterListView);
    return d->getOrCreateHeader();
}

/*!
  Returns the text document used for each page footer.
  \sa setFooterText()
*/
QTextDocument *QtPrinterListView::footerDocument()
{
    Q_D(QtPrinterListView);
    return d->getOrCreateFooter();
}

/*!
  Sets the header on all pages to have the \a header text.
  This replaces any content that the header had previously.
  \sa headerDocument()
*/
void QtPrinterListView::setHeaderText(const QString &header)
{
    Q_D(QtPrinterListView);
    d->getOrCreateHeader()->setPlainText(header);
}

/*!
  Sets the footer on all pages to have the \a footer text.
  This replaces any content that the footer had previously.
  \sa footerDocument()
*/
void QtPrinterListView::setFooterText(const QString &footer)
{
    Q_D(QtPrinterListView);
    d->getOrCreateFooter()->setPlainText(footer);
}

/*!
  Return the text set as header.
  \sa headerDocument()
*/
QString QtPrinterListView::headerText() const
{
    Q_D(const QtPrinterListView);
    return d->getOrCreateHeader()->toPlainText();
}

/*!
  Return the text set as footer.
  \sa headerDocument()
*/
QString QtPrinterListView::footerText() const
{
    Q_D(const QtPrinterListView);
    if (d->footer == 0)
        return QString();
    return d->footer->toPlainText();
}

QtGraphicsListViewItemCreatorBase *QtPrinterListView::viewItemCreator() const
{
    Q_D(const QtPrinterListView);
    return d->creator;
}

/*!
  note that ownership is taken.
*/
void QtPrinterListView::setViewItemCreator(QtGraphicsListViewItemCreatorBase *creator)
{
    Q_D(QtPrinterListView);
    delete d->creator;
    d->creator = creator;
}

extern int qt_defaultDpiY();
extern int qt_defaultDpiX();

/*!
  Start the actual printing using the printer() options.
  Returns true if printing was successful, or false otherwise.
*/
bool QtPrinterListView::print()
{
    Q_D(QtPrinterListView);
    if (! d->printer->isValid()) {
        qWarning() << "QtPrinterListView::print: printer not valid, please setup the printer before calling print";
        return false;
    }

    // next use a view just for the filling of the options...
    QtGraphicsListView view;
    view.setModel(d->model);

    QPainter painter;
    if (!painter.begin(d->printer)) {
        qWarning() << "QtPrinterListView::print: printer fails to begin(), sorry can't print";
        return false;
    }

    // re-layout the header footer to the current page size.
    if (d->header) {
        d->header->documentLayout()->setPaintDevice(d->printer);
        d->header->setPageSize(d->printer->pageRect().size());
    }
    if (d->footer) {
        d->footer->documentLayout()->setPaintDevice(d->printer);
        d->footer->setPageSize(d->printer->pageRect().size());
    }

    QtGraphicsListViewItem *listViewItem;
    if (d->creator)
        listViewItem = d->creator->create(0, &view);
    else
        listViewItem = view.itemCreator()->create(0, &view);
    listViewItem->setCacheMode(QGraphicsItem::NoCache);

    QStyleOptionViewItemV4 widgetOptions;
    QStyleOptionGraphicsItem graphicsViewOptions;
    view.initStyleOption(&widgetOptions);

    const qreal headerSize = d->printHeader(painter);
    const qreal footerSize = d->printFooter(painter);
    const bool vertical = d->orientation == Qt::Vertical;
    const qreal scaleX = d->printer->logicalDpiX() / (qreal) qt_defaultDpiX();
    const qreal scaleY = d->printer->logicalDpiY() / (qreal) qt_defaultDpiY();
    QSizeF constraint = d->printer->pageRect().size();
    // cache the space we have for actual list items.
    const qreal contentSize = vertical ? constraint.height() - headerSize - footerSize : constraint.width();

    view.resize(constraint - QSizeF(0, headerSize + footerSize));

    // a constraint is -1 in the direction we allow items to grow in.
    if (vertical) {
        constraint.setHeight(-1);
    } else {
        constraint.setWidth(-1);
        // in this direction headers/footers eat our precious space
        constraint.setHeight(constraint.height() - headerSize -  - footerSize);
    }

    qreal progression = vertical ? headerSize : 0; // init
    const int max = d->model->count();
    for (int index = 0; index < max; ++index) {
        view.initStyleOption(&widgetOptions, index);

        //QSizeF itemSize = listViewItem->sizeHint(index, &widgetOptions, Qt::PreferredSize, constraint);
        listViewItem->setIndex(index);
        QRectF geometry = view.itemGeometry(index);
        if (vertical)
            geometry.setWidth(constraint.width() / scaleX);
        else
            geometry.setHeight(constraint.height() / scaleY - headerSize - footerSize);
        listViewItem->setGeometry(geometry);
        geometry = listViewItem->geometry();// the listViewItem can be stubborn
        graphicsViewOptions.rect = geometry.toRect();
        if ((vertical && progression - headerSize + geometry.height() > contentSize)
                || (!vertical && progression + geometry.width()> contentSize)) {
            if (! d->printer->newPage()) {
                qWarning() << "QtPrinterListView::print: Failed to start a new page, aborting printing";
                return false;
            }
            progression = vertical ? headerSize : 0; // reset
            d->printHeader(painter);
            d->printFooter(painter);
        }

        painter.save();
        painter.translate(vertical ? 0 : progression,
                vertical ? progression : headerSize);
        painter.scale(scaleX, scaleY);
        listViewItem->paint(&painter, &graphicsViewOptions);
        painter.restore();
        progression += vertical ? geometry.height() * scaleY: geometry.width() * scaleX;
    }
    return true;
}

/*!
  Prints the \a model to the \a printer
*/
//static
void QtPrinterListView::printModel(QPrinter *printer, QtListModelInterface *model, const QString &header, const QString &footer)
{
    QtPrinterListView printerView(printer, model);
    if (!header.isEmpty())
        printerView.setHeaderText(header);
    if (!footer.isEmpty())
        printerView.setFooterText(footer);

    printerView.print();
}

QT_END_NAMESPACE

#endif
