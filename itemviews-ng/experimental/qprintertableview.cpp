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

#include "qprintertableview.h"
#include "qprintertableview_p.h"

#ifndef QT_NO_PRINTER

#include "../qgraphicstableview.h"
#include "../qtablemodelinterface.h"
#include "../qgraphicsheader.h"

#include <qgraphicsscene.h>
#include <qpainter.h>
#include <qtextdocument.h>
#include <qabstracttextdocumentlayout.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE

// #define DEBUG_TABLES

QtPrinterTableViewPrivate::QtPrinterTableViewPrivate(QPrinter *p)
    : q_ptr(0),
    ownsPrinter(p == 0),
    printer(ownsPrinter ? new QPrinter : p),
    model(0),
    orientation(Qt::Vertical),
    header(0),
    footer(0)
{
    headerFooterSpacing = 5; // in mm
}

QtPrinterTableViewPrivate::~QtPrinterTableViewPrivate()
{
    delete header;
    delete footer;
    if (ownsPrinter)
        delete printer;
}

qreal QtPrinterTableViewPrivate::printHeader(QPainter &painter)
{
    if (header == 0)
        return 0;
    header->drawContents(&painter);
    return header->size().height() + headerFooterSpacingInPixels();
}

qreal QtPrinterTableViewPrivate::printFooter(QPainter &painter)
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

QTextDocument *QtPrinterTableViewPrivate::getOrCreateHeader() const
{
    if (header == 0) {
        header = new QTextDocument();
        header->setUndoRedoEnabled(false);
        header->setDocumentMargin(0);
    }
    return header;
}

QTextDocument *QtPrinterTableViewPrivate::getOrCreateFooter() const
{
    if (footer == 0) {
        footer = new QTextDocument();
        footer->setUndoRedoEnabled(false);
        footer->setDocumentMargin(0);
    }
    return footer;
}

/*!
  \class QtPrinterTableView
  \brief QtPrinterTableView allows you to print a table model.
*/

QtPrinterTableView::QtPrinterTableView(QtTableModelInterface *model, QtGraphicsHeader *verticalHeader, QtGraphicsHeader *horizontalHeader)
    : d_ptr(new QtPrinterTableViewPrivate(0))
{
    Q_ASSERT(model);
    d_ptr->q_ptr = this;
    d_ptr->model = model;
    d_ptr->horizontalHeader = horizontalHeader;
    d_ptr->verticalHeader = verticalHeader;
}

QtPrinterTableView::QtPrinterTableView(QPrinter *printer, QtTableModelInterface *model, QtGraphicsHeader *verticalHeader, QtGraphicsHeader *horizontalHeader)
    : d_ptr(new QtPrinterTableViewPrivate(printer))
{
    Q_ASSERT(model);
    d_ptr->q_ptr = this;
    d_ptr->model = model;
    d_ptr->horizontalHeader = horizontalHeader;
    d_ptr->verticalHeader = verticalHeader;
}

QtPrinterTableView::~QtPrinterTableView()
{
    delete d_ptr;
}

QPrinter *QtPrinterTableView::printer()
{
    Q_D(QtPrinterTableView);
    return d->printer;
}

void QtPrinterTableView::setOrientation(Qt::Orientation orientation)
{
    Q_D(QtPrinterTableView);
    d->orientation = orientation;
}

Qt::Orientation QtPrinterTableView::orientation() const
{
    Q_D(const QtPrinterTableView);
    return d->orientation;
}

QTextDocument *QtPrinterTableView::headerDocument()
{
    Q_D(QtPrinterTableView);
    return d->getOrCreateHeader();
}

QTextDocument *QtPrinterTableView::footerDocument()
{
    Q_D(QtPrinterTableView);
    return d->getOrCreateFooter();
}

void QtPrinterTableView::setHeaderText(const QString &header)
{
    Q_D(QtPrinterTableView);
    d->getOrCreateHeader()->setPlainText(header);
}

void QtPrinterTableView::setFooterText(const QString &footer)
{
    Q_D(QtPrinterTableView);
    d->getOrCreateFooter()->setPlainText(footer);
}

QString QtPrinterTableView::headerText() const
{
    Q_D(const QtPrinterTableView);
    return d->getOrCreateHeader()->toPlainText();
}

QString QtPrinterTableView::footerText() const
{
    Q_D(const QtPrinterTableView);
    if (d->footer == 0)
        return QString();
    return d->footer->toPlainText();
}

extern int qt_defaultDpiY();
extern int qt_defaultDpiX();

