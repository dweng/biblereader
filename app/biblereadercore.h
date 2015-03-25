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
#include "biblecommentarydao.h"
/**
 * @brief
 * The core class of Bible Reader
 * this class will perform all actions
 * 1. bible reading
 * 2. search dictory
 * 3. bible text compare
 * 4. dictionary
 * 5. commentary
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

    /**
     * @brief getVerses
     * Get verses from [start] to [end]
     * @param start
     * @param end
     * @return bible text of these verses
     */
    QString getVerses(BibleVersePos start, BibleVersePos end);

    /**
     * @brief getChapter
     * Get a bible chapter of default bible version
     * @param bookNumber book id
     * @param chapter chapter id
     * @return a chapter of a bible book
     */
    BibleChapter getChapter(int bookNumber, int chapter);

    /**
     * @brief getChapter
     * Get a bible chapter of [version]
     * @param version bible version
     * @param bookNumber book id
     * @param chapter chapter id
     * @return a chapter of a bible book of [version]
     */
    BibleChapter getChapter(QString version, int bookNumber, int chapter);

    /**
     * @brief BibleReaderCore::getChapterText
     * To get one chapter from bible book which has number in database.
     *
     * @param bookNumber book number the chapter in
     * @param chapter chapter number
     * @return {QString} chapter string
     */
    QString getChapterText(int bookNumber, int chapter);

    /**
     * @brief BibleReaderCore::getChapterText
     * To get one chapter from bible book which has number in database.
     *
     * @param version bible version
     * @param bookNumber book number the chapter in
     * @param chapter chapter number
     * @return {QString} chapter string
     */
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

    /**
     * @todo
     * @brief copyVerses
     * copy verse from [start] to [end]
     * @param start start verse
     * @param end end verse
     */
    bool copyVerses(BibleVersePos start, BibleVersePos end);

    /**
     * @brief getAllBooks
     * Get all books of current bible
     * @return a book list
     */
    QList<BibleBook> getAllBooks();

    /**
     * @brief getChaptersCountOfOneBook
     * see as name :)
     * @param n book id
     * @return
     */
    QList<int> getChaptersCountOfOneBook(int n);

    /**
     * @brief getCurrentVersion
     * current bible version getter
     * @return current version
     */
    QString getCurrentVersion() const;

    /**
     * @brief setCurrentVersion
     * current bible version setter
     * @param value the version want to be set
     */
    void setCurrentVersion(const QString &value);

    /**
     * @brief addBibleVersion
     * add bible [version] to bibles list, and this bible data is store at
     * [dataPath]
     * @param version bible version
     * @param dataPath bible data stored path
     * @return succeed with true else false
     */
    bool addBibleVersion(QString &version, QString &dataPath);

    /**
     * @brief addBibleVersions
     * add bibles [versions] to bibles list, and these bible data are stored at
     * [dataPaths] list
     * @param versions bible versions' list
     * @param dataPaths bible data paths list
     * @return
     */
    bool addBibleVersions(QStringList &versions, QStringList &dataPaths);

    /**
     * @brief getCurrentDict
     * @return current dict name
     */
    QString getCurrentDict() const;

    /**
     * @brief setCurrentDict
     * set current dict name
     * @param dictName dictionary name
     */
    void setCurrentDict(const QString &dictName);

    /**
     * @brief addDictionary
     * add a dict to dict DB instances map
     * @param name dict name
     * @param path dict database file path
     * @return true if succeed or false if failed
     */
    bool addDictionary(QString &name, QString& path);

    /**
     * @brief scan bible data
     * @return a list contains all bible versions information
     */
    QList<BibleInfo> getAllBibleVersions();

    /**
     * @brief scan dictionarys data
     * @return a list contains all dicts information
     */
    QList<BibleDictInfo> getAllDictionarys();

    /**
     * @brief scan commentarys data
     * @return a list contains all commentarys information
     */
    QList<BibleCommentaryInfo> getAllCommentarys();

    /**
     * @brief get all words and explainations of current dictionary
     */
    QMap<QString, QString> getAllWordsAndExplainationsOfCurrentDict();

    /**
     * @brief getAllWordsAndExplainations
     * get all words and explainations of a dictionary
     * @param dictName dict name
     * @return dict map
     */
    QMap<QString, QString> getAllWordsAndExplainations(QString& dictName);

    /**
     * @brief getBiblePathBase
     * return base path of bibles' store
     * @return QString
     */
    QString getBiblePathBase() const;

    /**
     * @brief setBiblePathBase
     * set base path of bibles' store
     * @param QString base path of bibles' store
     */
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

    BibleReaderConfigurator *getConfigurator() const;
    void setConfigurator(BibleReaderConfigurator *value);

    QString getCurrentCommentary() const;
    void setCurrentCommentary(const QString &value);

    QString getBcPathBase() const;
    void setBcPathBase(const QString &value);

    bool addCommentary(QString &name, QString &path);
    QString getChapterCmt(int book, int chapter);
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

    /**
     * @brief this signal will be emited when user triggered [compare this verse]
     * action in menu or on toolbar
     */
    void compareCurrentVerse();

public slots:
    /**
     * @brief navigate to next chapter
     */
    void navNextChapter();

    /**
     * @brief navigate to previous chapter
     */
    void navPrevChapter();

    /**
     * @brief this slot will fire compareCurrentVerse singal.
     * for public scope usage
     */
    void fireCmpCurVerse();

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
     * @brief currentCommentary
     */
    QString currentCommentary;

    /**
     * @brief currentCmtDAO
     */
    BibleCommentaryDAO *currentCmtDAO;

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
     * @brief <commentary name, commentary DAO>
     */
    QMap<QString, BibleCommentaryDAO*> allBCDAOs;

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
     * @brief bcPathBase
     */
    QString bcPathBase;

    /**
     * @brief allCmts
     */
    QList<BibleCommentaryInfo> allCmts;

    /**
     * @brief Bible Reader configurator instance
     */
    BibleReaderConfigurator *configurator;

private:/* Helper functions*/
    int getBookID(QString bookName);
};

#endif // BIBLEREADERCORE_H
