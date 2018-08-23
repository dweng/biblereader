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
#include "biblereadercore.h"
#include <QDir>
#include <QStringList>
// for logging
#include "Logger.h"
#include "ConsoleAppender.h"

BibleReaderCore::BibleReaderCore(QObject *parent) :
    QObject(parent)
{
    LOG_DEBUG("constructor");
    resManager = new BibleReaderResourceManager(this);
    resManager->setResourceUrl(QUrl("http://files.biblereader.cn/resources/all.xml"));
    // test

    currentBookNumber = 0;
    currentChapterNumber = 0;
    currentVerseNumber = 0;
    previousVerseNumber = 0;
    previousBookNumber = 0;
    previousChapterNumber = 0;
}

BibleReaderCore::~BibleReaderCore()
{
    LOG_DEBUG("destructor");
    qDeleteAll(allBTDAOs);
    qDeleteAll(allBDDAOs);
    qDeleteAll(allBCDAOs);
    qDeleteAll(allBXDAOs);

    configurator->setLastBook(currentBookNumber);
    configurator->setLastChapter(currentChapterNumber);
    configurator->setLastVerse(currentVerseNumber);

    delete configurator;
    delete resManager;
}

void BibleReaderCore::init()
{
    // get configurations of BibleReader
    configurator = new BibleReaderConfigurator();
    // process bibles
    biblePathBase = configurator->getBiblePathBase();
    getAllBibleVersions();
    setCurrentVersion(configurator->getDefaultBibleVersion());

    // process dicts
    dictPathBase = configurator->getDictPathBase();
    getAllDictionarys();
    setCurrentDict(configurator->getDefaultDict());

    // process commentarys
    bcPathBase = configurator->getBcPathBase();
    getAllCommentarys();
    setCurrentCommentary(configurator->getDefaultCommentary());

    // process xrefs
    bxPathBase = configurator->getBxPathBase();
    LOG_INFO() << bxPathBase;
    getAllXrefs();

    currentBookNumber = configurator->getLastBook();
    currentChapterNumber = configurator->getLastChapter();
    currentVerseNumber = configurator->getLastVerse();

    // store first verse history
    history.push_back(BibleVersePos(currentBookNumber, currentChapterNumber, currentVerseNumber));
    historyPos = 0;
}

BibleVerse BibleReaderCore::getVerse(QString book, int chapter, int verse)
{
    int bookID = getBookID(book);
    return currentBibleDAO->getOneVerse(bookID, chapter, verse);
}

BibleVerse BibleReaderCore::getVerse(int bookNumber, int chapter, int verse)
{
    return currentBibleDAO->getOneVerse(bookNumber, chapter, verse);
}

BibleVerse BibleReaderCore::getVerse(QString version, int bookNumber, int chapter, int verse)
{
    return allBTDAOs.value(version)->getOneVerse(bookNumber, chapter, verse);
}

BibleInfo BibleReaderCore::getCurrentBibleInfo()
{
    return currentBibleDAO->getBibleInfo();
}


QString BibleReaderCore::getVerses(BibleVersePos start, BibleVersePos end)
{
    QList<BibleVerse> verses = currentBibleDAO->getVerses(start, end);

    QString text;
    for (int i = 0; i < verses.size(); i++) {
        text.append(verses[i].text()).append("\n");
    }
    LOG_INFO() << text;
    return text;
}

BibleChapter BibleReaderCore::getChapter(int bookNumber, int chapter)
{
    return currentBibleDAO->getChapter(bookNumber, chapter);
}

BibleChapter BibleReaderCore::getChapter(QString version, int bookNumber, int chapter)
{
    return allBTDAOs.value(version)->getChapter(bookNumber, chapter);
}


QString BibleReaderCore::getChapterText(int bookNumber, int chapter)
{
    return currentBibleDAO->getChapter(bookNumber, chapter).text();
}

QString BibleReaderCore::getChapterText(QString version,
                                    int bookNumber,
                                    int chapter)
{
    return allBTDAOs.value(version)->getChapter(bookNumber, chapter).text();
}

