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

#ifndef QTLISTSELECTIONMANAGER_H
#define QTLISTSELECTIONMANAGER_H

#include "qitemviewsglobal.h"

#include <QtCore/qobject.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Gui)

class QtListModelInterface;
class QtListSelectionManagerPrivate;
class QtListSelectionChangeData;

class Q_ITEMVIEWSNG_EXPORT QtListSelectionChange
{
public:
    QtListSelectionChange();
    QtListSelectionChange(const QtListSelectionChange &other);
    ~QtListSelectionChange();
	QtListSelectionChange &operator=(const QtListSelectionChange &other);
    int index() const;
    int count() const;
    QList<int> indexes() const;
private:
    friend class QtListSelectionManager;
    friend class QtListSelectionManagerPrivate;
    QSharedDataPointer<QtListSelectionChangeData> d;
};

Q_DECLARE_TYPEINFO(QtListSelectionChange, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QtListSelectionChange);

class Q_ITEMVIEWSNG_EXPORT QtListSelectionManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentItem READ currentItem WRITE setCurrentItem)
    Q_PROPERTY(bool anchoredSelectionActive READ isAnchoredSelectionActive WRITE setAnchoredSelectionActive)
    Q_PROPERTY(SelectionMode anchoredSelectionMode READ anchoredSelectionMode WRITE setAnchoredSelectionMode)
public:
    enum SelectionMode {
        Select,
        Deselect,
        Toggle
    };

    QtListSelectionManager(QObject *parent = 0);
    ~QtListSelectionManager();

    QtListModelInterface *model() const;

    int currentItem() const;
    int anchorItem() const;

    bool isSelected(int index) const;
    QList<int> selectedItems() const;

    bool isAnchoredSelectionActive() const;
    void setAnchoredSelectionActive(bool active);

    SelectionMode anchoredSelectionMode() const;
    void setAnchoredSelectionMode(SelectionMode mode);

    void beginAnchoredSelection(int anchor, SelectionMode mode = Select);
    void endAnchoredSelection();

public Q_SLOTS:
    void setCurrentItem(int current);
    void setAnchorItem(int anchor);
    void setSelected(int index, int count = 1, SelectionMode mode = Select);
    void clearSelections();

Q_SIGNALS:
    void currentChanged(int current, int previous);
    void anchorChanged(int anchor, int previous);
    void selectionsChanged(const QtListSelectionChange &change);

protected:
    QtListSelectionManager(QtListSelectionManagerPrivate&, QObject *parent = 0);
    QtListSelectionManagerPrivate *d_ptr;
    void setModel(QtListModelInterface *model);
    void commitAnchoredSelection();

private:
    friend class QtListController;
    friend class QtListSelectionChange;
    Q_DECLARE_PRIVATE(QtListSelectionManager)
    Q_DISABLE_COPY(QtListSelectionManager)
    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_itemsInserted(int index, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsRemoved(int index, int count))
    Q_PRIVATE_SLOT(d_func(), void _q_itemsMoved(int from, int to, int count))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif//QTLISTSELECTIONMANAGER_H
