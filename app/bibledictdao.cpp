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
#include "bibledictdao.h"
// for logging
#include "Logger.h"

BibleDictDAO::BibleDictDAO(QString &dn, QString &dp)
{
    LOG_INFO() << "Constructor with:" << dn << ", "<<dp;
    dictName = dn;
    dictPath = dp;

    init();
}

BibleDictDAO::~BibleDictDAO()
{
    dictDB.close();
    delete query;
}

QString BibleDictDAO::getExplaination(QString &word)
{
    return dicts.value(word, QObject::tr("There is no explaination for this word"));
}

QMap<QString, QString> BibleDictDAO::getAllWordsAndExplainations()
{
    return dicts;
}

BibleDictInfo BibleDictDAO::getBibleDictInfo()
{
    LOG_INFO() << "get bible dict:" << dictName;
    QString name;
    QString sql = "select * from info";
    query->exec(sql);
    while (query->next()) {
        name = query->value(0).toString();
        if (name == "version") {
            di.setVersion(query->value(1).toInt());
        } else if (name == "shortname") {
            di.setShortname(query->value(1).toString());
            di.setName(query->value(1).toString());
        } else if (name == "fullname") {
            di.setFullname(query->value(1).toString());
        } else if (name == "copyright") {
            di.setCopyright(query->value(1).toString());
        } else if (name == "lang") {
            di.setLang(query->value(1).toString());
        }
    }

    return di;
}

bool BibleDictDAO::init()
{
    LOG_INFO() << "init bible dictionary data: " << dictName << "," << dictPath;
    if (QSqlDatabase::contains(dictName)) {
        dictDB = QSqlDatabase::database(dictName);
    } else {
        dictDB = QSqlDatabase::addDatabase("QSQLITE", dictName);
        dictDB.setDatabaseName(dictPath);
    }

    if (!dictDB.open()) {
        LOG_ERROR() << "Can not open database:" << dictName << "\n"
                    << dictDB.lastError().text();
        return false;
    }

    query = new QSqlQuery(dictDB);

    // init bible data
    initDictData();

    return true;
}

bool BibleDictDAO::initDictData()
{
    LOG_INFO() << "init dictionary data:" << dictName;
    QString sql = "select word, explaination from dict";

    if (dicts.empty()) {
        query->exec(sql);
        while (query->next()) {
            addWord(query->value(0).toString(), query->value(1).toString());
        }
    }

    return true;
}

void BibleDictDAO::addWord(QString word, QString explaination)
{
    dicts.insert(word, explaination);
}