QList<BibleVerse> BibleReaderCore::search(QString q,
                                          BibleTextDAO::SearchScope scope,
                                          QList<int> customBooks)
{
    return currentBibleDAO->search(q, scope, customBooks);
}

bool BibleReaderCore::copyVerses(BibleVersePos start, BibleVersePos end)
{
    return true;
}

QList<BibleBook> BibleReaderCore::getAllBooks()
{
    return currentBibleDAO->getAllBooks();
}

QList<int> BibleReaderCore::getChaptersCountOfOneBook(int n)
{
    return currentBibleDAO->getChaptersCountOfOneBook(n);
}

void BibleReaderCore::navNextChapter()
{
    if (currentBookNumber == 0 || currentChapterNumber == 0
            || currentVerseNumber == 0) {
        return;
    }

    // next chapter is in the same book
    QList<BibleBook> allBooks = getAllBooks();
    BibleBook currentBook = getCurrentBook(allBooks);
    int chapterCount = currentBook.getChaptersCount();
    if (currentChapterNumber < chapterCount) {
        currentChapterNumber++;
    } else if (currentChapterNumber == chapterCount){
        if (currentBookNumber == allBooks.last().getBookNumber()) {
            // do nothing
        } else {
            // get next book and set current chapter to 1, current verse to 1
            for (int i = 0; i < allBooks.count(); i++) {
                if (currentBookNumber == allBooks[i].getBookNumber()) {
                    currentBookNumber = allBooks[i+1].getBookNumber();
                    currentChapterNumber = 1;
                    break;
                }
            }
        }
    }
    currentVerseNumber = 1;
    setCurrentBCV(currentBookNumber, currentChapterNumber, currentVerseNumber);
}

void BibleReaderCore::navPrevChapter()
{
    if (currentBookNumber == 0 || currentChapterNumber == 0
            || currentVerseNumber == 0) {
        return;
    }

    // previous chapter is in the same book
    QList<BibleBook> allBooks = getAllBooks();
    if (currentChapterNumber > 1) {
        currentChapterNumber--;
    } else if (currentChapterNumber == 1){
        if (currentBookNumber == allBooks.first().getBookNumber()) {
            // do nothing
        } else {
            // get next book and set current chapter to 1, current verse to 1
            for (int i = 0; i < allBooks.count(); i++) {
                if (currentBookNumber == allBooks[i].getBookNumber()) {
                    currentBookNumber = allBooks[i-1].getBookNumber();
                    currentChapterNumber = allBooks[i-1].getChaptersCount();
                    break;
                }
            }
        }
    }
    currentVerseNumber = 1;
    setCurrentBCV(currentBookNumber, currentChapterNumber, currentVerseNumber);
}

void BibleReaderCore::fireCmpCurVerse()
{
    emit compareCurrentVerse();
}

void BibleReaderCore::fireShowDictItem(QString dictName, QString itemName)
{
    emit showDictItem(dictName, itemName);
}

void BibleReaderCore::navBackHistory()
{
    if (history.size() > 1) {
        historyPos--;
        setCurrentBCV(history[historyPos].getBookNumber(),
                history[historyPos].getChapterNumber(),
                history[historyPos].getVerseNumber(), 1);

        if (historyPos == 0) {
            emit navToHistoryItem(0);
        } else {
            emit navToHistoryItem(historyPos);
        }
    }
}

void BibleReaderCore::navForwordHistory()
{
    if (history.size() > 1) {
        historyPos++;
        setCurrentBCV(history[historyPos].getBookNumber(),
                history[historyPos].getChapterNumber(),
                history[historyPos].getVerseNumber(), 1);

        if (historyPos == history.size()-1) {
            emit navToHistoryItem(-1);
        } else {
            emit navToHistoryItem(historyPos);
        }
    }
}

void BibleReaderCore::fireSearchRequest(QString q)
{
    emit searchRequest(q);
}

