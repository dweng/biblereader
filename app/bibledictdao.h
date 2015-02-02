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

#ifndef BIBLEDICTDAO_H
#define BIBLEDICTDAO_H

#include <QString>

// for sql
#include <QSqlQuery>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMap>

#include "bibledictinfo.h"

/**
 * @brief this class will access Bible dictionary database
 *
 * @version 1.0
 */
class BibleDictDAO
{
public:
    /**
     * @brief BibleDictDAO Constructor
     * @param dn dictionary name
     * @param dp dictionary database path
     */
    explicit BibleDictDAO(QString &dn, QString &dp);
    /**
     * @brief Destructor.
     */
    ~BibleDictDAO();
    /**
     * @brief Get explaination of a word.
     * @param word the word we need to get explaination
     * @return
     */
    QString getExplaination(QString &word);
    /**
     * @brief Get whole dictionary.
     * @return dictionary map
     */
    QMap<QString, QString> getAllWordsAndExplainations();
    /**
     * @brief Get Bible dictionary information.
     * @return
     */
    BibleDictInfo getBibleDictInfo();

private:
    /**
     * @brief init database connection.
     * @return true if succeed, false if failed
     */
    bool init();
    /**
     * @brief Cache all dictionary to memory.
     * @return true if succeed, false if failed.
     */
    bool initDictData();
    /**
     * @brief Add a word and its explaination to dictionary cache
     * @param word the word we want to add
     * @param explaination the explaination we want to add
     */
    void addWord(QString word, QString explaination);
private:
    /**
     * dictionary name
     * example: SNCHS (Strong Number Chinese version)
     */
    QString dictName;
    /**
     * dictionary db path
     * example: /Users/user/biblereader/dicts/
     */
    QString dictPath;

    /**
     * bible dictionary inforamtion
     * @todo for future usage
     */
    BibleDictInfo di;

    /**
     * dict database instance
     */
    QSqlDatabase dictDB;

    /**
     * database query instace for dictionary
     */
    QSqlQuery *query;
    /**
     * dictionary cache, will be inited in constructor
     */
    QMap<QString, QString> dicts;
};

#endif // BIBLEDICTDAO_H
