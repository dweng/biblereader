#include "biblereaderconfigurator.h"
#include <QFile>
#include <QApplication>

BibleReaderConfigurator::BibleReaderConfigurator(QObject *parent) :
    QObject(parent)
{
    settings = NULL;
    settingsFile = QApplication::applicationDirPath() + "/biblereader.ini";
    if (!QFile(settingsFile).exists()) {
        // create default settings
        settings = new QSettings(settingsFile, QSettings::IniFormat);
        settings->setValue("/base/biblePathBase", QApplication::applicationDirPath() + "/bibles/");
        settings->setValue("/base/dictPathBase", QApplication::applicationDirPath() + "/dicts/");
        settings->setValue("/base/bcPathBase", QApplication::applicationDirPath() + "/commentarys/");
        settings->setValue("/base/defaultBibleVersion", "LZZ");
        settings->setValue("/base/defaultDict", "SNCHS");
        settings->setValue("/base/defaultCommentary", "DDE");
        settings->setValue("/base/lastBook", "1");
        settings->setValue("/base/lastChapter", "1");
        settings->setValue("/base/lastVerse", "1");
    }
    if (!settings)
        settings = new QSettings(settingsFile, QSettings::IniFormat);

    biblePathBase = settings->value("/base/biblePathBase").toString();
    dictPathBase = settings->value("/base/dictPathBase").toString();
    bcPathBase = settings->value("/base/bcPathBase").toString();
    defaultBibleVersion = settings->value("/base/defaultBibleVersion").toString();
    defaultDict = settings->value("/base/defaultDict").toString();
    defaultCommentary = settings->value("/base/defaultCommentary").toString();
    lastBook = settings->value("/base/lastBook").toInt();
    lastChapter = settings->value("/base/lastChapter").toInt();
    lastVerse = settings->value("/base/lastVerse").toInt();
    if (lastBook == 0 || lastChapter == 0 || lastVerse == 0) {
        lastBook = 1; // gen
        lastChapter = 1;
        lastVerse = 1;
    }
}

BibleReaderConfigurator::~BibleReaderConfigurator()
{
    if (settings) {
        settings->setValue("/base/biblePathBase", biblePathBase);
        settings->setValue("/base/dictPathBase", dictPathBase);
        settings->setValue("/base/bcPathBase", bcPathBase);
        settings->setValue("/base/defaultBibleVersion", defaultBibleVersion);
        settings->setValue("/base/defaultDict", defaultDict);
        settings->setValue("/base/defaultCommentary", defaultCommentary);
        settings->setValue("/base/lastBook", QString::number(lastBook));
        settings->setValue("/base/lastChapter", QString::number(lastChapter));
        settings->setValue("/base/lastVerse", QString::number(lastVerse));
        delete settings;
    }
}
QString BibleReaderConfigurator::getBiblePathBase() const
{
    return biblePathBase;
}

void BibleReaderConfigurator::setBiblePathBase(const QString &value)
{
    biblePathBase = value;
}
QString BibleReaderConfigurator::getDefaultBibleVersion() const
{
    return defaultBibleVersion;
}

void BibleReaderConfigurator::setDefaultBibleVersion(const QString &value)
{
    defaultBibleVersion = value;
}
int BibleReaderConfigurator::getLastBook() const
{
    return lastBook;
}

void BibleReaderConfigurator::setLastBook(int value)
{
    lastBook = value;
}
int BibleReaderConfigurator::getLastChapter() const
{
    return lastChapter;
}

void BibleReaderConfigurator::setLastChapter(int value)
{
    lastChapter = value;
}
int BibleReaderConfigurator::getLastVerse() const
{
    return lastVerse;
}

void BibleReaderConfigurator::setLastVerse(int value)
{
    lastVerse = value;
}

QString BibleReaderConfigurator::getDictPathBase() const
{
    return dictPathBase;
}

void BibleReaderConfigurator::setDictPathBase(QString &value)
{
    dictPathBase = value;
}

QString BibleReaderConfigurator::getDefaultDict() const
{
    return defaultDict;
}

void BibleReaderConfigurator::setDefaultDict(QString &value)
{
    defaultDict = value;
}

QString BibleReaderConfigurator::getDefaultCommentary() const
{
    return defaultCommentary;
}

void BibleReaderConfigurator::setDefaultCommentary(QString &value)
{
    defaultCommentary = value;
}
QString BibleReaderConfigurator::getBcPathBase() const
{
    return bcPathBase;
}

void BibleReaderConfigurator::setBcPathBase(const QString &value)
{
    bcPathBase = value;
}






