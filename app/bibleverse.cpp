#include "bibleverse.h"
#include "biblechapter.h"

BibleVerse::BibleVerse()
{
}

BibleVerse::~BibleVerse()
{
}

int BibleVerse::getVerse() const
{
    return verse;
}

void BibleVerse::setVerse(int value)
{
    verse = value;
}
QString BibleVerse::getVerseText() const
{
    return verseText;
}

void BibleVerse::setVerseText(const QString &value)
{
    verseText = value;
}
QString BibleVerse::getBibleVersion() const
{
    return bibleVersion;
}

void BibleVerse::setBibleVersion(const QString &value)
{
    bibleVersion = value;
}
int BibleVerse::getChapter() const
{
    return chapter;
}

void BibleVerse::setChapter(int value)
{
    chapter = value;
}
int BibleVerse::getBookNumber() const
{
    return bookNumber;
}

void BibleVerse::setBookNumber(int value)
{
    bookNumber = value;
}

QString BibleVerse::text()
{
    QString t = "";
    return t.sprintf("[%s %i:%i] %s",
                     bookName.toStdString().c_str(),
                     chapter,
                     verse,
                     verseText.toStdString().c_str());
}
QString BibleVerse::getBookName() const
{
    return bookName;
}

void BibleVerse::setBookName(const QString &value)
{
    bookName = value;
}









