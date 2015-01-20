#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T11:50:47
#
#-------------------------------------------------
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation: $$[QT_INSTALL_DOCS])
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Plugins: $$[QT_INSTALL_PLUGINS])
message(Data files: $$[QT_INSTALL_DATA])
message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
message(Settings: $$[QT_INSTALL_SETTINGS])
message(Examples: $$[QT_INSTALL_EXAMPLES])
message(Demonstrations: $$[QT_INSTALL_DEMOS])

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BibleReader
TEMPLATE = app

message($$QMAKESPEC)
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
    biblecommentary.cpp \
    biblereaderaboutdlg.cpp \
    biblereaderdonationdlg.cpp

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
    biblecommentary.h \
    biblereaderaboutdlg.h \
    biblereaderdonationdlg.h

OTHER_FILES += \
    bar-descriptor.xml \
    convert.py \
    BibleReader.rc

TRANSLATIONS += \
    translations/ZH_cn.ts

LIBS += -L$$PWD/../lib -lLogger

INCLUDEPATH += $$PWD/../cutelogger/include/
DEPENDPATH += $$PWD/../lib/

RESOURCES += \
    BibleReader.qrc

RC_FILE = BibleReader.rc
