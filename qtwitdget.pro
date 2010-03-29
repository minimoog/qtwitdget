TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += network \
    xml \
    sql
//win32:LIBS += libeay32.lib
//unix:LIBS += -lcrypto
HEADERS += twitstatusedit.h \
    shortenedurl.h \
    mainwindow.h \
    langchangedialog.h \
    qtwitscene.h \
    qtwitsceneunread.h \
    qtwitview.h \
    groupdialog.h \
    sceneitems/pixmapbuttonitem.h \
    sceneitems/gradientrectitem.h \
    sceneitems/netpixmapitem.h \
    sceneitems/statustextitem.h \
    qtwit/qtwitupdate.h \
    qtwit/qtwitstatusdata.h \
    qtwit/qtwitstatus.h \
    qtwit/qtwitrtstatus.h \
    qtwit/qtwitrtstatusdata.h \
    qtwit/qtwitverifycredentials.h \
    qtwit/qtwitfriends.h \
    qtwit/qtwitextuserinfodata.h \
    qtwit/qtwitextuserinfo.h \
    qtwit/qtwitbase.h \
    qtwit/qtwitfriendstimeline.h \
    qtwit/qtwitdestroy.h \
    qtwit/qtwitfavorites.h \
    qtwit/qtwithometimeline.h \
    qtwit/qtwitmentions.h \
    qtwit/hometimeline.h \
    qtwit/mentions.h \
    qtwit/qtwitretweet.h \
    xml/xmlreaderextuserinfo.h \
    xml/xmlreaderstatus.h \
    xml/xmlreaderextusers.h \
    oauth/oauth.h \
    oauth/oauthtwitter.h \
    oauth/pindialog.h \
    sceneitems/statusitemgroup.h \
    signalwaiter.h \
    tweetlistmodel.h \
    tweetviewitem.h \
    tweetlistview.h \
    tweetlistmodelabstract.h
SOURCES += main.cpp \
    twitstatusedit.cpp \
    shortenedurl.cpp \
    mainwindow.cpp \
    langchangedialog.cpp \
    qtwitscene.cpp \
    qtwitsceneunread.cpp \
    qtwitview.cpp \
    groupdialog.cpp \
    sceneitems/pixmapbuttonitem.cpp \
    sceneitems/gradientrectitem.cpp \
    sceneitems/netpixmapitem.cpp \
    sceneitems/statustextitem.cpp \
    qtwit/qtwitbase.cpp \
    qtwit/qtwitfriendstimeline.cpp \
    qtwit/qtwitupdate.cpp \
    qtwit/qtwitstatus.cpp \
    qtwit/qtwitverifycredentials.cpp \
    qtwit/qtwitfriends.cpp \
    qtwit/qtwitdestroy.cpp \
    qtwit/qtwitfavorites.cpp \
    qtwit/qtwithometimeline.cpp \
    qtwit/qtwitmentions.cpp \
    qtwit/hometimeline.cpp \
    qtwit/mentions.cpp \
    qtwit/qtwitretweet.cpp \
    xml/xmlreaderstatus.cpp \
    xml/xmlreaderextuserinfo.cpp \
    xml/xmlreaderextusers.cpp \
    oauth/oauth.cpp \
    oauth/oauthtwitter.cpp \
    oauth/pindialog.cpp \
    sceneitems/statusitemgroup.cpp \
    signalwaiter.cpp \
    tweetlistmodel.cpp \
    tweetviewitem.cpp \
    tweetlistview.cpp \
    tweetlistmodelabstract.cpp
FORMS += forms/mainwindowform.ui \
    forms/langchangedialogform.ui \
    forms/groupdialogform.ui \
    forms/pindialogform.ui
TRANSLATIONS += translations/qtwitdget_en.ts \
    translations/qtwitdget_mk.ts
RESOURCES = qtwitdget.qrc
win32:RC_FILE = qtwitdget.rc
