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
#ifndef BIBLEREADERCONFIGURATORCORE_H
#define BIBLEREADERCONFIGURATORCORE_H

#include <QObject>
#include <QString>
#include <QSettings>

/**
 * @brief The BibleReaderConfigurator class
 * this class will perform all configurating jobs of BibleReader
 */
class BibleReaderConfigurator : public QObject
{
    Q_OBJECT
public:
    explicit BibleReaderConfigurator(QObject *parent = 0);
    ~BibleReaderConfigurator();

    QString getBiblePathBase() const;
    void setBiblePathBase(const QString &value);

    QString getDefaultBibleVersion() const;
    void setDefaultBibleVersion(const QString &value);

    int getLastBook() const;
    void setLastBook(int value);

    int getLastChapter() const;
    void setLastChapter(int value);

    int getLastVerse() const;
    void setLastVerse(int value);

    QString getDictPathBase() const;
    void setDictPathBase(QString& value);

    QString getDefaultDict() const;
    void setDefaultDict(QString &value);

    QString getDefaultCommentary() const;
    void setDefaultCommentary(QString &value);

    QString getBcPathBase() const;
    void setBcPathBase(const QString &value);

signals:

public slots:

private:
    /**
     * @brief biblePathBase
     * this path is point to where bibles stored
     */
    QString biblePathBase;
    /**
     * @brief dictionary path base
     * this path is point to where dictionatys stored
     */
    QString dictPathBase;

    /**
     * @brief bible commentary path base
     */
    QString bcPathBase;

    /**
     * @brief default dictionary
     */
    QString defaultDict;
    /**
     * @brief defaultBibleVersion
     * BibleReader will open this default bible on startup
     */
    QString defaultBibleVersion;

    /**
     * @brief BR will open this commentary on start up
     */
    QString defaultCommentary;

    /**
     * @brief settings
     * this QSettings pointer will store biblereader.ini's information.
     */
    QSettings *settings;
    /**
     * @brief settingsFile
     * the configuration file name, default to "biblereader.ini"
     */
    QString settingsFile;
    /**
     * @brief currentBook
     * the previous book which is reading
     */
    int lastBook;
    /**
     * @brief lastChapter
     * the previous chapter which is reading
     */
    int lastChapter;
    /**
     * @brief lastVerse
     * the previous verse which is reading
     */
    int lastVerse;

};

#endif // BIBLEREADERCONFIGURATORCORE_H
