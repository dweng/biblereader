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
#include "bibletextdao.h"
#include <QSqlError>

// for logging
#include "Logger.h"
#include "ConsoleAppender.h"

/**
 * @brief constructor
 * Assign values of class members
 */
BibleTextDAO::BibleTextDAO(QString &bv, QString &path)
{
    bibleVersion = bv;
    biblePath = path;
    allBooks.clear();
    allBookIDs.clear();
    init();
}

BibleTextDAO::~BibleTextDAO()
{
    bibleDB.close();
    delete query;
}

BibleVerse BibleTextDAO::getOneVerse(int book, int chapter, int verse)
{
    QString sql = "";
    sql.sprintf("select * from verses where book_number=%i and chapter=%i and verse=%i",
                book, chapter, verse);

    query->exec(sql);
    BibleVerse bv;

    int bookId;
    while (query->next()) {
        bookId = query->value(0).toInt();
        bv.setBookNumber(bookId);
        bv.setBookName(getBookByID(bookId).getShortName());
        bv.setChapter(query->value(1).toInt());
        bv.setVerse(query->value(2).toInt());
        bv.setVerseText(query->value(3).toString());
    }

    return bv;
}

/**
 * @brief BibleTextDAO::getSomeVerses
 * @return
 */
QList<BibleVerse> BibleTextDAO::getVerses(BibleVersePos start, BibleVersePos end)
{
    QString sql = "select * from verses where book_number >= " +
            QString::number(start.getBookNumber()) + " and book_number <= " +
            QString::number(end.getBookNumber()) + " and chapter >= " +
            QString::number(start.getChapterNumber()) + " and chapter <= " +
            QString::number(end.getChapterNumber()) + " and verse >= " +
            QString::number(start.getVerseNumber()) + " and verse <= " +
            QString::number(end.getVerseNumber());

    query->exec(sql);
    QList<BibleVerse> verses;
    BibleVerse bv;
    int bookId;
    while(query->next()) {
        bookId = query->value(0).toInt();
        bv.setBookNumber(bookId);
        bv.setBookName(getBookByID(bookId).getShortName());
        bv.setChapter(query->value(1).toInt());
        bv.setVerse(query->value(2).toInt());
        bv.setVerseText(query->value(3).toString());

        verses.push_back(bv);
    }

    return verses;
}

BibleChapter BibleTextDAO::getChapter(int bookNumber, int chapter)
{
    QString sql = "select * from verses where book_number=" +
            QString::number(bookNumber) + " and chapter=" + QString::number(chapter);

    query->exec(sql);
    BibleChapter bc;
    QList<BibleVerse> bvs;
    int versesCount = 0;
    while (query->next()) {
        BibleVerse bv_temp;
        QString verseText = query->value(3).toString();//text
        int verse = query->value(2).toInt();

        bv_temp.setVerseText(verseText);
        bv_temp.setVerse(verse);
        bv_temp.setChapter(chapter);

        bvs.push_back(bv_temp);

        versesCount++;
    }

    bc.setVersesList(bvs);
    bc.setChapter(chapter);
    bc.setVersesCount(versesCount);
    bc.setBook(getBookByID(bookNumber));

    return bc;
}

QList<BibleVerse> BibleTextDAO::search(QString q,
                                       SearchScope scope,
                                       QList<int> customBooks)
{
    QString sql = "select * from verses where text like '\%"+q+"\%' ";

    // scope to QList<int> books
    switch (scope) {
    case All:
    case Pentateuch:
    case History:
    case Wisdom:
    case Prophets:
    case Gospels:
    case Acts:
    case PaulEpistles:
    case GeneralEpistles:
    case Revelation:
        sql += convertBookIDsToSql(scope);
        break;
    case UserDefined:
        sql += convertBookIDsToSql(scope, customBooks);
        break;
    default:
        sql += convertBookIDsToSql(All);
        break;
    }

    query->exec(sql);
    QList<BibleVerse> result;
    while (query->next()) {
        BibleVerse bv;
        bv.setBookNumber(query->value(0).toInt());
        bv.setChapter(query->value(1).toInt());
        bv.setVerse(query->value(2).toInt());
        bv.setVerseText(query->value(3).toString());
        bv.setBookName(getBookNameByID(query->value(0).toInt()));
        bv.setBibleVersion(this->bibleVersion);

        result.push_back(bv);
    }

    return result;
}

bool BibleTextDAO::init()
{
    LOG_INFO() << "init bible data: " << bibleVersion << "," << biblePath;
    if (QSqlDatabase::contains(bibleVersion)) {
        bibleDB = QSqlDatabase::database(bibleVersion);
    } else {
        bibleDB = QSqlDatabase::addDatabase("QSQLITE", bibleVersion);
    }

    bibleDB.setDatabaseName(biblePath);

    if (!bibleDB.open()) {
        LOG_ERROR() << "Can not open database:" << bibleVersion << "\n"
                    << bibleDB.lastError().text();
        return false;
    }

    query = new QSqlQuery(bibleDB);

    // init bible data
    initBibleData();

    return true;
}

