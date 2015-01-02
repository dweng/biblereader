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
#ifndef BIBLETEXTBLOCKDATA_H
#define BIBLETEXTBLOCKDATA_H

#include <QTextBlockUserData>
#include <QString>

/**
 * @brief The class will store bible verse data. and will be set by
 * setUserData of QTextBlock
 */
class BibleTextBlockData : public QTextBlockUserData
{
public:
    /**
     * @brief constructor
     * @param bv bible version
     * @param b book id
     * @param c chapter id
     * @param v verse id
     */
    BibleTextBlockData(QString bv, int b, int c, int v);
    /**
     * @brief destructor
     */
    ~BibleTextBlockData();

    /**
     * @brief getter of book
     * @return book id
     */
    int getBook() const;
    /**
     * @brief setter of book
     * @param value book id to be set
     */
    void setBook(int value);
    /**
     * @brief chapter getter
     * @return chapter id
     */
    int getChapter() const;
    /**
     * @brief chapter setter
     * @param value chapter id to be set
     */
    void setChapter(int value);
    /**
     * @brief verse setter
     * @return verse id
     */
    int getVerse() const;
    /**
     * @brief verse setter
     * @param value verse id to be set
     */
    void setVerse(int value);
    /**
     * @brief bibleVersion getter
     * @return bible version
     */
    QString getBibleVersion() const;
    /**
     * @brief bibleVersion setter
     * @param value bible version to be set
     */
    void setBibleVersion(const QString &value);

private:
    /**
     * @brief bible version of this text block
     */
    QString bibleVersion;

    /**
     * @brief book id of this text block
     */
    int book;
    /**
     * @brief chapter id of this text block
     */
    int chapter;
    /**
     * @brief verse id of this text block
     */
    int verse;
};

#endif // BIBLETEXTBLOCKDATA_H
