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
    twittabgroup.h \
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
    timelinetabwidget.h \
    sceneitems/statusitemgroup.h \
    signalwaiter.h \
    itemviews-ng/qtreewidgetng_p.h \
    itemviews-ng/qtreewidgetng.h \
    itemviews-ng/qtreeselectionmanager_p.h \
    itemviews-ng/qtreeselectionmanager.h \
    itemviews-ng/qtreemodelinterface.h \
    itemviews-ng/qtreemodelbase.h \
    itemviews-ng/qtreemodeladaptor_p.h \
    itemviews-ng/qtreemodeladaptor.h \
    itemviews-ng/qtreedefaultmodel.h \
    itemviews-ng/qtreecontroller_p.h \
    itemviews-ng/qtreecontroller.h \
    itemviews-ng/qtablewidgetng_p.h \
    itemviews-ng/qtablewidgetng.h \
    itemviews-ng/qtableselectionmanager_p.h \
    itemviews-ng/qtableselectionmanager.h \
    itemviews-ng/qtablemodelinterface.h \
    itemviews-ng/qtablemodeladaptor_p.h \
    itemviews-ng/qtablemodeladaptor.h \
    itemviews-ng/qtabledefaultmodel.h \
    itemviews-ng/qtablecontroller_p.h \
    itemviews-ng/qtablecontroller.h \
    itemviews-ng/qsectionspans_p.h \
    itemviews-ng/qlistwidgetng_p.h \
    itemviews-ng/qlistwidgetng.h \
    itemviews-ng/qlistselectionmanager_p.h \
    itemviews-ng/qlistselectionmanager.h \
    itemviews-ng/qlistmodelinterface.h \
    itemviews-ng/qlistmodeladaptor_p.h \
    itemviews-ng/qlistmodeladaptor.h \
    itemviews-ng/qlistfromtreeadaptor_p.h \
    itemviews-ng/qlistfromtreeadaptor.h \
    itemviews-ng/qlistdefaultmodel.h \
    itemviews-ng/qlistcontroller_p.h \
    itemviews-ng/qlistcontroller.h \
    itemviews-ng/qitemviewsglobal.h \
    itemviews-ng/qgraphicstreeview_p.h \
    itemviews-ng/qgraphicstreeview.h \
    itemviews-ng/qgraphicstableview_p.h \
    itemviews-ng/qgraphicstableview.h \
    itemviews-ng/qgraphicslistview_p.h \
    itemviews-ng/qgraphicslistview.h \
    itemviews-ng/qgraphicsheader_p.h \
    itemviews-ng/qgraphicsheader.h \
    itemviews-ng/qdataroles_p.h \
    itemviews-ng/experimental/qkineticlistcontroller_p.h \
    itemviews-ng/experimental/qkineticlistcontroller.h \
    itemviews-ng/experimental/qgraphicsscrollbar_p.h \
    itemviews-ng/experimental/qgraphicsscrollbar.h \
    itemviews-ng/experimental/qgraphicspathview.h \
    itemviews-ng/experimental/qgraphicsgridview.h \
    tweetmodel.h \
    tweetviewitem.h
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
    timelinetabwidget.cpp \
    sceneitems/statusitemgroup.cpp \
    signalwaiter.cpp \
    itemviews-ng/qtreewidgetng.cpp \
    itemviews-ng/qtreeselectionmanager.cpp \
    itemviews-ng/qtreemodeladaptor.cpp \
    itemviews-ng/qtreedefaultmodel.cpp \
    itemviews-ng/qtreecontroller.cpp \
    itemviews-ng/qtablewidgetng.cpp \
    itemviews-ng/qtableselectionmanager.cpp \
    itemviews-ng/qtablemodelinterface.cpp \
    itemviews-ng/qtablemodeladaptor.cpp \
    itemviews-ng/qtabledefaultmodel.cpp \
    itemviews-ng/qtablecontroller.cpp \
    itemviews-ng/qlistwidgetng.cpp \
    itemviews-ng/qlistselectionmanager.cpp \
    itemviews-ng/qlistmodelinterface.cpp \
    itemviews-ng/qlistmodeladaptor.cpp \
    itemviews-ng/qlistfromtreeadaptor.cpp \
    itemviews-ng/qlistdefaultmodel.cpp \
    itemviews-ng/qlistcontroller.cpp \
    itemviews-ng/qgraphicstreeview.cpp \
    itemviews-ng/qgraphicstableview.cpp \
    itemviews-ng/qgraphicslistview.cpp \
    itemviews-ng/qgraphicsheader.cpp \
    itemviews-ng/experimental/qkineticlistcontroller.cpp \
    itemviews-ng/experimental/qgraphicsscrollbar.cpp \
    itemviews-ng/experimental/qgraphicspathview.cpp \
    itemviews-ng/experimental/qgraphicsgridview.cpp \
    tweetmodel.cpp \
    tweetviewitem.cpp
FORMS += forms/mainwindowform.ui \
    forms/langchangedialogform.ui \
    forms/groupdialogform.ui \
    forms/pindialogform.ui
TRANSLATIONS += translations/qtwitdget_en.ts \
    translations/qtwitdget_mk.ts
RESOURCES = qtwitdget.qrc
win32:RC_FILE = qtwitdget.rc
OTHER_FILES += 
DEFINES += Q_ITEMVIEWSNG_EXPORT=""
