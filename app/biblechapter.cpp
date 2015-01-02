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
#include "biblechapter.h"

BibleChapter::BibleChapter()
{
}

int BibleChapter::getChapter() const
{
    return chapter;
}

void BibleChapter::setChapter(int value)
{
    chapter = value;
}
QList<BibleVerse> BibleChapter::getVersesList() const
{
    return versesList;
}

void BibleChapter::setVersesList(const QList<BibleVerse> &value)
{
    versesList = value;
    setVersesCount(value.count());
}
QString BibleChapter::getBibleVersion() const
{
    return bibleVersion;
}

void BibleChapter::setBibleVersion(const QString &value)
{
    bibleVersion = value;
}
int BibleChapter::getVersesCount() const
{
    return versesCount;
}

void BibleChapter::setVersesCount(int value)
{
    versesCount = value;
}

QString BibleChapter::text()
{
    QString t = QString("%1 %2\n").arg(
                book.getLongName(), QString::number(chapter)
                );
    QString temp;

    if (versesList.count() != 0) {
        for (int i = 0; i < versesList.size(); i++) {
            temp = "";
            temp.sprintf("%i %s\n",
                      versesList[i].getVerse(),
                      versesList[i].getVerseText().toStdString().c_str());

            t += temp;
        }
    } else {
        t += QObject::tr("This version doesn't contain this verse.");
    }

    return t;
}
BibleBook BibleChapter::getBook() const
{
    return book;
}

void BibleChapter::setBook(const BibleBook &value)
{
    book = value;
}







