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

#ifndef QTWITUSERDATA_H
#define QTWITUSERDATA_H

#include <QSharedData>
#include <QString>

class QTwitUserData : public QSharedData
{
public:

    QTwitUserData() : id(0) { }

    int id;
    QString name;
    QString screenName;
    QString location;
    QString description;
    QString profileImageUrl;
    QString url;

    //other data is useless this time
};

#endif // QTWITUSERDATA_H

/*
<id>1401881</id>
<name>Doug Williams</name>
<screen_name>dougw</screen_name>
<location>San Francisco, CA</location>
<description>Twitter API Support. Internet, greed, users, dougw and opportunities are my passions.</description>
<profile_image_url>http://s3.amazonaws.com/twitter_production/profile_images/59648642/avatar_normal.png</profile_image_url>
<url>http://www.igudo.com</url>
<protected>false</protected>
<followers_count>1031</followers_count>
<profile_background_color>9ae4e8</profile_background_color>
<profile_text_color>000000</profile_text_color>
<profile_link_color>0000ff</profile_link_color>
<profile_sidebar_fill_color>e0ff92</profile_sidebar_fill_color>
<profile_sidebar_border_color>87bc44</profile_sidebar_border_color>
<friends_count>293</friends_count>
<created_at>Sun Mar 18 06:42:26 +0000 2007</created_at>
<favourites_count>0</favourites_count>
<utc_offset>-18000</utc_offset>
<time_zone>Eastern Time (US & Canada)</time_zone>
<profile_background_image_url>http://s3.amazonaws.com/twitter_production/profile_background_images/2752608/twitter_bg_grass.jpg</profile_background_image_url>
<profile_background_tile>false</profile_background_tile>
<statuses_count>3390</statuses_count>
<notifications>false</notifications>
<following>false</following>
<verified>true</verified>
*/
