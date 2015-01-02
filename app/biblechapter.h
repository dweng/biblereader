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
#ifndef BIBLECHAPTER_H
#define BIBLECHAPTER_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "bibleverse.h"
#include "biblebook.h"

/**
 * @brief The BibleChapter class
 * This class represents a chapter of a bible book.
 */
class BibleChapter
{
public:
    BibleChapter();

    int getChapter() const;
    void setChapter(int value);

    QList<BibleVerse> getVersesList() const;
    void setVersesList(const QList<BibleVerse> &value);

    QString getBibleVersion() const;
    void setBibleVersion(const QString &value);

    int getVersesCount() const;
    void setVersesCount(int value);

    QString text();

    BibleBook getBook() const;
    void setBook(const BibleBook &value);

private:
    /**
     * @brief bible book the chapter in
     */
    BibleBook book;
    /**
     * @brief the chapter number
     */
    int chapter;

    /**
     * @brief all verses of this chapter
     */
    QList<BibleVerse> versesList;

    /**
     * @brief bible version of this chapter
     */
    QString bibleVersion;
    /**
     * @brief verses count of this chapter
     */
    int versesCount;
};

#endif // BIBLECHAPTER_H
