TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

QT += network xml sql

win32:LIBS += libeay32.lib
unix:LIBS += -lcrypto

HEADERS +=  imagedownloader.h \
			texteditwithlimit.h \
			shortenedurl.h \
			mainwindow.h \
			langchangedialog.h \
			twittabgroup.h \
			qtwitdget.h \
			qtwit/qtwitupdate.h \
			qtwit/qtwitstatusdata.h \
			qtwit/qtwitstatus.h \
			qtwit/qtwitverifycredentials.h \
			qtwit/qtwitfriends.h \
			qtwit/qtwitextuserinfodata.h \
			qtwit/qtwitextuserinfo.h \
			qtwit/qtwitbase.h \
			qtwit/qtwitfriendstimeline.h \
			qtwit/qtwitdestroy.h \
			xml/xmlreaderextuserinfo.h \
			xml/xmlreaderstatus.h \
			oauth/oauth.h \
			oauth/oauthtwitter.h 
			
SOURCES +=  main.cpp \
			imagedownloader.cpp \
			texteditwithlimit.cpp \
			shortenedurl.cpp \
			mainwindow.cpp \
			langchangedialog.cpp \
			qtwitdget.cpp \
			qtwit/qtwitbase.cpp \
			qtwit/qtwitfriendstimeline.cpp \
			qtwit/qtwitupdate.cpp \
			qtwit/qtwitstatus.cpp \
			qtwit/qtwitverifycredentials.cpp \
			qtwit/qtwitfriends.cpp \
			qtwit/qtwitdestroy.cpp \
			xml/xmlreaderstatus.cpp \
			xml/xmlreaderextuserinfo.cpp \
			oauth/oauth.cpp \
			oauth/oauthtwitter.cpp 
			
FORMS += 	forms/mainwindowform.ui \
			forms/langchangedialogform.ui
			
TRANSLATIONS += translations/qtwitdget_en.ts \
				translations/qtwitdget_mk.ts

RESOURCES = qtwitdget.qrc

win32:RC_FILE = qtwitdget.rc
