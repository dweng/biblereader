#include "biblereaderconfigurator.h"
#include <QFile>
#include <QDir>
#include <QApplication>
#include <QFontDatabase>
BibleReaderConfigurator::BibleReaderConfigurator(QObject *parent) :
    QObject(parent)
{
    settings = NULL;
    QDir dir(QApplication::applicationDirPath());
#ifdef Q_OS_OSX || Q_OS_MACX || Q_OS_MAC || Q_OS_MAC64
    dir.cdUp();
    dir.cd("Resources");
#endif
    settingsFile = dir.absolutePath() + "/biblereader.ini";
    if (!QFile(settingsFile).exists()) {
        // create default settings
        settings = new QSettings(settingsFile, QSettings::IniFormat);
        settings->setValue("/base/biblePathBase", dir.absolutePath() + "/bibles/");
        settings->setValue("/base/dictPathBase", dir.absolutePath() + "/dicts/");
        settings->setValue("/base/bcPathBase", dir.absolutePath() + "/commentarys/");
        settings->setValue("/base/defaultBibleVersion", "LZZ");
        settings->setValue("/base/defaultDict", "SNCHS");
        settings->setValue("/base/defaultCommentary", "DDESJZS");
        settings->setValue("/base/lastBook", "1");
        settings->setValue("/base/lastChapter", "1");
        settings->setValue("/base/lastVerse", "1");

        // bible text browser
        settings->setValue("/text/selectedVerseBG", "#7dde96");
        settings->setValue("/text/projectDlgBG", "#000000");
        settings->setValue("/text/projectDlgFG", "#ffffff");

        settings->setValue("/text/bibleTextFontFamily",
                           QFontDatabase::systemFont(QFontDatabase::GeneralFont).family());
        settings->setValue("/text/bibleTextFontSize", 14.0);

        // app
        settings->setValue("/app/autoupdate", true);
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

    // bible text
    selectedVerseBG = QColor(settings->value("/text/selectedVerseBG").toString());
    projectDlgBG = QColor(settings->value("/text/projectDlgBG").toString());
    projectDlgFG = QColor(settings->value("/text/projectDlgFG").toString());
    bibleTextFontFamily = settings->value("/text/bibleTextFontFamily").toString();
    bibleTextFontSize = settings->value("/text/bibleTextFontSize").toDouble();
    isAutoUpdate = settings->value("/app/autoupdate").toBool();
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

        // bible text settings
        settings->setValue("/text/selectedVerseBG", selectedVerseBG.name());
        settings->setValue("/text/projectDlgBG", projectDlgBG.name());
        settings->setValue("/text/projectDlgFG", projectDlgFG.name());
        settings->setValue("/text/bibleTextFontFamily", bibleTextFontFamily);
        settings->setValue("/text/bibleTextFontSize", bibleTextFontSize);
        // app setting
        settings->setValue("/app/autoupdate", isAutoUpdate);

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

void BibleReaderConfigurator::setDefaultCommentary(const QString &value)
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

QColor BibleReaderConfigurator::getSelectedVerseBG() const
{
    return selectedVerseBG;
}

void BibleReaderConfigurator::setSelectedVerseBG(const QColor &value)
{
    if (selectedVerseBG != value) {
        selectedVerseBG = value;
        emit selectedVerseBGChanged(value);
    }
}

QColor BibleReaderConfigurator::getProjectDlgBG() const
{
    return projectDlgBG;
}

void BibleReaderConfigurator::setProjectDlgBG(const QColor &value)
{
    if (projectDlgBG != value) {
        projectDlgBG = value;
        emit projectDlgBGChanged(value);
    }

}

QColor BibleReaderConfigurator::getProjectDlgFG() const
{
    return projectDlgFG;
}

void BibleReaderConfigurator::setProjectDlgFG(const QColor &value)
{
    if (projectDlgFG != value) {
        projectDlgFG = value;
        emit projectDlgFGChanged(value);
    }
}

bool BibleReaderConfigurator::getIsAutoUpdate() const
{
    return isAutoUpdate;
}

void BibleReaderConfigurator::setIsAutoUpdate(bool value)
{
    isAutoUpdate = value;
}

QString BibleReaderConfigurator::getBibleTextFontFamily() const
{
    return bibleTextFontFamily;
}

void BibleReaderConfigurator::setBibleTextFontFamily(const QString &value)
{
    if (bibleTextFontFamily != value) {
        bibleTextFontFamily = value;
        emit bibleTextFontFamilyChanged(value);
    }

}

double BibleReaderConfigurator::getBibleTextFontSize() const
{
    return bibleTextFontSize;
}

void BibleReaderConfigurator::setBibleTextFontSize(double value)
{
    if (bibleTextFontSize != value) {
        bibleTextFontSize = value;
        emit bibleTextFontSizeChanged(value);
    }

}








