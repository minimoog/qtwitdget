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

#ifndef QTTREEDEFAULTMODEL_H
#define QTTREEDEFAULTMODEL_H

#include "qtreemodelinterface.h"
#include <QtGui/qicon.h>
#include <QtGui/qfont.h>
#include <QtGui/qbrush.h>
#include <QtCore/qstring.h>
#include <QtCore/qiodevice.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

struct QtTreeDefaultItemPrivate;

class Q_ITEMVIEWSNG_EXPORT QtTreeDefaultItem
{
public:
    QtTreeDefaultItem(QtTreeDefaultItem *parent = 0);
    QtTreeDefaultItem(const QString &text, QtTreeDefaultItem *parent = 0);
    virtual ~QtTreeDefaultItem();

    // ### should these be part of the public API ?
    QtTreeDefaultItem *parent() const;
    QtTreeDefaultItem *firstChild() const;
    QtTreeDefaultItem *next() const;
    QtTreeDefaultItem *previous() const;

    int columnCount() const;
    void setColumnCount(int count);

    const QList<QtTreeDefaultItem*> &children() const;
    void insertChild(int i, QtTreeDefaultItem *child);
    void appendChild(QtTreeDefaultItem *child);
    QtTreeDefaultItem *takeChild(int i);
    void removeChild(QtTreeDefaultItem *child);
    
#ifndef QT_NO_DATASTREAM
    virtual void read(QDataStream &in);
    virtual void write(QDataStream &out) const;
#endif

    // data functions
    virtual QHash<QByteArray,QVariant> data(int column, const QList<QByteArray> &roles) const;
    virtual QVariant data(int column, const QByteArray &role) const; // ###
    virtual void setData(const QVariant &data, int column, const QByteArray &role); // ### is the order of args ok ?

    // ### we should add
    // Qt::TextRole == Qt::DisplayRole
    // Qt::IconRole == Qt::DecorationRole
    
    inline QString text(int column = 0) const { return qvariant_cast<QString>(data(column, "DisplayRole")); }
    inline void setText(const QString &text, int column = 0) { setData(text, column, "DisplayRole"); }

    inline QIcon icon(int column = 0) const { return qvariant_cast<QIcon>(data(column, "DecorationRole")); }
    inline void setIcon(const QIcon &icon, int column = 0) { setData(icon, column, "DecorationRole"); }

    inline QString statusTip(int column = 0) const { return qvariant_cast<QString>(data(column, "StatusTipRole")); }
    inline void setStatusTip(const QString &tip, int column = 0) { setData(tip, column, "StatusTipRole"); }
#ifndef QT_NO_TOOLTIP
    inline QString toolTip(int column = 0) const { return qvariant_cast<QString>(data(column, "ToolTipRole")); }
    inline void setToolTip(const QString &tip, int column = 0) { setData(tip, column, "ToolTipRole"); }
#endif
#ifndef QT_NO_WHATSTHIS
    inline QString whatsThis(int column = 0) const { return qvariant_cast<QString>(data(column, "WhatsThisRole")); }
    inline void setWhatsThis(const QString &what, int column = 0) { setData(what, column, "WhatsThisRole"); }
#endif
    inline QFont font(int column = 0) const { return qvariant_cast<QFont>(data(column, "FontRole")); }
    inline void setFont(const QFont &font, int column = 0) { setData(font, column, "FontRole"); }

    inline int textAlignment(int column = 0) const { return qvariant_cast<int>(data(column, "TextAlignmentRole")); }
    inline void setTextAlignment(int alignment, int column = 0) { setData(alignment, column, "TextAlignmentRole"); }

    inline QBrush background(int column = 0) const { return qvariant_cast<QBrush>(data(column, "BackgroundRole")); }
    inline void setBackground(const QBrush &brush, int column = 0) { setData(brush, column, "BackgroundRole"); }

    inline QBrush foreground(int column = 0) const { return qvariant_cast<QBrush>(data(column, "ForegroundRole")); }
    inline void setForeground(const QBrush &brush, int column = 0) { setData(brush, column, "ForegroundRole"); }

    inline Qt::CheckState checkState(int column = 0) const { return static_cast<Qt::CheckState>(data(column, "CheckStateRole").toInt()); }
    inline void setCheckState(Qt::CheckState state, int column = 0) { setData(state, column, "CheckStateRole"); }

    inline QSizeF sizeHint(int column = 0) const { return qvariant_cast<QSizeF>(data(column, "SizeHintRole")); }
    inline void setSizeHint(const QSizeF &size, int column = 0) { setData(size, column, "SizeHintRole"); }

private:
    QtTreeDefaultItemPrivate *d_ptr;
    friend class QtTreeDefaultModel;
};

class QtTreeDefaultModelPrivate;

class Q_ITEMVIEWSNG_EXPORT QtTreeDefaultModel : public QtTreeModelInterface<QtTreeDefaultItem*>
{
    Q_OBJECT
public:
    explicit QtTreeDefaultModel(QObject *parent = 0);
    virtual ~QtTreeDefaultModel();

    QtTreeDefaultModel::iterator itemIterator(QtTreeDefaultItem *item); // ###
    QtTreeDefaultItem *item(iterator &iterator);
    QtTreeDefaultItem *rootItem() const;

protected:
    // model interface
    QtTreeDefaultItem *firstChild(QtTreeDefaultItem *item) const;
    QtTreeDefaultItem *nextSibling(QtTreeDefaultItem *item) const;
    QtTreeDefaultItem *previousSibling(QtTreeDefaultItem *item) const;
    QtTreeDefaultItem *parentItem(QtTreeDefaultItem *item) const;
    QHash<QByteArray,QVariant> data(QtTreeDefaultItem *item, int column, const QList<QByteArray> &roles) const;
    bool setData(const QVariant &data, QtTreeDefaultItem *item, int column, const QByteArray &role);
    bool isValid(QtTreeDefaultItem *item) const;
    int columnCount(QtTreeDefaultItem *item) const;

    // optimization
    bool hasChildren(QtTreeDefaultItem *item) const;
    bool hasNextSibling(QtTreeDefaultItem *item) const;
    bool hasPreviousSibling(QtTreeDefaultItem *item) const;
    bool hasParent(QtTreeDefaultItem *item) const;

    // other
    void itemInserted(QtTreeDefaultItem *item);
    void itemRemoved(QtTreeDefaultItem *item);
    void itemChanged(QtTreeDefaultItem *item, const QList<QByteArray> &roles);

protected:
    QtTreeDefaultModelPrivate *d_ptr;
    
private:
    friend class QtTreeDefaultItem;
    Q_DECLARE_PRIVATE(QtTreeDefaultModel)
    Q_DISABLE_COPY(QtTreeDefaultModel)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTTREEDEFAULTMODEL_H
