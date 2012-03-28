QT += network sql opengl

RESOURCES = qtwitdget.qrc
win32:RC_FILE = qtwitdget.rc

# Add more folders to ship with the application, here
folder_01.source = qml/QTwitdget
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

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
    qtweetlib/qtweetstatusdestroy.cpp \
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
    qtweetlib/qtweetstatusshow.cpp \
    conversationlistmodel.cpp \
    directmessagesqmllistmodel.cpp \
    qtweetlib/qtweetfriendshipdestroy.cpp \
    qtweetlib/qtweetfriendshipcreate.cpp \
    qtweetlib/qtweetfavoritescreate.cpp \
    shorturlsitem.cpp \
    userlogins.cpp \
    namsingleton.cpp \
    qtweetlib/json/qjsonwriter.cpp \
    qtweetlib/json/qjsonvalue.cpp \
    qtweetlib/json/qjsonparser.cpp \
    qtweetlib/json/qjsonobject.cpp \
    qtweetlib/json/qjsondocument.cpp \
    qtweetlib/json/qjsonarray.cpp \
    qtweetlib/json/qjson.cpp \
    qtweetlib/qtweetuserstatusesfriends.cpp \
    qtweetlib/qtweetuserstatusesfollowers.cpp \
    qtweetlib/qtweetusersearch.cpp \
    qtweetlib/qtweetuserlookup.cpp \
    qtweetlib/qtweetstatusretweets.cpp \
    qtweetlib/qtweetstatusretweetedby.cpp \
    qtweetlib/qtweetstatusretweetbyid.cpp \
    qtweetlib/qtweetstatusretweet.cpp \
    qtweetlib/qtweetretweettouser.cpp \
    qtweetlib/qtweetretweettome.cpp \
    qtweetlib/qtweetretweetsofme.cpp \
    qtweetlib/qtweetretweetbyuser.cpp \
    qtweetlib/qtweetretweetbyme.cpp \
    qtweetlib/qtweetpublictimeline.cpp \
    qtweetlib/qtweetlistupdate.cpp \
    qtweetlib/qtweetlistunsubscribe.cpp \
    qtweetlib/qtweetlistsubscriptions.cpp \
    qtweetlib/qtweetlistsubscribers.cpp \
    qtweetlib/qtweetlistsubscribe.cpp \
    qtweetlib/qtweetliststatuses.cpp \
    qtweetlib/qtweetlistshowlist.cpp \
    qtweetlib/qtweetlistmemberships.cpp \
    qtweetlib/qtweetlistgetmembers.cpp \
    qtweetlib/qtweetlistgetlists.cpp \
    qtweetlib/qtweetlistdeletemember.cpp \
    qtweetlib/qtweetlistdeletelist.cpp \
    qtweetlib/qtweetlistcreate.cpp \
    qtweetlib/qtweetlistaddmember.cpp \
    qtweetlib/qtweetgeosimilarplaces.cpp \
    qtweetlib/qtweetgeosearch.cpp \
    qtweetlib/qtweetgeoreversegeocode.cpp \
    qtweetlib/qtweetgeoplaceid.cpp \
    qtweetlib/qtweetgeoplacecreate.cpp \
    qtweetlib/qtweetfriendstimeline.cpp \
    qtweetlib/qtweetfriendsid.cpp \
    qtweetlib/qtweetfollowersid.cpp \
    qtweetlib/qtweetfavoritesdestroy.cpp \
    qtweetlib/qtweetfavorites.cpp \
    qtweetlib/qtweetdirectmessagessent.cpp \
    qtweetlib/qtweetdirectmessagedestroy.cpp \
    qtweetlib/qtweetblocksexists.cpp \
    qtweetlib/qtweetblocksdestroy.cpp \
    qtweetlib/qtweetblockscreate.cpp \
    qtweetlib/qtweetblocksblockingids.cpp \
    qtweetlib/qtweetblocksblocking.cpp \
    qtweetlib/qtweetaccountratelimitstatus.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    qtweetlib/qtweetuserstream.h \
    qtweetlib/qtweetuser.h \
    qtweetlib/qtweetstatusupdate.h \
    qtweetlib/qtweetstatusdestroy.h \
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
    directmessagesqmllistmodel.h \
    mainwindow.h \
    qtweetlib/qtweetsearch.h \
    searchqmllistmodel.h \
    qtweetlib/qtweetusershow.h \
    userinfo.h \
    qtweetlib/qtweetusertimeline.h \
    usertimelinelistmodel.h \
    qtweetlib/qtweetstatusshow.h \
    conversationlistmodel.h \
    qtweetlib/qtweetfriendshipdestroy.h \
    qtweetlib/qtweetfriendshipcreate.h \
    qtweetlib/qtweetfavoritescreate.h \
    shorturlsitem.h \
    userlogins.h \
    namsingleton.h \
    namfactory.h \
    qtweetlib/json/qjsonwriter_p.h \
    qtweetlib/json/qjsonvalue.h \
    qtweetlib/json/qjsonparser_p.h \
    qtweetlib/json/qjsonobject.h \
    qtweetlib/json/qjsonexport.h \
    qtweetlib/json/qjsondocument.h \
    qtweetlib/json/qjsonarray.h \
    qtweetlib/json/qjson_p.h \
    qtweetlib/qtweetuserstatusesfriends.h \
    qtweetlib/qtweetuserstatusesfollowers.h \
    qtweetlib/qtweetusersearch.h \
    qtweetlib/qtweetuserlookup.h \
    qtweetlib/qtweetstatusretweets.h \
    qtweetlib/qtweetstatusretweetedby.h \
    qtweetlib/qtweetstatusretweetbyid.h \
    qtweetlib/qtweetstatusretweet.h \
    qtweetlib/qtweetretweettouser.h \
    qtweetlib/qtweetretweettome.h \
    qtweetlib/qtweetretweetsofme.h \
    qtweetlib/qtweetretweetbyuser.h \
    qtweetlib/qtweetretweetbyme.h \
    qtweetlib/qtweetpublictimeline.h \
    qtweetlib/qtweetlistupdate.h \
    qtweetlib/qtweetlistunsubscribe.h \
    qtweetlib/qtweetlistsubscriptions.h \
    qtweetlib/qtweetlistsubscribers.h \
    qtweetlib/qtweetlistsubscribe.h \
    qtweetlib/qtweetliststatuses.h \
    qtweetlib/qtweetlistshowlist.h \
    qtweetlib/qtweetlistmemberships.h \
    qtweetlib/qtweetlistgetmembers.h \
    qtweetlib/qtweetlistgetlists.h \
    qtweetlib/qtweetlistdeletemember.h \
    qtweetlib/qtweetlistdeletelist.h \
    qtweetlib/qtweetlistcreate.h \
    qtweetlib/qtweetlistaddmember.h \
    qtweetlib/qtweetgeosimilarplaces.h \
    qtweetlib/qtweetgeosearch.h \
    qtweetlib/qtweetgeoreversegeocode.h \
    qtweetlib/qtweetgeoplaceid.h \
    qtweetlib/qtweetgeoplacecreate.h \
    qtweetlib/qtweetfriendstimeline.h \
    qtweetlib/qtweetfriendsid.h \
    qtweetlib/qtweetfollowersid.h \
    qtweetlib/qtweetfavoritesdestroy.h \
    qtweetlib/qtweetfavorites.h \
    qtweetlib/qtweetdirectmessagessent.h \
    qtweetlib/qtweetdirectmessagedestroy.h \
    qtweetlib/qtweetblocksexists.h \
    qtweetlib/qtweetblocksdestroy.h \
    qtweetlib/qtweetblockscreate.h \
    qtweetlib/qtweetblocksblockingids.h \
    qtweetlib/qtweetblocksblocking.h \
    qtweetlib/qtweetaccountratelimitstatus.h

ICON = qtwitdget.icns
