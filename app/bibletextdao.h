/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese Simplified.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 *
 */
#ifndef BIBLETEXTDTO_H
#define BIBLETEXTDTO_H

#include "biblebook.h"
#include "bibleinfo.h"
#include "biblechapter.h"
#include "bibleverse.h"
#include "bibleversepos.h"
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QList>
#include <QtSql/QSqlDatabase>
#include <QMap>

/**
 * @brief The BibleTextDAO class
 *
 * This database access object class is used to communicate with
 * SQLite's database of bible text.
 */
class BibleTextDAO
{
public:
    // search scope
    enum SearchScope {
        All = 1,
        Pentateuch,
        History,
        Wisdom,
        Prophets,
        Gospels,
        Acts,
        PaulEpistles,
        GeneralEpistles,
        Revelation,
        UserDefined
    };

public:
    BibleTextDAO(QString &bv, QString &dataPath);
    ~BibleTextDAO();

public:
    BibleVerse getOneVerse(int book, int chapter, int verse);
    QList<BibleVerse> getVerses(BibleVersePos start, BibleVersePos end);
    BibleChapter getChapter(int bookNumber, int chapter);

    QList<BibleVerse> search(QString q,
                       SearchScope scope = All,
                       QList<int> customBooks = QList<int>());
    bool init();
    bool deinit();

    BibleInfo getBibleInfo();
    QList<BibleBook> getAllBooks();
    BibleBook getBookByID(int b);
    QList<int> getChaptersCountOfOneBook(int bookNumber);
    int getBookIDByName(QString book);
private:
    //QList<QVariant> query(QString sql);
    QSqlDatabase bibleDB;
    QString bibleVersion;
    QString biblePath;
    QSqlQuery *query;

    BibleInfo bi;

    // all books' id of Holy Bible caches
    QList<int> allBookIDs;
    // all books of Holy Bible caches
    QList<BibleBook> allBooks;

private:
    void initBibleData();
    QString convertBookIDsToSql(SearchScope scope,
                                QList<int> customBooks = QList<int>());
    QString getBookNameByID(int n);
};

#endif // BIBLETEXTDTO_H
