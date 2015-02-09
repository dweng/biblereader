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
#include <QSqlError>
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
    bcDB.close();
    if (query) {
        delete query;
    }
}

bool BibleCommentaryDAO::init()
{
    LOG_INFO() << "init bible commentary data: " << bcName << "," << bcPath;
    if (QSqlDatabase::contains(bcName)) {
        bcDB = QSqlDatabase::database(bcName);
    } else {
        bcDB = QSqlDatabase::addDatabase("QSQLITE", bcName);
        bcDB.setDatabaseName(bcPath);
    }

    if (!bcDB.open()) {
        LOG_ERROR() << "Can not open database:" << bcName << "\n"
                    << bcDB.lastError().text();
        return false;
    }

    query = new QSqlQuery(bcDB);

    return true;
}
