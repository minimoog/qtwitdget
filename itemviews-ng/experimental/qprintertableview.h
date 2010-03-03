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
#ifndef QTPRINTERTABLEVIEW_H
#define QTPRINTERLTABLEIEW_H

#include <QtGui/qprinter.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtPrinterTableViewPrivate;
class QtTableModelInterface;
class QtGraphicsHeader;
class QTextDocument;

class Q_ITEMVIEWSNG_EXPORT QtPrinterTableView
{
public:
    explicit QtPrinterTableView(QPrinter *printer, QtTableModelInterface *model, QtGraphicsHeader *verticalHeader = 0, QtGraphicsHeader *horizontalHeader = 0);
    explicit QtPrinterTableView(QtTableModelInterface *model, QtGraphicsHeader *verticalHeader = 0, QtGraphicsHeader *horizontalHeader = 0);
    ~QtPrinterTableView();

    QPrinter *printer();
    bool print();

    void setOrientation(Qt::Orientation orientation);
    Qt::Orientation orientation() const;

    QTextDocument *headerDocument();
    QTextDocument *footerDocument();

    void setHeaderText(const QString &header);
    void setFooterText(const QString &footer);
    QString headerText() const;
    QString footerText() const;

    // TODO add the setter of the creator object, add static print object

protected:
    QtPrinterTableViewPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtPrinterTableView)
    Q_DISABLE_COPY(QtPrinterTableView)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif //QTPRINTERTABLEVIEW_H
