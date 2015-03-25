/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 */
#include <QDir>
#include <QSettings>
#include <QFile>
#include <QObject>
#include "biblecommentarydao.h"
#include "Logger.h"

BibleCommentaryDAO::BibleCommentaryDAO(QString &bc, QString &bcp)
{
    LOG_INFO() << "Constructor Bible Commentary with " << bc << "," << bcp;
    bcName = bc;
    bcPath = bcp;

    init();
}

BibleCommentaryDAO::~BibleCommentaryDAO()
{
}

BibleCommentaryInfo BibleCommentaryDAO::getCmtInfo()
{
    QString infoFile = bcPath + QDir::separator()+"info.ini";
    QSettings info(infoFile, QSettings::IniFormat);

    BibleCommentaryInfo bcinfo;

    bcinfo.setFullname(info.value("name").toString());
    bcinfo.setName(info.value("name").toString());
    bcinfo.setCopyright(info.value("copyright").toString());
    bcinfo.setDescription(info.value("description").toString());
    bcinfo.setLang(info.value("lang").toString());
    bcinfo.setShortname(info.value("shortname").toString());
    bcinfo.setVersion(info.value("version").toInt());

    return bcinfo;
}

QString BibleCommentaryDAO::getChapterCmt(int book, int chapter)
{
    QString bookNumber = QString("%1").arg(book, 2, 10, QChar('0'));
    QString chapterNumber = QString("%1").arg(chapter, 2, 10, QChar('0'));
    QString cmtfile = bcPath + QDir::separator() +
            bookNumber + QDir::separator() + chapterNumber + ".txt";
    QFile f(cmtfile);

    if (f.open(QIODevice::ReadOnly)) {
        return f.readAll();
    } else {
        return QObject::tr("There is no commentary file for this chapter");
    }
}

bool BibleCommentaryDAO::init()
{
    LOG_INFO() << "init bible commentary data: " << bcName << "," << bcPath;

    return true;
}
