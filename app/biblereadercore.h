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

#ifndef BIBLEREADERCORE_H
#define BIBLEREADERCORE_H

#include <QObject>
#include <QString>

#include "bibleversepos.h"
#include "bibletextdao.h"
#include "bibledictdao.h"
#include "bibleinfo.h"
#include "bibledictinfo.h"
#include "biblereaderconfigurator.h"

/**
 * @brief
 * The core class of Bible Reader
 * this class will perform all actions
 * 1. bible reading
 * 2. search dictory
 * 3. bible text compare
 * 4. dictionary
 * @todo 3. commentary
 * @todo 4. Bible maps
 * @todo 5. original text processing
 */
class BibleReaderCore : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief BibleReaderCore constructor
     * @param parent
     */
    explicit BibleReaderCore(QObject *parent = 0);

    /**
     * @brief BibleReaderCore destructor
     */
    ~BibleReaderCore();

    /**
     * @brief get a verse which in book [book], chapter [chapter], verse [verse]
     * @param book book name
     * @param chapter chapter of verse in
     * @param verse verse number
     * @return the bible verse
     */
    BibleVerse getVerse(QString book, int chapter, int verse);

    /**
     * @brief get a verse which in book [bookNumber], chapter [chapter], verse [verse]
     * @param bookNumber the book number in database
     * @param chapter chapter number in database
     * @param verse verse number in database
     * @return the bible verse
     */
    BibleVerse getVerse(int bookNumber, int chapter, int verse);

    /**
     * @brief getVerse: get a verse which is book [bookNumber], chapter [chapter], verse [verse]
     * and using bible version [version]
     * @param version bible version
     * @param bookNumber the book number in database
     * @param chapter chapter number in database
     * @param verse verse number in database
     * @return the bible verse
     */
    BibleVerse getVerse(QString version, int bookNumber, int chapter, int verse);

    /**
     * @brief Get current bible information
     * @return BibleInfo of current bible
     */
    BibleInfo getCurrentBibleInfo();

    QString getVerses(BibleVersePos start, BibleVersePos end);
    BibleChapter getChapter(int bookNumber, int chapter);
    BibleChapter getChapter(QString version, int bookNumber, int chapter);
    QString getChapterText(int bookNumber, int chapter);
    QString getChapterText(QString version, int bookNumber, int chapter);

    /**
     * @brief search bible text with query [q]
     * @param q query string
     * @param scope the book we will go to search
     * @param customBooks if the scope is UserDefined, this will contains the books we will
     * go to search
     * @return all matched bible verse in a list
     */
    QList<BibleVerse> search(QString q,
                             BibleTextDAO::SearchScope scope = BibleTextDAO::All,
                             QList<int> customBooks = QList<int>());

    void copyVerses(BibleVersePos start, BibleVersePos end);

    QList<BibleBook> getAllBooks();
    QList<int> getChaptersCountOfOneBook(int n);

    QString getCurrentVersion() const;
    void setCurrentVersion(const QString &value);

    bool addBibleVersion(QString &version, QString &dataPath);
    bool addBibleVersions(QStringList &versions, QStringList &dataPaths);

    QString getCurrentDict() const;
    void setCurrentDict(const QString &dictName);

    bool addDictionary(QString &name, QString& path);

    // scan bible data
    QList<BibleInfo> getAllBibleVersions();
    // scan dictionarys data
    QList<BibleDictInfo> getAllDictionarys();

    // get all words and explainations of a dictionary
    QMap<QString, QString> getAllWordsAndExplainationsOfCurrentDict();
    QMap<QString, QString> getAllWordsAndExplainations(QString& dictName);

    QString getBiblePathBase() const;
    void setBiblePathBase(const QString &value);

    int getCurrentBookNumber() const;
    void setCurrentBookNumber(int value);

    int getCurrentChapterNumber() const;
    void setCurrentChapterNumber(int value);

    int getCurrentVerseNumber() const;
    void setCurrentVerseNumber(int value);

    int setCurrentBCV(int b, int c, int v);

    QString getExplaination(QString dict, QString word);

    BibleBook getCurrentBook(QList<BibleBook> &books);

signals:
    /**
     * @brief this signal will be emited when current
     * chapter changed.
     * @param book current book id
     * @param chapter current chapter id
     */
    void currentChapterChanged(int book, int chapter);

    /**
     * @brief this signal will be emited when current verse changed
     * @param book current book id
     * @param chapter current chapter id
     * @param verse current verse id
     */
    void currentVerseChanged(int book, int chapter, int verse);

    /**
     * @brief this signal will be emited when current bible version is changed
     * @param version
     */
    void currentBibleVersionChanged(QString version);

public slots:
    void navNextChapter();
    void navPrevChapter();

private:
    /**
     * @brief current bible version
     */
    QString currentVersion;
    /**
     * @brief current bible version DAO
     */
    BibleTextDAO *currentBibleDAO;

    /**
     * @brief current bible dict name
     */
    QString currentDict;
    /**
     * @brief current dictionary DAO
     */
    BibleDictDAO *currentDictDAO;

    /**
     * @brief current book number
     */
    int currentBookNumber;
    /**
     * @brief current chapter number
     */
    int currentChapterNumber;
    /**
     * @brief current verse number
     */
    int currentVerseNumber;
    /**
     * @brief previous verse number
     */
    int previousVerseNumber;

    /**
     * @brief <bible version, bible DAO>
     */
    QMap<QString, BibleTextDAO*> allBTDAOs;
    /**
     * @brief <dict name, dict DAO>
     */
    QMap<QString, BibleDictDAO*> allBDDAOs;

    /**
     * @brief bibles store path
     */
    QString biblePathBase;

    /**
     * @brief all bible versions
     */
    QList<BibleInfo> allBibles;

    /**
     * @brief dictionary store path
     */
    QString dictPathBase;

    /**
     * @brief all dictionarys
     */
    QList<BibleDictInfo> allDicts;

    /**
     * @brief Bible Reader configurator instance
     */
    BibleReaderConfigurator *configurator;

private:/* Helper functions*/
    int getBookID(QString bookName);
};

#endif // BIBLEREADERCORE_H
