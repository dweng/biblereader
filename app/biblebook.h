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

#ifndef BIBLEBOOK_H
#define BIBLEBOOK_H

#include <QString>

/**
 * This class represents a book of bible.
 * example:
 * Gen
 * Exd
 * Lvi
 * etc.,
 */
class BibleBook
{
public:
    // default constructor
    BibleBook();

    int getBookNumber() const;
    void setBookNumber(int value);

    int getChaptersCount() const;
    void setChaptersCount(int value);

    int getVersesCount() const;
    void setVersesCount(int value);

    QString getBibleVersion() const;
    void setBibleVersion(const QString &value);

    QString getLongName() const;
    void setLongName(const QString &value);

    QString getShortName() const;
    void setShortName(const QString &value);

private:
    /**
     * full name of book
     * example: Mark
     */
    QString longName;
    /**
     * short name of book
     * example: gen
     */
    QString shortName;
    /**
     * book number in database
     * example: 10
     */
    int bookNumber;
    /**
     * how many chapters in this book
     * example: gen has 50 chapters
     */
    int chaptersCount;
    /**
     * how many verses in this book
     * example: gen 1 has 31 verses
     */
    int versesCount;
    /**
     * which bible
     * example: CUVS (chinese union version simplfied)
     */
    QString bibleVersion;
};

#endif // BIBLEBOOK_H
