/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef LISTMODELINTERFACE_H
#define LISTMODELINTERFACE_H

#include <QObject>
#include <QHash>

class ListViewInterface;

class ListModelInterface : public QObject
{
    Q_OBJECT
public:
    ListModelInterface(QObject *parent = 0);
    void setView(ListViewInterface* view);
    ListViewInterface* view() const;
    virtual int count() const = 0;
    virtual QVariant data(int index) const = 0;
    virtual QHash<QByteArray, QVariant> data(int index, const QList<QByteArray>& roles) const = 0;

signals:
    void itemsInserted(int index, int count);
    void itemsRemoved(int index, int count);
    void itemsMoved(int from, int to, int count);
    void itemsChanged(int index, int count, const QList<QByteArray>& roles);

private:
    ListViewInterface* m_view;
};

#endif // LISTMODELINTERFACE_H
