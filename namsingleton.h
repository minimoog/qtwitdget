/* Copyright (c) 2011, Antonie Jovanoski
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

#ifndef NAMSINGLETON_H
#define NAMSINGLETON_H

#include <QNetworkAccessManager>
#include <QMutex>

/**
 *  Singleton for QNetworkAccessManager
 */
class NAMSingleton
{
public:
    static NAMSingleton* instance() {
        static QMutex mutex;
        if (!m_namInstance) {
            mutex.lock();

            if (!m_namInstance)
                m_namInstance = new NAMSingleton;

            mutex.unlock();
        }

        return m_namInstance;
    }

    QNetworkAccessManager* qnam() const { return m_netManager; }

private:
    NAMSingleton() {
        m_netManager = new QNetworkAccessManager;
    }
    ~NAMSingleton() {
        delete m_netManager;
    }

    NAMSingleton(const NAMSingleton&);
    NAMSingleton& operator=(const NAMSingleton&);

    static NAMSingleton* m_namInstance;
    QNetworkAccessManager *m_netManager;
};

#endif // NAMSINGLETON_H
