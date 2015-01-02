/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese Simplified.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 *
 */
#ifndef BIBLETEXTTABWIDGET_H
#define BIBLETEXTTABWIDGET_H

#include <QTabWidget>
#include <QList>
#include "biblereadercore.h"
#include "bibletextbrowser.h"
#include "bibletextcomparebrowser.h"

/**
 * @brief This class will represent the tab container of
 * Bible versions. all bible versions will be in this container and
 * each bible version represents a tab.
 *
 * also a compare tab with all versions will be added.
 *
 * @version 1.0
 */
class BibleTextTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit BibleTextTabWidget(BibleReaderCore *brc, QWidget *parent = 0);

signals:

public slots:

private slots:
    /**
     * @brief this slot will show parallel bible text in
     * parallel tab
     */
    void showBibleText(int index);

    /**
     * @brief this slot will do some jobs when current bible version changed.
     * @param version current bible version
     */
    void onBibleVersionChanged(QString version);

    /**
     * @brief set $version as current bible version and get verse by
     * [book], [chapter], [verse]
     * @param version bible version
     * @param book book id
     * @param chapter chapter id
     * @param verse verse id
     */
    void goToVerse(QString version, int book, int chapter, int verse);
private:
    /**
     * @brief BibleReaderCore instance
     */
    BibleReaderCore *brCore;

    /**
     * @brief Bible Text browsers
     */
    QList<BibleTextBrowser *> btBrowsers;

    /**
     * @brief this bible text browser is to show
     * parallel bible text for current chapter and current
     * verse
     */
    BibleTextCompareBrowser *compareBrowser;

    /**
     * @brief Compare browser tab index
     */
    int compareBrowserTabIndex;

    /**
     * @brief information of all bibles
     */
    QList<BibleInfo> bibles;
};

#endif // BIBLETEXTTABWIDGET_H
