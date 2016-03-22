#ifndef BIBLEXREFSDAO_H
#define BIBLEXREFSDAO_H

#include "bibleversexref.h"
#include <QtSql>
#include <QList>
#include <QMap>
class BibleXRefsDAO
{
public:
    BibleXRefsDAO(QString &xref, QString &dataPath);
    ~BibleXRefsDAO();

    QList<BibleVerseXref> getXrefsByChapter(int book, int chapter);
    QList<BibleVerseXref> getXrefsByVerse(int book, int chapter, int verse);

private:
    QString xrefName;
    QString xrefPath;
    QSqlDatabase xrefDB;
    QSqlQuery *query;

private:
    bool init();
    QString getBookNamebyId(int bid);
};


#endif // BIBLEXREFSDAO_H
