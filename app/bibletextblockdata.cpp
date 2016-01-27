/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is a simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese.
 *
 * You can use it totally freely, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 */
#include "bibletextblockdata.h"

BibleTextBlockData::BibleTextBlockData(QString &bv, int b, int c, int v)
{
    bibleVersion = bv;
    book = b;
    chapter = c;
    verse = v;
}

BibleTextBlockData::~BibleTextBlockData()
{
    // do nothing now
}
int BibleTextBlockData::getBook() const
{
    return book;
}

void BibleTextBlockData::setBook(int value)
{
    book = value;
}
int BibleTextBlockData::getChapter() const
{
    return chapter;
}

void BibleTextBlockData::setChapter(int value)
{
    chapter = value;
}
int BibleTextBlockData::getVerse() const
{
    return verse;
}

void BibleTextBlockData::setVerse(int value)
{
    verse = value;
}
QString BibleTextBlockData::getBibleVersion() const
{
    return bibleVersion;
}

void BibleTextBlockData::setBibleVersion(const QString &value)
{
    bibleVersion = value;
}