int BibleReaderCore::getPreviousBookNumber() const
{
    return previousBookNumber;
}

void BibleReaderCore::setPreviousBookNumber(int value)
{
    previousBookNumber = value;
}

int BibleReaderCore::getPreviousChapterNumber() const
{
    return previousChapterNumber;
}

void BibleReaderCore::setPreviousChapterNumber(int value)
{
    previousChapterNumber = value;
}

int BibleReaderCore::getPreviousVerseNumber() const
{
    return previousVerseNumber;
}

void BibleReaderCore::setPreviousVerseNumber(int value)
{
    previousVerseNumber = value;
}

int BibleReaderCore::getHistoryPos() const
{
    return historyPos;
}

void BibleReaderCore::setHistoryPos(int value)
{
    historyPos = value;
}

QList<BibleVerseXref> BibleReaderCore::getXrefsByChapter(int book, int chapter)
{
    QList<BibleVerseXref> result;
    result.clear();
    for (int i = 0; i < allBXDAOs.values().size(); i++) {
        QList<BibleVerseXref> tmp = allBXDAOs.values()[i]->getXrefsByChapter(book, chapter);
        result += tmp;
    }

    return result;
}

QList<BibleVerseXref> BibleReaderCore::getXrefsByVerse(int book, int chapter, int verse)
{
    QList<BibleVerseXref> result;
    result.clear();
    for (int i = 0; i < allBXDAOs.values().size(); i++) {
        QList<BibleVerseXref> tmp = allBXDAOs.values()[i]->getXrefsByVerse(book, chapter, verse);
        result += tmp;
    }

    return result;
}

void BibleReaderCore::navNextBook()
{
    if (currentBookNumber == 66) return;

    setCurrentBCV(currentBookNumber+1, 1, 1);
}

void BibleReaderCore::navPrevBook()
{
    if (currentBookNumber == 1) return;
    setCurrentBCV(currentBookNumber-1, 1, 1);
}

QList<BibleVersePos> BibleReaderCore::getHistory() const
{
    return history;
}

BibleReaderResourceManager *BibleReaderCore::getResManager() const
{
    return resManager;
}


QString BibleReaderCore::getBcPathBase() const
{
    return bcPathBase;
}

void BibleReaderCore::setBcPathBase(const QString &value)
{
    bcPathBase = value;
}

bool BibleReaderCore::addCommentary(QString &name, QString &path)
{
    LOG_INFO() << "add commentary:" << name << ", " << path;
    if (!allBCDAOs.contains(name)) {
        BibleCommentaryDAO *bcDAO = new BibleCommentaryDAO(name, path);
        allBCDAOs.insert(name, bcDAO);
        emit loadResourceFinished(bcDAO->getCmtInfo().getFullname());
    }

    return true;
}

bool BibleReaderCore::addXref(QString &name, QString &path)
{
    LOG_INFO() << "add xref:" << name << ", " << path;
    if (!allBXDAOs.contains(name)) {
        BibleXRefsDAO *bxDAO = new BibleXRefsDAO(name, path);
        allBXDAOs.insert(name, bxDAO);
    }

    return true;
}

QString BibleReaderCore::getChapterCmt(QString cn, int book, int chapter)
{
    return allBCDAOs.value(cn)->getChapterCmt(book, chapter);
}

QString BibleReaderCore::getCurrentCommentary() const
{
    return currentCommentary;
}

void BibleReaderCore::setCurrentCommentary(const QString &value)
{
    LOG_INFO() << "set current commentary:" << value;
    if (allBCDAOs.contains(value)) {
        currentCommentary = value;
        currentCmtDAO = allBCDAOs.value(value);

        configurator->setDefaultCommentary(value);

        // some sigal can be emit here
    }
}

BibleReaderConfigurator *BibleReaderCore::getConfigurator() const
{
    return configurator;
}

void BibleReaderCore::setConfigurator(BibleReaderConfigurator *value)
{
    configurator = value;
}


int BibleReaderCore::getCurrentVerseNumber() const
{
    return currentVerseNumber;
}

