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

#include "userlogins.h"
#include <QSettings>

UserLogins::UserLogins()
{
}

QList<UserLoginData> UserLogins::getUserLogins() const
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");

    int size = settings.beginReadArray("logins");
    QList<UserLoginData> userlogins;

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        UserLoginData login;
        login.oauthToken = settings.value("oauth_token").toString();
        login.oauthTokenSecret = settings.value("oauth_token_secret").toString();
        login.id = settings.value("user_id").toLongLong();
        login.screenName = settings.value("user_screenname").toString();

        userlogins.append(login);
    }

    settings.endArray();

    return userlogins;
}

UserLoginData UserLogins::lastLoggedUser() const
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    UserLoginData login;

    int size = settings.beginReadArray("logins");

    if (size == 0)
        // returns empty object
        return login;

    settings.setArrayIndex(size - 1);
    login.oauthToken = settings.value("oauth_token").toString();
    login.oauthTokenSecret = settings.value("oauth_token_secret").toString();
    login.id = settings.value("user_id").toLongLong();
    login.screenName = settings.value("user_screenname").toString();
    settings.endArray();

    return login;
}

// remark, writes user login at last place
void UserLogins::writeUserLogin(const UserLoginData &userlogin)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");

    QList<UserLoginData> userlogins = getUserLogins();

    int index = userlogins.indexOf(userlogin);

    //erase it, if it exists
    if (index != -1) {
        settings.beginWriteArray("logins");
        settings.setArrayIndex(index);
        settings.remove("");
        settings.endArray();
    }

    //tnen, write at last position
    settings.beginWriteArray("logins");
    settings.setArrayIndex(userlogins.count() - 2);
    settings.setValue("oauth_token", userlogin.oauthToken);
    settings.setValue("oauth_token_secret", userlogin.oauthTokenSecret);
    settings.setValue("user_id", userlogin.id);
    settings.setValue("user_screenname", userlogin.screenName);
    settings.endArray();
}

void UserLogins::clearUserLogin(qint64 userid)
{
    QList<UserLoginData> userlogins = getUserLogins();

    UserLoginData fakeUserLoginData;
    fakeUserLoginData.id = userid;

    int index = userlogins.indexOf(fakeUserLoginData);

    if (index != -1) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");

        settings.beginWriteArray("logins");
        settings.setArrayIndex(index);
        settings.remove("");
        settings.endArray();
    }
}
