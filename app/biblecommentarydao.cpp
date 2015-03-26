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
#include <QtXml>
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
    if (bci.getShortname() == "") {
        QString infoFile = bcPath + QDir::separator()+"info.xml";
        QDomDocument doc;
        QFile file(infoFile);
        if (!file.open(QIODevice::ReadOnly))
            return bci;
        if (!doc.setContent(&file)) {
            file.close();
            return bci;
        }
        file.close();

        // print out the element names of all elements that are direct children
        // of the outermost element.
        QDomElement docElem = doc.documentElement();

        QDomNode n = docElem.firstChild();
        while(!n.isNull()) {
            QDomElement e = n.toElement(); // try to convert the node to an element.
            if(!e.isNull()) {
                LOG_INFO() << qPrintable(e.tagName()) << qPrintable(e.text()) <<endl; // the node really is an element.
                if (e.tagName() == "name") {
                    bci.setName(e.text());
                } else if (e.tagName() == "fullname") {
                    bci.setFullname(e.text());
                } else if (e.tagName() == "shortname") {
                    bci.setShortname(e.text());
                } else if (e.tagName() == "version") {
                    bci.setVersion(e.text().toInt());
                } else if (e.tagName() == "copyright") {
                    bci.setCopyright(e.text());
                } else if (e.tagName() == "creator") {

                } else if (e.tagName() == "lang") {
                    bci.setLang(e.text());
                } else if (e.tagName() == "description") {
                    bci.setDescription(e.text());
                }
            }
            n = n.nextSibling();
        }
    }

    return bci;
}

QString BibleCommentaryDAO::getChapterCmt(int book, int chapter)
{
    QString bookNumber = QString("%1").arg(book, 2, 10, QChar('0'));
    int len = (book == 19 ? 3 : 2);
    QString chapterNumber = QString("%1").arg(chapter, len, 10, QChar('0'));
    QString cmtfile = bcPath + QDir::separator() +
            bookNumber + "_" + chapterNumber + ".htm";
    LOG_INFO() << "loading cmt: " << cmtfile;
    QFile f(cmtfile);

    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
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
