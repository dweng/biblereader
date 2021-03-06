#include "bibleverse.h"
#include "biblechapter.h"

BibleVerse::BibleVerse()
{
    xrefs.clear();
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

QString BibleVerse::header()
{
    QString t = "";
    return t.sprintf("[%s %i:%i]",
                     bookName.toStdString().c_str(),
                     chapter,
                     verse);
}

QString BibleVerse::textLongHeader()
{
    QString t = "";
    return t.sprintf("[%s %i:%i] %s",
                     bookNameLong.toStdString().c_str(),
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

QString BibleVerse::getVerseHeader()
{
    return QString("%1 %2:%3").arg(bookName, QString::number(chapter), QString::number(verse));
}

void BibleVerse::addXref(BibleVerseXref xref)
{
    xrefs.push_back(xref);
}

QList<BibleVerseXref> BibleVerse::getXrefs() const
{
    return xrefs;
}

QString BibleVerse::getBookNameLong() const
{
    return bookNameLong;
}

void BibleVerse::setBookNameLong(const QString &value)
{
    bookNameLong = value;
}









