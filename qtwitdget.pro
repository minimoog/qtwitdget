QT += network sql declarative

RESOURCES = qtwitdget.qrc
win32:RC_FILE = qtwitdget.rc

# Add more folders to ship with the application, here
folder_01.source = qml/QTwitdget
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML2_IMPORT_PATH =

# Avoid auto screen rotation
#DEFINES += ORIENTATIONLOCK

# Needs to be defined for Symbian
#DEFINES += NETWORKACCESS

symbian:TARGET.UID3 = 0xEDDF745F

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    qtweetlib/qtweetuserstream.cpp \
    qtweetlib/qtweetuser.cpp \
    qtweetlib/qtweetstatusupdate.cpp \
    qtweetlib/qtweetstatus.cpp \
    qtweetlib/qtweetsearchresult.cpp \
    qtweetlib/qtweetsearchpageresults.cpp \
    qtweetlib/qtweetplace.cpp \
    qtweetlib/qtweetnetbase.cpp \
    qtweetlib/qtweetmentions.cpp \
    qtweetlib/qtweetlist.cpp \
    qtweetlib/qtweethometimeline.cpp \
    qtweetlib/qtweetgeocoord.cpp \
    qtweetlib/qtweetgeoboundingbox.cpp \
    qtweetlib/qtweetentityusermentions.cpp \
    qtweetlib/qtweetentityurl.cpp \
    qtweetlib/qtweetentityhashtag.cpp \
    qtweetlib/qtweetdmstatus.cpp \
    qtweetlib/qtweetdirectmessages.cpp \
    qtweetlib/qtweetdirectmessagenew.cpp \
    qtweetlib/qtweetconvert.cpp \
    qtweetlib/qtweetaccountverifycredentials.cpp \
    qtweetlib/oauthtwitter.cpp \
    qtweetlib/oauth.cpp \
    tweetqmllistmodel.cpp \
    shortenedurl.cpp \
    mentionsqmllistmodel.cpp \
    mainwindow.cpp \
    qtweetlib/qtweetsearch.cpp \
    searchqmllistmodel.cpp \
    qtweetlib/qtweetusershow.cpp \
    userinfo.cpp \
    qtweetlib/qtweetusertimeline.cpp \
    usertimelinelistmodel.cpp \
    conversationlistmodel.cpp \
    qtweetlib/qtweetfriendshipdestroy.cpp \
    qtweetlib/qtweetfriendshipcreate.cpp \
    qtweetlib/qtweetfavoritescreate.cpp \
    shorturlsitem.cpp \
    userlogins.cpp \
    namsingleton.cpp \
    qtweetlib/qtweetstatusesdestroy.cpp \
    qtweetlib/qtweetentitymedia.cpp \
    qtweetlib/qtweetstatusesshowid.cpp

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES +=

HEADERS += \
    qtweetlib/qtweetuserstream.h \
    qtweetlib/qtweetuser.h \
    qtweetlib/qtweetstatusupdate.h \
    qtweetlib/qtweetstatus.h \
    qtweetlib/qtweetsearchresult.h \
    qtweetlib/qtweetsearchpageresults.h \
    qtweetlib/qtweetplace.h \
    qtweetlib/qtweetnetbase.h \
    qtweetlib/qtweetmentions.h \
    qtweetlib/qtweetlist.h \
    qtweetlib/qtweetlib_global.h \
    qtweetlib/qtweethometimeline.h \
    qtweetlib/qtweetgeocoord.h \
    qtweetlib/qtweetgeoboundingbox.h \
    qtweetlib/qtweetentityusermentions.h \
    qtweetlib/qtweetentityurl.h \
    qtweetlib/qtweetentityhashtag.h \
    qtweetlib/qtweetdmstatus.h \
    qtweetlib/qtweetdirectmessages.h \
    qtweetlib/qtweetdirectmessagenew.h \
    qtweetlib/qtweetconvert.h \
    qtweetlib/qtweetaccountverifycredentials.h \
    qtweetlib/oauthtwitter.h \
    qtweetlib/oauth.h \
    tweetqmllistmodel.h \
    shortenedurl.h \
    mentionsqmllistmodel.h \
    mainwindow.h \
    qtweetlib/qtweetsearch.h \
    searchqmllistmodel.h \
    qtweetlib/qtweetusershow.h \
    userinfo.h \
    qtweetlib/qtweetusertimeline.h \
    usertimelinelistmodel.h \
    conversationlistmodel.h \
    qtweetlib/qtweetfriendshipdestroy.h \
    qtweetlib/qtweetfriendshipcreate.h \
    qtweetlib/qtweetfavoritescreate.h \
    shorturlsitem.h \
    userlogins.h \
    namsingleton.h \
    qtweetlib/qtweetstatusesdestroy.h \
    qtweetlib/qtweetentitymedia.h \
    qtweetlib/qtweetstatusesshowid.h

ICON = qtwitdget.icns