void BibleReaderCore::setCurrentVerseNumber(int value)
{
    currentVerseNumber = value;
}

/**
 * @brief BibleReaderCore::setCurrentBCV
 * @param b
 * @param c
 * @param v
 * @param operation 0 default, 1 history navigation
 * @return
 */
int BibleReaderCore::setCurrentBCV(int b, int c, int v, int operation)
{
    // set previous verse
    setPreviousBookNumber(currentBookNumber);
    setPreviousChapterNumber(currentChapterNumber);
    setPreviousVerseNumber(currentVerseNumber);

    // set current
    setCurrentBookNumber(b);
    setCurrentChapterNumber(c);
    setCurrentVerseNumber(v);

    // history navigation, not store to history list
    if (operation == 0) {
        history.push_back(BibleVersePos(b, c, v));
        historyPos = history.size()-1;
        emit navToHistoryItem(-1);
    }
    emit currentVerseChanged(b, c, v);
    return 1;
}

QString BibleReaderCore::getExplaination(QString dict, QString word)
{
    return allBDDAOs.value(dict)->getExplaination(word);
}

int BibleReaderCore::getCurrentChapterNumber() const
{
    return currentChapterNumber;
}

void BibleReaderCore::setCurrentChapterNumber(int value)
{
    currentChapterNumber = value;
}

int BibleReaderCore::getCurrentBookNumber() const
{
    return currentBookNumber;
}

void BibleReaderCore::setCurrentBookNumber(int value)
{
    currentBookNumber = value;
}

QString BibleReaderCore::getBiblePathBase() const
{
    return biblePathBase;
}

void BibleReaderCore::setBiblePathBase(const QString &value)
{
    biblePathBase = value;
}

QString BibleReaderCore::getCurrentVersion() const
{
    return currentVersion;
}

void BibleReaderCore::setCurrentVersion(const QString &version)
{
    if (allBTDAOs.contains(version)) {
        this->currentVersion = version;
        // set correct current version in configurator
        configurator->setDefaultBibleVersion(currentVersion);
        this->currentBibleDAO = allBTDAOs.value(version);

        emit currentBibleVersionChanged(version);
    }
}

bool BibleReaderCore::addBibleVersion(QString &version, QString &dataPath)
{
    BibleTextDAO *btdao = new BibleTextDAO(version, dataPath);
    allBTDAOs.insert(version, btdao);
    emit loadResourceFinished(btdao->getBibleInfo().getFullname());
    return true;
}

bool BibleReaderCore::addBibleVersions(QStringList &versions, QStringList &dataPaths)
{
    if ((versions.count() == dataPaths.count())) {
        for (int i = 0; i < versions.count(); i++) {
            addBibleVersion(versions[i], dataPaths[i]);
        }
    }

    return true;
}

QString BibleReaderCore::getCurrentDict() const
{
    return currentDict;
}

void BibleReaderCore::setCurrentDict(const QString &dictName)
{
    LOG_INFO() << "current dictionary:" << dictName;
    if (allBDDAOs.contains(dictName)) {
        currentDict = dictName;
        currentDictDAO = allBDDAOs.value(dictName);
        configurator->setDefaultDict(currentDict);
    }
}

bool BibleReaderCore::addDictionary(QString &name, QString &path)
{
    BibleDictDAO *bddao = new BibleDictDAO(name, path);
    allBDDAOs.insert(name,
                     bddao);
    emit loadResourceFinished(bddao->getBibleDictInfo().getFullname());
    return true;
}

QList<BibleInfo> BibleReaderCore::getAllBibleVersions()
{
    if (allBibles.empty()) {
        QDir biblePath = QDir(biblePathBase);
        if (!biblePath.exists()) {
            return allBibles;
        }

        biblePath.setFilter(QDir::AllDirs | QDir::NoSymLinks);
        QFileInfoList bibleVersions = biblePath.entryInfoList();

        for (int i = 0; i < bibleVersions.count(); i++) {
            QString bibleVersion = bibleVersions[i].fileName();
            if (bibleVersion == "." || bibleVersion == "..") continue;

            QString bibleDataPath = biblePathBase + bibleVersion+QDir::separator()+bibleVersion+".BIB";
            addBibleVersion(bibleVersion, bibleDataPath);
            BibleInfo bi = allBTDAOs.value(bibleVersion)->getBibleInfo();

            allBibles.push_back(bi);
        }
    }
    return allBibles;
}

