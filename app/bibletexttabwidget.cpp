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
#include "bibletexttabwidget.h"
#include <QTextDocumentFragment>
#include <QTextBlock>
#include "bibletextblockdata.h"
#include "Logger.h"

BibleTextTabWidget::BibleTextTabWidget(BibleReaderCore *brc, QWidget *parent) :
    QTabWidget(parent)
{
    QString tabBarStyle = "QTabBar::tab {min-width:50px;color: black;border: 1px solid;border-top-left-radius: 5px;border-top-right-radius: 10px;padding:5px;}	\
                               QTabBar::tab:!selected {margin-top: 5px;} \
                               QTabBar::tab:selected {color: blue;}";
    //setStyleSheet(tabBarStyle);
    setElideMode(Qt::ElideNone);
    setUsesScrollButtons(true);
    brCore = brc;
    compareBrowser = new BibleTextCompareBrowser(brCore, this);
    connect(brCore, SIGNAL(currentVerseChanged(int,int,int)), compareBrowser,
            SLOT(showComparedBibleText()));


    // add BibleTextBrowser instance for each bible version
    bibles = brCore->getAllBibleVersions();

    for (int i = 0; i < bibles.count(); i++) {
        BibleTextBrowser *btBrowser = new BibleTextBrowser(brCore, this);
        connect(brCore, SIGNAL(currentVerseChanged(int,int,int)), btBrowser,
                SLOT(showCurrentChapter()));

        btBrowser->setBibleVersion(bibles[i].getVersion());
        btBrowser->showCurrentChapter();
        int index = addTab(btBrowser, bibles[i].getFullname());
        btBrowser->setBibleIndex(index);
        btBrowsers.push_back(btBrowser);
        if (brCore->getCurrentVersion() == bibles[i].getVersion()) {
            setCurrentIndex(index);
        }
    }

    compareBrowserTabIndex = addTab(compareBrowser, tr("Parallel Bible Text Tab"));

    // connect tab switch singal
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(showBibleText(int)));
    connect(brCore, SIGNAL(compareCurrentVerse()), this, SLOT(showCmpTxtTab()));
}

void BibleTextTabWidget::showBibleText(int index)
{
    if (index == compareBrowserTabIndex) {
        compareBrowser->showComparedBibleText();
    } else {
        btBrowsers[index]->showCurrentChapter(TabSwitch);
        brCore->setCurrentVersion(bibles[index].getVersion());
    }
}

void BibleTextTabWidget::onBibleVersionChanged(QString version)
{
    int index = -1;
    for (int i = 0; i < bibles.count(); i++) {
        if (bibles[i].getVersion() == version) {
            index = i;
            break;
        }
    }
    setCurrentIndex(index);
}

void BibleTextTabWidget::goToVerse(QString version, int book, int chapter, int verse)
{
    int index = -1;
    for (int i = 0; i < bibles.count(); i++) {
        if (bibles[i].getVersion() == version) {
            index = i;
            break;
        }
    }

    setCurrentIndex(index);
    brCore->setCurrentVersion(version);
    brCore->setCurrentBCV(book, chapter, verse);
}

void BibleTextTabWidget::showCmpTxtTab()
{
    setCurrentIndex(compareBrowserTabIndex);
    compareBrowser->showComparedBibleText();
}

