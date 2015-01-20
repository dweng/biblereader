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

    /**
     * @brief getter of book number
     * @return int book number
     */
    int getBookNumber() const;

    /**
     * @brief setter of book number
     * @param value
     */
    void setBookNumber(int value);

    /**
     * @brief getter of chapters count
     * @return int
     */
    int getChaptersCount() const;

    /**
     * @brief setter of chapters count
     * @param value
     */
    void setChaptersCount(int value);

    /**
     * @brief getter of verses count
     * @return int
     */
    int getVersesCount() const;

    /**
     * @brief setter of verses count
     * @param value
     */
    void setVersesCount(int value);

    /**
     * @brief getter of bible version
     * @return QString
     */
    QString getBibleVersion() const;

    /**
     * @brief setter of bible version
     * @param value
     */
    void setBibleVersion(const QString &value);

    /**
     * @brief getter of long name of book
     * @return QString
     */
    QString getLongName() const;

    /**
     * @brief setter of long name
     * @param value
     */
    void setLongName(const QString &value);

    /**
     * @brief getter of short name
     * @return QString
     */
    QString getShortName() const;

    /**
     * @brief setter of short name
     * @param value
     */
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