BibleInfo BibleTextDAO::getBibleInfo()
{
    if (bi.getVersion() == "") {
        QString sql = "select * from info";
        query->exec(sql);

        while(query->next()) {
            QString name = query->value(0).toString();
            QVariant value = query->value(1);
            if (name.compare("description") == 0) {
                bi.setDescription(value.toString());
            } else if (name.compare("chapter_string") == 0) {
                bi.setChapterString(value.toString());
            } else if (name.compare("language") == 0) {
                bi.setLanguage(value.toString());
            } else if (name.compare("russian_numbering") == 0) {
                // do nothing
            } else if (name.compare("strong_numbers") == 0) {
                bi.setHasSN(value.toBool());
            } else if (name.compare("right_to_left") == 0) {
                bi.setRtl(value.toBool());
            } else if (name.compare("fullname") == 0) {
                bi.setFullname(value.toString());
            }
        }
        bi.setVersion(bibleVersion);
        bi.setDataPath(biblePath);
    }

    return bi;
}

QList<BibleBook> BibleTextDAO::getAllBooks()
{
    if (allBooks.empty() && allBookIDs.empty()) {
        QString sql = "";
        sql.sprintf("select book_number, short_name, long_name, chapter_count from books");

        query->exec(sql);
        while (query->next()) {
            BibleBook bb;
            bb.setBookNumber(query->value(0).toInt());
            bb.setShortName(query->value(1).toString());
            bb.setLongName(query->value(2).toString());
            bb.setChaptersCount(query->value(3).toInt());
            allBookIDs.push_back(query->value(0).toInt());
            allBooks.push_back(bb);
        }

    }

    return allBooks;
}

BibleBook BibleTextDAO::getBookByID(int b)
{
    if (allBooks.empty() && allBookIDs.empty()) {
        getAllBooks();
    }

    return allBooks.value(allBookIDs.indexOf(b));
}

QList<int> BibleTextDAO::getChaptersCountOfOneBook(int bookNumber)
{
    QString sql = "";
    sql.sprintf("select count(*) from verses where book_number=%i group by chapter", bookNumber);

    query->exec(sql);
    QList<int> chapters;
    while (query->next()) {
        chapters.push_back(query->value(0).toInt());
    }

    return chapters;
}

int BibleTextDAO::getBookIDByName(QString book)
{
    QString sql = "";
    sql.sprintf("select book_number from books where long_name=%s", book.toStdString().c_str());

    query->exec(sql);
    while (query->next()) {
        return query->value(0).toInt();
    }
    return 0;
}

void BibleTextDAO::initBibleData()
{
    getAllBooks();
}

QString BibleTextDAO::convertBookIDsToSql(SearchScope scope,
                                          QList<int> customBooks)
{
    QString t = "and book_number in (%1)";
    QString ids = "";
    switch (scope) {
    case All:
        for (int i = 0; i < allBookIDs.count(); i++) {
            if (i == allBookIDs.count() - 1) {
                ids += QString("%1").arg(allBookIDs[i]);
            } else {
                ids += QString("%1,").arg(allBookIDs[i]);
            }
        }
        break;
    case Pentateuch:
        ids += "1, 2, 3, 4, 5";
        break;
    case History:
        ids += "6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17";
        break;
    case Wisdom:
        ids += "18, 19, 20, 21, 22";
        break;
    case Prophets:
        ids += "23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39";
        break;
    case Gospels:
        ids += "40, 41, 42, 43";
        break;
    case Acts:
        ids += "44";
        break;
    case PaulEpistles:
        ids += "45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58";
        break;
    case GeneralEpistles:
        ids += "59, 60, 61, 62, 63, 64, 65";
        break;
    case Revelation:
        ids += "66";
        break;
    case UserDefined:
        for (int i = 0; i < customBooks.count(); i++) {
            if (i == customBooks.count() -1)
                ids += QString("%1").arg(customBooks[i]);
            else
                ids += QString("%1, ").arg(customBooks[i]);
        }
    }

    return t.arg(ids);
}

QString BibleTextDAO::getBookNameByID(int n)
{
    for (int i = 0; i < allBooks.count(); i++) {
        BibleBook b = allBooks[i];
        if (b.getBookNumber() == n) {
            return b.getShortName();
        }
    }

    // no book number match "n"
    return "None";
}

/*
QList<QVariant> BibleTextDTO::query(QString sql)
{
    QList<QVariant> res;
    query.exec(sql);
    while (query.next()) {
        res.push_back(query.);
    }

    return res;
}
*/
