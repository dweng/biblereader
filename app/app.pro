#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T11:50:47
#
#-------------------------------------------------

QT       += core gui sql xml network printsupport webkitwidgets

macx {
    QT += macextras
}
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BibleReader
TEMPLATE = app

debug {
    DESTDIR = $$PWD/../bin/debug
}
release {
    DESTDIR = $$PWD/../bin/release
}

SOURCES += main.cpp\
    biblereadercore.cpp \
    biblechapter.cpp \
    bibleverse.cpp \
    biblebook.cpp \
    bibleinfo.cpp \
    bibletreemodel.cpp \
    bibletreeitem.cpp \
    bibleversepos.cpp \
    bibletextbrowser.cpp \
    bibletextdao.cpp \
    biblereaderconfigurator.cpp \
    bibledictdao.cpp \
    bibledictinfo.cpp \
    bibledictionarywidget.cpp \
    biblereadermainwindow.cpp \
    biblecommentarydao.cpp \
    biblesearchwidget.cpp \
    bibletreewidget.cpp \
    biblecommentarywidget.cpp \
    bibletexttabwidget.cpp \
    bibletextblockdata.cpp \
    biblereaderapp.cpp \
    bibletextcomparebrowser.cpp \
    biblesearchresultwidget.cpp \
    biblereaderaboutdlg.cpp \
    bibledicttabwidget.cpp \
    biblecommentaryinfo.cpp \
    biblecommentarytabwidget.cpp \
    biblereaderprojectdialog.cpp \
    biblereaderhtmldelegate.cpp \
    biblereaderconfigdlg.cpp \
    biblereaderconfiggeneralpage.cpp \
    biblereaderresourcemanager.cpp \
    biblereaderresourcemanagerdlg.cpp \
    biblecommentarybrowser.cpp \
    biblereadertooltip.cpp \
    biblexrefsdao.cpp \
    bibleversexref.cpp \
    biblereaderlayout.cpp \
    bibledictbrowser.cpp \
    biblereaderdownloader.cpp \
    biblereaderconfigbiblepage.cpp

HEADERS  += biblereadercore.h \
    biblechapter.h \
    bibleverse.h \
    biblebook.h \
    bibleinfo.h \
    bibletreemodel.h \
    bibletreeitem.h \
    bibleversepos.h \
    bibletextbrowser.h \
    bibletextdao.h \
    biblereaderconfigurator.h \
    bibledictdao.h \
    bibledictinfo.h \
    bibledictionarywidget.h \
    biblereadermainwindow.h \
    biblecommentarydao.h \
    biblesearchwidget.h \
    bibletreewidget.h \
    biblecommentarywidget.h \
    bibletexttabwidget.h \
    bibletextblockdata.h \
    biblereaderapp.h \
    bibletextcomparebrowser.h \
    biblesearchresultwidget.h \
    biblereaderaboutdlg.h \
    bibledicttabwidget.h \
    biblecommentaryinfo.h \
    biblecommentarytabwidget.h \
    biblereaderprojectdialog.h \
    biblereaderhtmldelegate.h \
    biblereaderconfigdlg.h \
    biblereaderconfiggeneralpage.h \
    biblereaderresourcemanager.h \
    biblereaderdownloader.h \
    biblereaderresourcemanagerdlg.h \
    biblecommentarybrowser.h \
    biblereadertooltip.h \
    biblexrefsdao.h \
    bibleversexref.h \
    biblereaderlayout.h \
    bibledictbrowser.h \
    biblereaderconfigbiblepage.h

OTHER_FILES += \
    convert.py \
    BibleReader.rc \
    br.nsi

TRANSLATIONS += \
    translations/ZH_cn.ts

LIBS += -L$$PWD/../lib -lLogger -lqsimpleupdater

INCLUDEPATH += $$PWD/../cutelogger/include/ \
                $$PWD/../QSimpleUpdater/include/
DEPENDPATH += $$PWD/../lib/

RESOURCES += \
    BibleReader.qrc \
    styles/qdarkstyle/style.qrc \
    styles/white/white.qrc
win32 {
RC_FILE = BibleReader.rc
}

macx {
ICON = $$PWD/assets/images/biblereader.icns
}

DISTFILES += \
    ebook.ddl.txt \
    layout/*.xml \
    assets/others/style.css \
    assets/others/script.js \
    styles/white/white.qss
