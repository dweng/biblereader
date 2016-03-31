#-------------------------------------------------
#
# Project created by QtCreator 2014-04-15T11:50:47
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BibleReader
TEMPLATE = subdirs
SUBDIRS = app cutelogger \
    updater MRichTextEditor

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6

debug {
    DESTDIR = $$PWD/bin/debug
}

release {
    DESTDIR = $$PWD/bin/release
}

app.depends = cutelogger MRichTextEditor
