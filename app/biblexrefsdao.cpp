#include "biblexrefsdao.h"
#include "Logger.h"
BibleXRefsDAO::BibleXRefsDAO(QString &xref, QString &dataPath)
{
    query = NULL;
    xrefName = xref;
    xrefPath = dataPath;

    init();
}

BibleXRefsDAO::~BibleXRefsDAO()
{
    xrefDB.close();
    delete query;
}

QList<BibleVerseXref> BibleXRefsDAO::getXrefsByChapter(int book, int chapter)
{
    QString sql = "select * from xrefs_bcv where fbi=%1 and fci=%2";
    sql = sql.arg(QString::number(book), QString::number(chapter));

    query->exec(sql);
    QList<BibleVerseXref> xrefsResult;
    xrefsResult.clear();
    while (query->next()) {
        BibleVerseXref xref;
        int fvi = query->value("fvi").toInt();
        int fbi = query->value("fbi").toInt();
        int fci = query->value("fci").toInt();
        int fspan = query->value("fspan").toInt();

        int tbi = query->value("tbi").toInt();
        int tci = query->value("tci").toInt();
        int tvi = query->value("tvi").toInt();
        int tspan = query->value("tspan").toInt();

        xref.setFbi(fbi);
        xref.setFci(fci);
        xref.setFvi(fvi);
        xref.setFspan(fspan);
        xref.setTbi(tbi);
        xref.setTci(tci);
        xref.setTvi(tvi);
        xref.setTspan(tspan);
        xref.setTbname(getBookNamebyId(tbi));

        xrefsResult.push_back(xref);
    }

    return xrefsResult;
}

QList<BibleVerseXref> BibleXRefsDAO::getXrefsByVerse(int book, int chapter, int verse)
{
    QString sql = "select * from xrefs_bcv where fbi=%1 and fci=%2 and fvi=%3";
    sql = sql.arg(QString::number(book), QString::number(chapter), QString::number(verse));

    query->exec(sql);
    QList<BibleVerseXref> xrefsResult;
    xrefsResult.clear();
    while (query->next()) {
        BibleVerseXref xref;
        int fvi = query->value("fvi").toInt();
        int fbi = query->value("fbi").toInt();
        int fci = query->value("fci").toInt();
        int fspan = query->value("fspan").toInt();

        int tbi = query->value("tbi").toInt();
        int tci = query->value("tci").toInt();
        int tvi = query->value("tvi").toInt();
        int tspan = query->value("tspan").toInt();

        xref.setFbi(fbi);
        xref.setFci(fci);
        xref.setFvi(fvi);
        xref.setFspan(fspan);
        xref.setTbi(tbi);
        xref.setTci(tci);
        xref.setTvi(tvi);
        xref.setTspan(tspan);
        xref.setTbname(getBookNamebyId(tbi));

        xrefsResult.push_back(xref);
    }

    return xrefsResult;
}

bool BibleXRefsDAO::init()
{
    LOG_INFO() << "init xrefs data: " << xrefName << "," << xrefPath;
    if (QSqlDatabase::contains(xrefName)) {
        xrefDB = QSqlDatabase::database(xrefName);
    } else {
        xrefDB = QSqlDatabase::addDatabase("QSQLITE", xrefName);
    }

    xrefDB.setDatabaseName(xrefPath);

    if (!xrefDB.open()) {
        LOG_ERROR() << "Can not open database:" << xrefName << "\n"
                    << xrefDB.lastError().text();
        return false;
    }

    query = new QSqlQuery(xrefDB);
    return true;
}

QString BibleXRefsDAO::getBookNamebyId(int bid)
{
    QStringList bookNames;
    QFile bnfile(QString(":/data/assets/data/booknames.txt"));
    bnfile.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!bnfile.atEnd()) {
        bookNames.push_back(QString(bnfile.readLine()));
    }
    for (int i = 0; i < bookNames.size(); i++) {
        if (bid == i+1) {
            return bookNames[i];
        }
    }
}

