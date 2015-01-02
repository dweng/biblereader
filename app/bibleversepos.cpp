#include "bibleversepos.h"

BibleVersePos::BibleVersePos()
{
}

BibleVersePos::BibleVersePos(int b, int c, int v)
{
    bookNumber = b;
    chapterNumber = c;
    verseNumber = v;
}
int BibleVersePos::getBookNumber() const
{
    return bookNumber;
}

void BibleVersePos::setBookNumber(int value)
{
    bookNumber = value;
}
int BibleVersePos::getChapterNumber() const
{
    return chapterNumber;
}

void BibleVersePos::setChapterNumber(int value)
{
    chapterNumber = value;
}
int BibleVersePos::getVerseNumber() const
{
    return verseNumber;
}

void BibleVersePos::setVerseNumber(int value)
{
    verseNumber = value;
}



