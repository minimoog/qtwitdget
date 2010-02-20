/* Copyright (c) 2009, Antonie Jovanoski
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
 
#ifndef QTWITVIEW_H
#define QTWITVIEW_H

#include <QGraphicsView>
#include <QScrollBar>

class QPropertyAnimation;

class QTwitView : public QGraphicsView
{
	Q_OBJECT
    Q_PROPERTY(int valueVertScrollBar READ valueVertScrollbar WRITE setValueVertScrollbar)
public:
	QTwitView(QWidget *parent = 0);

    /* sets/gets value of vertical scrollbar */
    void setValueVertScrollbar(int value);
    int valueVertScrollbar() const;

    /* since horizontal scrollbar is disabled, and statuses are resizable to get whole x sizing
     * we need just y position to focus to y position of status
     *
     * y should be y position of status to be centered (top left corner)
     * uses scrollbar for moving
     */
    void moveToPointAnim(qreal y);

signals:
	void scrollBarMaxPos(bool maxPos);
protected:
	void resizeEvent(QResizeEvent *e);
private slots:
	void scrollBarPos(int pos);

private:
    QPropertyAnimation* m_scrbarAnimation;
};
 
#endif //QTWITVIEW_H
