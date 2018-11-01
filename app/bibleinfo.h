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
#ifndef BIBLEINFO_H
#define BIBLEINFO_H

#include <QString>

/**
 * @brief The BibleInfo class
 * this class represents bible information of a Bible
 */
class BibleInfo
{
public:
    BibleInfo();

    QString getVersion() const;
    void setVersion(const QString &value);

    QString getLanguage() const;
    void setLanguage(const QString &value);

    bool getHasSN() const;
    void setHasSN(bool value);

    QString getChapterString() const;
    void setChapterString(const QString &value);

    bool getRtl() const;
    void setRtl(bool value);

    QString getDataPath() const;
    void setDataPath(const QString &value);

    QString getDescription() const;
    void setDescription(const QString &value);

    QString getCopyright() const;
    void setCopyright(const QString &value);

    QString getFullname() const;
    void setFullname(const QString &value);

    int getNumber() const;
    void setNumber(int value);

private:
    /**
     * @brief bible version with full name
     */
    QString fullname;

    /**
     * @brief bible version
     * it is short name of bible version.
     * example: CUV CUVS LZZ
     */
    QString version;
    /**
     * @brief description of the bible
     *
     */
    QString description;
    /**
     * @brief language of the bible
     */
    QString language;
    /**
     * @brief has strong number or not
     */
    bool hasSN;
    /**
     * @brief the bible text is right to left or not
     */
    bool rtl;
    /**
     * @brief chapter string. [may be removed in future]
     */
    QString chapterString;
    /**
     * @brief the bible data file path
     */
    QString dataPath;
    /**
     * @brief copyright text
     */
    QString copyright;

    /**
     * @brief number
     * bible version, Ex: 1, 2, 3
     */
    int number;
};

#endif // BIBLEINFO_H
