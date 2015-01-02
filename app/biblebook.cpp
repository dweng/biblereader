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

#include "biblebook.h"

/**
 * @brief Constructor, and do nothing now.
 */
BibleBook::BibleBook()
{
}

/**
 * @brief Get book number.
 */
int BibleBook::getBookNumber() const
{
    return bookNumber;
}

/**
 * @brief Set book number.
 * @param value Book Number
 */
void BibleBook::setBookNumber(int value)
{
    bookNumber = value;
}

/**
 * @brief Get chapters count.
 * @return chapters count
 */
int BibleBook::getChaptersCount() const
{
    return chaptersCount;
}

/**
 * @brief Set chapters count.
 * @param value Chapters count.
 */
void BibleBook::setChaptersCount(int value)
{
    chaptersCount = value;
}

/**
 * @brief Get verses count of this book
 * @return verses count of this Book
 */
int BibleBook::getVersesCount() const
{
    return versesCount;
}

/**
 * @brief Get verses count of this Book.
 * @param value verses count of this Book to set.
 */
void BibleBook::setVersesCount(int value)
{
    versesCount = value;
}

/**
 * @brief Get bible version. will return a string like
 * "CUVS"
 * @return bible version string
 */
QString BibleBook::getBibleVersion() const
{
    return bibleVersion;
}

/**
 * @brief Set bible version.
 * @param value bible version string.
 */
void BibleBook::setBibleVersion(const QString &value)
{
    bibleVersion = value;
}

/**
 * @brief Get long name of this Book.
 * @return long name of this Book.
 */
QString BibleBook::getLongName() const
{
    return longName;
}

/**
 * @brief Set long name of this Book.
 * @param value long name of this Book to set
 */
void BibleBook::setLongName(const QString &value)
{
    longName = value;
}

/**
 * @brief Get short name of this Book.
 * @return short name of this Book.
 */
QString BibleBook::getShortName() const
{
    return shortName;
}

/**
 * @brief Set short name of this Book.
 * @param value short name of this Book to be set.
 */
void BibleBook::setShortName(const QString &value)
{
    shortName = value;
}

