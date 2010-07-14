TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += network \
    sql \
    declarative

# win32:LIBS += libeay32.lib
# unix:LIBS += -lcrypto
HEADERS += twitstatusedit.h \
    shortenedurl.h \
    mainwindow.h \
    langchangedialog.h \
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
    qtwit/qtwitdestroy.h \
    qtwit/qtwitfavorites.h \
    qtwit/qtwithometimeline.h \
    qtwit/qtwitmentions.h \
    qtwit/hometimeline.h \
    qtwit/mentions.h \
    qtwit/qtwitretweet.h \
    oauth/oauth.h \
    oauth/oauthtwitter.h \
    signalwaiter.h \
    tweetlistmodel.h \
    tweetviewitem.h \
    tweetlistview.h \
    tweetlistmodelunread.h \
    qtwit/qtwituserdata.h \
    qtwit/qtwituser.h \
    qtwit/qtwitdmstatusdata.h \
    qtwit/qtwitdmstatus.h \
    qtwit/qtwitdirectmessages.h \
    listmodelinterface.h \
    listviewinterface.h \
    listviewiteminterface.h \
    qtwit/qtwitnewdirectmessage.h \
    tweetqmllistmodel.h

# QJSON
HEADERS += qjson/json_parser.hh \
    qjson/json_scanner.h \
    qjson/location.hh \
    qjson/parser_p.h \
    qjson/position.hh \
    qjson/qjson_debug.h \
    qjson/stack.hh \
    qjson/parser.h \
    qjson/parserrunnable.h \
    qjson/qobjecthelper.h \
    qjson/serializer.h \
    qjson/serializerrunnable.h \
    qjson/qjson_export.h
SOURCES += main.cpp \
    twitstatusedit.cpp \
    shortenedurl.cpp \
    mainwindow.cpp \
    langchangedialog.cpp \
    qtwitview.cpp \
    groupdialog.cpp \
    sceneitems/pixmapbuttonitem.cpp \
    sceneitems/gradientrectitem.cpp \
    sceneitems/netpixmapitem.cpp \
    sceneitems/statustextitem.cpp \
    qtwit/qtwitbase.cpp \
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
    oauth/oauth.cpp \
    oauth/oauthtwitter.cpp \
    signalwaiter.cpp \
    tweetlistmodel.cpp \
    tweetviewitem.cpp \
    tweetlistview.cpp \
    tweetlistmodelunread.cpp \
    qtwit/qtwitdirectmessages.cpp \
    listmodelinterface.cpp \
    listviewinterface.cpp \
    listviewiteminterface.cpp \
    qtwit/qtwitnewdirectmessage.cpp \
    tweetqmllistmodel.cpp

# QJSON
SOURCES += qjson/json_parser.cc \
    qjson/json_scanner.cpp \
    qjson/parser.cpp \
    qjson/parserrunnable.cpp \
    qjson/qobjecthelper.cpp \
    qjson/serializer.cpp \
    qjson/serializerrunnable.cpp
FORMS += forms/mainwindowform.ui \
    forms/langchangedialogform.ui \
    forms/groupdialogform.ui
TRANSLATIONS += translations/qtwitdget_en.ts \
    translations/qtwitdget_mk.ts
RESOURCES = qtwitdget.qrc
win32:RC_FILE = qtwitdget.rc

OTHER_FILES += \
    qml/Button.qml \
    qml/Tweet.qml \
    qml/TweetList.qml
