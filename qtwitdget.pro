TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += network \
    sql \
    declarative

# win32:LIBS += libeay32.lib
# unix:LIBS += -lcrypto
HEADERS += \
    shortenedurl.h \
    mainwindow.h \
    langchangedialog.h \
    groupdialog.h \
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
    qtwit/hometimeline.h \
    oauth/oauth.h \
    oauth/oauthtwitter.h \
    signalwaiter.h \
    qtwit/qtwituserdata.h \
    qtwit/qtwituser.h \
    qtwit/qtwitdmstatusdata.h \
    qtwit/qtwitdmstatus.h \
    qtwit/qtwitdirectmessages.h \
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
    shortenedurl.cpp \
    mainwindow.cpp \
    langchangedialog.cpp \
    groupdialog.cpp \
    qtwit/qtwitbase.cpp \
    qtwit/qtwitupdate.cpp \
    qtwit/qtwitstatus.cpp \
    qtwit/qtwitverifycredentials.cpp \
    qtwit/qtwitfriends.cpp \
    qtwit/qtwitdestroy.cpp \
    qtwit/qtwitfavorites.cpp \
    qtwit/qtwithometimeline.cpp \
    qtwit/hometimeline.cpp \
    oauth/oauth.cpp \
    oauth/oauthtwitter.cpp \
    signalwaiter.cpp \
    qtwit/qtwitdirectmessages.cpp \
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
    qml/TweetList.qml \
    qml/TestButton.qml \
    qml/AuthPage.qml \
    qml/MainScreen.qml \
    qml/TweetUpdate.qml
