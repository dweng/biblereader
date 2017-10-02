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

QT       += core gui sql xml network printsupport webkitwidgets

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
    biblereaderaboutdlg.cpp \
    biblereaderdonationdlg.cpp \
    bibledicttabwidget.cpp \
    biblecommentaryinfo.cpp \
    biblecommentarytabwidget.cpp \
    biblereaderprojectdialog.cpp \
    biblereaderhtmldelegate.cpp \
    biblereaderconfigdlg.cpp \
    biblereaderconfiggeneralpage.cpp \
    biblereaderversion.cpp \
    biblereaderresourcemanager.cpp \
    biblereaderdownloader.cpp \
    biblereaderresourcemanagerdlg.cpp \
    biblecommentarybrowser.cpp \
    biblereadertooltip.cpp \
    biblexrefsdao.cpp \
    bibleversexref.cpp \
    biblereaderlayout.cpp \
    bibledictbrowser.cpp

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
    biblereaderdonationdlg.h \
    bibledicttabwidget.h \
    biblecommentaryinfo.h \
    biblecommentarytabwidget.h \
    biblereaderprojectdialog.h \
    biblereaderhtmldelegate.h \
    biblereaderconfigdlg.h \
    biblereaderconfiggeneralpage.h \
    biblereaderversion.h \
    biblereaderresourcemanager.h \
    biblereaderdownloader.h \
    biblereaderresourcemanagerdlg.h \
    biblecommentarybrowser.h \
    biblereadertooltip.h \
    biblexrefsdao.h \
    bibleversexref.h \
    biblereaderlayout.h \
    bibledictbrowser.h

OTHER_FILES += \
    convert.py \
    BibleReader.rc \
    br.nsi

TRANSLATIONS += \
    translations/ZH_cn.ts

LIBS += -L$$PWD/../lib -lLogger

INCLUDEPATH += $$PWD/../cutelogger/include/
DEPENDPATH += $$PWD/../lib/

RESOURCES += \
    BibleReader.qrc \
    styles/qdarkstyle/style.qrc
win32 {
RC_FILE = BibleReader.rc
}

macx {
ICON = $$PWD/assets/images/biblereader.icns
}

DISTFILES += \
    ebook.ddl.txt \
    translations/ZH_cn.ts \
    layout/*.xml
