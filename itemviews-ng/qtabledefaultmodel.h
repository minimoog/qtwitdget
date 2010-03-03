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

#ifndef QTTABLEDEFAULTMODEL_H
#define QTTABLEDEFAULTMODEL_H

#include "qtablemodelinterface.h"
#include <QtGui/qicon.h>
#include <QtGui/qfont.h>
#include <QtGui/qbrush.h>
#include <QtCore/qstring.h>
#include <QtCore/qiodevice.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtTableDefaultItemPrivate;

class Q_ITEMVIEWSNG_EXPORT QtTableDefaultItem
{
public:
    QtTableDefaultItem();
    QtTableDefaultItem(const QString &text);
    virtual ~QtTableDefaultItem();

#ifndef QT_NO_DATASTREAM
    virtual void read(QDataStream &in);
    virtual void write(QDataStream &out) const;
#endif
    
    // data functions

    virtual QHash<QByteArray,QVariant> data(const QList<QByteArray> &roles) const;
    virtual void setData(const QHash<QByteArray,QVariant> &data);

    inline QVariant data(const QByteArray &role) const { return data(QList<QByteArray>() << role).value(role); }
    inline void setData(const QVariant &data, const QByteArray &role) { QHash<QByteArray,QVariant> hash; hash.insert(role, data); setData(hash); }

    inline QString text() const { return qvariant_cast<QString>(data("DisplayRole")); }
    inline void setText(const QString &text) { setData(text, "DisplayRole"); }

    inline QIcon icon() const { return qvariant_cast<QIcon>(data("DecorationRole")); }
    inline void setIcon(const QIcon &icon) { setData(icon, "DecorationRole"); }

    inline QString statusTip() const { return qvariant_cast<QString>(data("StatusTipRole")); }
    inline void setStatusTip(const QString &tip) { setData(tip, "StatusTipRole"); }
#ifndef QT_NO_TOOLTIP
    inline QString toolTip() const { return qvariant_cast<QString>(data("ToolTipRole")); }
    inline void setToolTip(const QString &tip) { setData(tip, "ToolTipRole"); }
#endif
#ifndef QT_NO_WHATSTHIS
    inline QString whatsThis() const { return qvariant_cast<QString>(data("WhatsThisRole")); }
    inline void setWhatsThis(const QString &what) { setData(what, "WhatsThisRole"); }
#endif
    inline QFont font() const { return qvariant_cast<QFont>(data("FontRole")); }
    inline void setFont(const QFont &font) { setData(font, "FontRole"); }

    inline int textAlignment() const { return qvariant_cast<int>(data("TextAlignmentRole")); }
    inline void setTextAlignment(int alignment) { setData(alignment, "TextAlignmentRole"); }

    inline QBrush background() const { return qvariant_cast<QBrush>(data("BackgroundRole")); }
    inline void setBackground(const QBrush &brush) { setData(brush, "BackgroundRole"); }

    inline QBrush foreground() const { return qvariant_cast<QBrush>(data("ForegroundRole")); }
    inline void setForeground(const QBrush &brush) { setData(brush, "ForegroundRole"); }

    inline Qt::CheckState checkState() const { return static_cast<Qt::CheckState>(data("CheckStateRole").toInt()); }
    inline void setCheckState(Qt::CheckState state) { setData(state, "CheckStateRole"); }

    inline QSizeF sizeHint() const { return qvariant_cast<QSizeF>(data("SizeHintRole")); }
    inline void setSizeHint(const QSizeF &size) { setData(size, "SizeHintRole"); }

protected:
    void itemChanged(const QList<QByteArray> &roles);
    
private:
    QtTableDefaultItemPrivate *d_ptr;
    friend class QtTableDefaultModel;
};

class QtTableDefaultModelPrivate;

class Q_ITEMVIEWSNG_EXPORT QtTableDefaultModel : public QtTableModelInterface
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount)
    Q_PROPERTY(int columnCount READ columnCount WRITE setColumnCount)
public:
    explicit QtTableDefaultModel(QObject *parent = 0);
    explicit QtTableDefaultModel(int rows, int columns, QObject *parent = 0);
    virtual ~QtTableDefaultModel();

    void setRowCount(int rows);
    int rowCount() const;

    void setColumnCount(int columns);
    int columnCount() const;

    int itemRow(const QtTableDefaultItem *item) const;
    int itemColumn(const QtTableDefaultItem *item) const;

    QtTableDefaultItem *item(int row, int column) const;
    void setItem(int row, int column, QtTableDefaultItem *item);

    // ### what about header labels ?

    QHash<QByteArray,QVariant> data(int row, int column, const QList<QByteArray> &roles) const;
    bool setData(int row, int column, const QHash<QByteArray,QVariant> &values);

public Q_SLOTS:
    void insertRow(int row);
    void removeRow(int row);
    void moveRow(int fromRow, int toRow);
    void insertColumn(int column);
    void removeColumn(int column);
    void moveColumn(int fromColumn, int toColumn);
    void clearContents();
    void clear();

protected:
    QtTableDefaultModel(QtTableDefaultModelPrivate &, QObject *parent = 0);
    void itemChanged(QtTableDefaultItem *item, const QList<QByteArray> &roles);
    void itemDeleted(QtTableDefaultItem *item);

    QtTableDefaultModelPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QtTableDefaultModel)
    Q_DISABLE_COPY(QtTableDefaultModel)
    friend class QtTableDefaultItem;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTABLEDEFAULTMODEL_H
