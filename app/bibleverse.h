#ifndef BIBLEVERSE_H
#define BIBLEVERSE_H

#include <QString>
#include <QStringList>
#include <QList>
#include "bibleversexref.h"
class BibleVerse
{
public:
    BibleVerse();
    ~BibleVerse();

    int getVerse() const;
    void setVerse(int value);

    QString getVerseText() const;
    void setVerseText(const QString &value);

    QString getBibleVersion() const;
    void setBibleVersion(const QString &value);

    int getChapter() const;
    void setChapter(int value);

    int getBookNumber() const;
    void setBookNumber(int value);

    QString text();

    QString getBookName() const;
    void setBookName(const QString &value);

    QString getVerseHeader();
    void addXref(BibleVerseXref xref);

    QList<BibleVerseXref> getXrefs() const;

private:
    int bookNumber;
    QString bookName;
    int chapter;
    int verse;

    QString verseText;
    QString bibleVersion;

    // xrefs
    QList<BibleVerseXref> xrefs;
};

#endif // BIBLEVERSE_H