QList<BibleDictInfo> BibleReaderCore::getAllDictionarys()
{
    LOG_INFO() << "add dictionary in:"<<dictPathBase;
    QString dict;
    QString dictDataPath;

    if (allDicts.empty()) {
        QDir dictPath = QDir(dictPathBase);
        if (!dictPath.exists()) {
            return allDicts;
        }

        dictPath.setFilter(QDir::AllDirs | QDir::NoSymLinks);
        QFileInfoList dicts = dictPath.entryInfoList();

        for (int i = 0; i < dicts.count(); i++) {
            dict = dicts[i].fileName();
            if (dict == "." || dict == "..") continue;

            dictDataPath = dictPathBase + dict +QDir::separator()+dict+".BIBDICT";
            addDictionary(dict, dictDataPath);
            BibleDictInfo di = allBDDAOs.value(dict)->getBibleDictInfo();

            allDicts.push_back(di);
        }
    }
    return allDicts;
}

QList<BibleCommentaryInfo> BibleReaderCore::getAllCommentarys()
{
    LOG_INFO() << "add commentayrs in:"<<bcPathBase;
    QString cmt;
    QString cmtDataPath;

    if (allCmts.empty()) {
        QDir cmtPath = QDir(bcPathBase);
        if (!cmtPath.exists()) {
            return allCmts;
        }

        cmtPath.setFilter(QDir::AllDirs | QDir::NoSymLinks);
        QFileInfoList cmts = cmtPath.entryInfoList();

        for (int i = 0; i < cmts.count(); i++) {
            cmt = cmts[i].fileName();
            if (cmt == "." || cmt == "..") continue;

            cmtDataPath = bcPathBase + cmt;
            addCommentary(cmt, cmtDataPath);
            BibleCommentaryInfo ci = allBCDAOs.value(cmt)->getCmtInfo();

            allCmts.push_back(ci);
        }
    }

    return allCmts;
}

QList<QString> BibleReaderCore::getAllXrefs()
{
    LOG_INFO() << "add xrefs in:" <<bxPathBase;
    QString xref;
    QString xrefDataPath;

    if (allXrefs.empty()) {
        QDir xrefPath = QDir(bxPathBase);
        if (!xrefPath.exists()) {
            return allXrefs;
        }

        xrefPath.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QFileInfoList xrefs = xrefPath.entryInfoList();

        for (int i = 0; i < xrefs.count(); i++) {
            xref = xrefs[i].fileName();
            xrefDataPath = bxPathBase + xref;
            addXref(xref, xrefDataPath);

            allXrefs.push_back(xref);
        }
    }

    return allXrefs;
}

QMap<QString, QString> BibleReaderCore::getAllWordsAndExplainationsOfCurrentDict()
{
    LOG_INFO() << "dict name:"<<currentDict;
    return currentDictDAO->getAllWordsAndExplainations();
}


int BibleReaderCore::getBookID(QString bookName)
{
    return currentBibleDAO->getBookIDByName(bookName);
}

BibleBook BibleReaderCore::getCurrentBook(QList<BibleBook> &books)
{
    for (int i = 0; i < books.count(); i++) {
        if (currentBookNumber == books[i].getBookNumber()) {
            return books[i];
        }
    }

    return BibleBook();
}

QMap<QString, QString> BibleReaderCore::getAllWordsAndExplainations(QString &dictName)
{
    if (allBDDAOs.contains(dictName)) {
        return allBDDAOs.value(dictName)->getAllWordsAndExplainations();
    }

    return QMap<QString, QString>();
}