bool QtPrinterTableView::print()
{
    Q_D(QtPrinterTableView);
    if (! d->printer->isValid()) {
        qWarning() << "QtPrinterTableView::print: printer not valid, please setup the printer before calling print";
        return false;
    }

    // next use a view just for the filling of the options...
    QGraphicsScene scene;
    QtGraphicsTableView view;
    scene.addItem(&view);
    view.setModel(d->model);


    class QTableHeaderDataProvider2 : public QtGraphicsHeaderDataProvider
    {
    public:
        QTableHeaderDataProvider2(QtTableModelInterface *model_) : model(model_) {}
        QHash<int,QVariant> data(int logicalIndex, const QList<int> &roles) const
        {
            // ### call the model - temporary implementation
            QHash<int,QVariant> hash;
            for (int i = 0; i < roles.count(); ++i)
                if (roles.at(i) == Qt::DisplayRole)
                    hash.insert(Qt::DisplayRole, logicalIndex + 1);
            return hash;
        }
        QtTableModelInterface *model;
    };

    if (d->horizontalHeader) {
        QtGraphicsHeader *header = new QtGraphicsHeader(Qt::Horizontal, &view);
        header->setDataProvider(new QTableHeaderDataProvider2(d->model));
        header->copySections(*d->horizontalHeader);
        view.setHorizontalHeader(header);
    }
    if (d->verticalHeader) {
        QtGraphicsHeader *header = new QtGraphicsHeader(Qt::Vertical, &view);
        header->setDataProvider(new QTableHeaderDataProvider2(d->model));
        header->copySections(*d->verticalHeader);
        view.setVerticalHeader(header);
    }

    QPainter painter;
    if (!painter.begin(d->printer)) {
        qWarning() << "QtPrinterTableView::print: printer fails to begin(), sorry can't print";
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

    const qreal headerSize = d->printHeader(painter);
    const qreal footerSize = d->printFooter(painter);
    const bool vertical = d->orientation == Qt::Vertical;
    const qreal scaleX = d->printer->logicalDpiX() / (qreal) qt_defaultDpiX();
    const qreal scaleY = d->printer->logicalDpiY() / (qreal) qt_defaultDpiY();
    painter.scale(scaleX, scaleY);

    const QRect rect = d->printer->pageRect();
    QSizeF visibleSize(rect.width() / scaleX, rect.height() / scaleY);
    if (vertical)
        visibleSize = QSizeF(visibleSize.width(), visibleSize.height() - headerSize - footerSize);
    else // then rotate it.
        visibleSize = QSizeF(visibleSize.height() - headerSize - footerSize, visibleSize.width());

    view.setGeometry(0, 0, visibleSize.width(), visibleSize.height());
    view.setHorizontalOffset(0);

    int row = 0;
    int column;
    bool first = true;
    qreal offsetInColumn = 0; // for those columns too wide and thus split over more than one page
    while (row < d->model->rowCount()) {
        column = 0;
        view.setFirstRow(row);
        qreal height = visibleSize.height();
        while (true) {
            const qreal rowHeight = view.rowHeight(row);
            if (height - rowHeight < 0)
                break;
            if (row >= d->model->rowCount())
                break;
            ++row;
            height -= rowHeight;
        }
        while (column < d->model->columnCount()) {
            if (!first) {
                d->printer->newPage();
                d->printHeader(painter);
                d->printFooter(painter);
            }
            first = false;
            view.setFirstColumn(column);
            view.setHorizontalOffset(offsetInColumn);

            qreal width = visibleSize.width();
            while (true) {
                const qreal columnWidth = view.columnWidth(column);
                if (width == visibleSize.width() && columnWidth - offsetInColumn > visibleSize.width()) {
                    // the column doesn't fit on a page. Lets split it.
                    offsetInColumn += visibleSize.width();
                    width = 0;
                    break;
                } else if (offsetInColumn > 0) { // we still have a part of a split column to print!
                    width -= columnWidth - offsetInColumn;
                    offsetInColumn = 0;
                    ++column;
                    continue;
                }

                if (width - columnWidth + offsetInColumn < 0)
                    break;
                if (column >= d->model->columnCount())
                    break;
                ++column;
                width -= columnWidth;
            }
            view.doLayout();

            painter.save(); // for each page
            painter.translate(0, headerSize);
            if (!vertical) {
                painter.translate(0, visibleSize.width());
                painter.rotate(-90);
            }
#ifdef DEBUG_TABLES
            painter.setPen(QPen(QColor(Qt::green)));
            painter.drawRect(QRectF(0, 0, visibleSize.width() - width, visibleSize.height() - height));
#endif
            painter.setClipRect(QRectF(0, 0, visibleSize.width() - width, visibleSize.height() - height));
            // find and paint children which are the cells
            QList<QGraphicsItem*> items = scene.items(view.rect());
            QList<QGraphicsItem*>::Iterator iter = items.begin();
            for (;iter != items.end(); ++iter) {
                QGraphicsItem *parent = *iter;
                while (parent != &view && parent != 0) // only draw children of our view
                    parent = parent->parentItem();
                if (parent == 0)
                    continue;
                if (!(*iter)->isVisible())
                    continue;
                painter.save();
                painter.translate((*iter)->mapToItem(&view, QPointF()));
#ifdef DEBUG_TABLES
                QGraphicsWidget *w = dynamic_cast<QGraphicsWidget*>(*iter);
                if (w) {
                    painter.save();
                    painter.setPen(QPen(QColor(Qt::red)));
                    painter.drawRect(QRectF(QPointF(), w->size()));
                    painter.restore();
                }
#endif
                (*iter)->paint(&painter, 0);
                painter.restore();
            }
            painter.restore(); // for each page
        }
    }
    return true;
}

QT_END_NAMESPACE

#endif
