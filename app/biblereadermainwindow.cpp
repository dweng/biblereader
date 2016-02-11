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
 */
#include "biblereadermainwindow.h"
#include "biblereadercore.h"
#include <QMessageBox>
#include <QDockWidget>
#include <QTreeWidget>
#include <QGridLayout>
#include <QToolBar>
#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QDir>
#include <QStatusBar>
#include <QListWidget>
#include <QSplitter>
// for document
#include <QTextBlock>
#include <QTextCursor>
#include <QClipboard>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

#include <QDesktopServices>

#include "bibletreewidget.h"
#include "bibledictionarywidget.h"
#include "biblereaderaboutdlg.h"
#include "biblereaderdonationdlg.h"

// for logging
#include "Logger.h"
#include "ConsoleAppender.h"

BibleReaderMainWindow::BibleReaderMainWindow(BibleReaderCore *brc, QWidget *parent)
    : QMainWindow(parent)
{
    LOG_DEBUG("constructor");
    brConfigDlg = NULL;
    brProjectDlg = NULL;

    bibleReaderCore = brc;

    //Set main window
    this->setMinimumSize(640, 480);
    this->setWindowTitle(tr("Bible Reader"));
    this->showMaximized();

    // set toolbar
    toolBar = new QToolBar(tr("Bible Navigator"), this);
    toolBar->show();

    navToPrevChapterAction = new QAction(QIcon(QString(":/img/assets/images/arrow_left.png")), tr("previous chapter"),this);
    navToPrevChapterAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    toolBar->addAction(navToPrevChapterAction);
    connect(navToPrevChapterAction, SIGNAL(triggered()), this, SLOT(navToPrevChapter()));

    navToNextChapterAction = new QAction(QIcon(QString(":/img/assets/images/arrow_right.png")),tr("next chapter"),this);
    navToNextChapterAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    toolBar->addAction(navToNextChapterAction);
    connect(navToNextChapterAction, SIGNAL(triggered()), this, SLOT(navToNextChapter()));
    this->addToolBar(toolBar);

    // set bible text tab widget
    btTabWidget = new BibleTextTabWidget(bibleReaderCore, this);
    this->setCentralWidget(btTabWidget);

    // set search dockable widget
    bibleSearchDockWidget = new QDockWidget(tr("Bible Search"),this);
    bibleSearchDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    bibleSearchDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, bibleSearchDockWidget);
    bibleSearchWidget = new BibleSearchWidget(bibleReaderCore, this);
    connect(bibleSearchWidget, SIGNAL(goToVerse(QString,int,int,int)), btTabWidget,
            SLOT(goToVerse(QString,int,int,int)));
    connect(bibleReaderCore, SIGNAL(currentBibleVersionChanged(QString)), bibleSearchWidget,
            SLOT(onBibleVersionChanged(QString)));
    bibleSearchDockWidget->setWidget(bibleSearchWidget);

    // Set dockable widgets
    // bible tree dock widget
    bibleTreeDockWidget = new QDockWidget(tr("Bible Tree"),this);
    bibleTreeDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    bibleTreeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, bibleTreeDockWidget);

    // commentary dock widget
    commentaryDockWidget = new QDockWidget(tr("Commentary"), this);
    commentaryDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    commentaryDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, commentaryDockWidget);
    bcTabWidget = new BibleCommentaryTabWidget(bibleReaderCore, this);
    commentaryDockWidget->setWidget(bcTabWidget);

    // bible tree
    bibleTreeWidget = new BibleTreeWidget(bibleReaderCore, this);
    bibleTreeDockWidget->setWidget(bibleTreeWidget);
    connect(bibleTreeWidget,
            SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                   this, SLOT(onChapterVerseChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(bibleReaderCore, SIGNAL(currentBibleVersionChanged(QString)), bibleTreeWidget,
            SLOT(onBibleVersionChanged(QString)));
    // build menus
    mainMenuBar = new QMenuBar(this);
    fileMenu = new QMenu(tr("File(&F)"), mainMenuBar);
    editMenu = new QMenu(tr("Edit(&E)"), mainMenuBar);
    toolMenu = new QMenu(tr("Tools(&T)"), mainMenuBar);
    helpMenu = new QMenu(tr("Help(&H)"), mainMenuBar);

    // add actions to File menu
    mainMenuBar->addMenu(fileMenu);
    exitAppAction = fileMenu->addAction(tr("Exit"));
    connect(exitAppAction, SIGNAL(triggered()), this, SLOT(quitBibleReader()));
    projectVersesAction = fileMenu->addAction(tr("Project Verses"));
    connect(projectVersesAction, SIGNAL(triggered()), this, SLOT(projectVerses()));

    // add actions to Edit menu
    mainMenuBar->addMenu(editMenu);
    copyAction = editMenu->addAction(tr("Copy current verse"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyCurrentVerse()));

    // add actions to Tool menu
    mainMenuBar->addMenu(toolMenu);
    configAction = toolMenu->addAction(tr("Configure"));
    connect(configAction, SIGNAL(triggered(bool)), this, SLOT(showCfgDlg()));
    resourceManagerAction = toolMenu->addAction(tr("Resources manager"));

    // add actions to Help menu
    mainMenuBar->addMenu(helpMenu);
    aboutMeAction = helpMenu->addAction(tr("About [Bible Reader]..."));
    connect(aboutMeAction, SIGNAL(triggered()), this, SLOT(showAboutDlg()));

    checkUpdate = helpMenu->addAction(tr("Check Update..."));
    connect(checkUpdate, SIGNAL(triggered(bool)), this, SLOT(checkNewVersion()));
    showHelpContent = helpMenu->addAction(tr("Show Help Content..."));
    connect(showHelpContent, SIGNAL(triggered(bool)), this, SLOT(openHelpPage()));
    donateBibleReader = helpMenu->addAction(tr("Donate [Bible Reader]..."));
    connect(donateBibleReader, SIGNAL(triggered()), this, SLOT(showDonationDlg()));

    // set main menu
    this->setMenuBar(mainMenuBar);

    // add status bar
    statusBar = new QStatusBar();
    this->setStatusBar(statusBar);
    showCurrentVerseInfo();
    connect(bibleReaderCore, SIGNAL(currentVerseChanged(int,int,int)),
            this, SLOT(showCurrentVerseInfo()));
    connect(bibleReaderCore, SIGNAL(currentBibleVersionChanged(QString)),
            this, SLOT(showCurrentVerseInfo()));

    // add dict dockable widget
    dictDockWidget = new QDockWidget(tr("Dictionary Window"),this);
    dictDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    dictDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    bdTabWidget = new BibleDictTabWidget(bibleReaderCore, this);
    dictDockWidget->setWidget(bdTabWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dictDockWidget);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    // auto update
    if (bibleReaderCore->getConfigurator()->getIsAutoUpdate()) {
        checkNewVersion();
    }
}

BibleReaderMainWindow::~BibleReaderMainWindow()
{
    LOG_DEBUG("destructor");
    delete bibleTreeDockWidget;
    delete bibleSearchDockWidget;
    delete dictDockWidget;
    delete commentaryDockWidget;
    delete toolBar;

    if (projectVersesAction) {
        delete projectVersesAction;
    }

    if (brProjectDlg)
        delete brProjectDlg;

    if (brConfigDlg)
        delete brConfigDlg;
    // delete dictWidget;
}

void BibleReaderMainWindow::onChapterVerseChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    int bookNumber = 0;
    int chapter = 0;
    int verse = 1;
    if (current) {
        bookNumber = current->data(1, Qt::DisplayRole).toInt();
        chapter = current->data(2, Qt::DisplayRole).toInt();
        QVariant temp = current->data(3, Qt::DisplayRole);
        if (temp.isValid()) {
            verse = temp.toInt();
        }
    } else {
        bookNumber = previous->data(1, Qt::DisplayRole).toInt();
        chapter = previous->data(2, Qt::DisplayRole).toInt();
        QVariant temp = previous->data(3, Qt::DisplayRole);
        if (temp.isValid()) {
            verse = temp.toInt();
        }
    }
    LOG_INFO() << "getting chapter...book:" << bookNumber
               << "chapter:" << chapter;
    if (chapter != 0)
        bibleReaderCore->setCurrentBCV(bookNumber, chapter, verse);
}

void BibleReaderMainWindow::onChapterChanged(int book, int chapter, int verse)
{
    LOG_INFO() << "getting chapter...book:" << book
               << "chapter:" << chapter
               << "verse:" << verse;

    bibleReaderCore->setCurrentBCV(book, chapter, verse);
}

void BibleReaderMainWindow::quitBibleReader()
{
    QApplication::exit();
}

void BibleReaderMainWindow::projectVerses()
{
    if (!brProjectDlg)
        brProjectDlg = new BibleReaderProjectDialog(bibleReaderCore, "", this);

    brProjectDlg->showFullScreen();
}

void BibleReaderMainWindow::showCfgDlg()
{
    if (!brConfigDlg)
        brConfigDlg = new BibleReaderConfigDlg(bibleReaderCore->getConfigurator(), this);
    brConfigDlg->exec();
}

void BibleReaderMainWindow::checkNewVersion()
{
    // 1. get update information from server
    QString server = "http://biblereader.cn/brupdate.php";

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(server)));
}

void BibleReaderMainWindow::openHelpPage()
{
    QDesktopServices::openUrl(QUrl("http://biblereader.cn/"));
}

void BibleReaderMainWindow::replyFinished(QNetworkReply *reply)
{
    int major, middle, minor;
    QString newVersionUrl;
    QString replyString = reply->readAll();
    if (!replyString.isEmpty()) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(replyString.toUtf8(), &error);

        if (error.error == QJsonParseError::NoError) {
            if (doc.isObject()) {
                // get update version information
                // and assign all information to data
                qDebug() << doc.toJson();

                QJsonObject object = doc.object();

                major = object.value("major").toInt();
                middle = object.value("middle").toInt();
                minor = object.value("minor").toInt();
                newVersionUrl = object.value("newversionurl").toString();

                BibleReaderVersion *version = bibleReaderCore->getVersion();

                int oldVersionNum = 100*version->getMajor()+10*version->getMiddle()+version->getMinor();
                int newVersionNum = 100*major + 10*middle + minor;
                if (newVersionNum > oldVersionNum) {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, tr("Update"),
                                                  tr("New version Bible Reader found, update?"), QMessageBox::Yes | QMessageBox::No);
                    if(reply == QMessageBox::Yes)
                    {
                        QDesktopServices::openUrl(QUrl(newVersionUrl));
                    }
                    else
                    {
                        //do nothing
                    }
                } else {
                    QMessageBox::information(this, tr("Update"), tr("No new version Bible Reader found."), QMessageBox::Ok);
                }
            }
        } else {
            qDebug() << replyString;
        }
    }
    reply->deleteLater();
}

void BibleReaderMainWindow::showAboutDlg()
{
    BibleReaderAboutDlg dlg(this);
    dlg.exec();
}

void BibleReaderMainWindow::showDonationDlg()
{
    BibleReaderDonationDlg dlg(this);
    dlg.show();
    dlg.exec();
}

void BibleReaderMainWindow::copyCurrentVerse()
{
    QClipboard *cb = QApplication::clipboard();
    BibleVerse verse = bibleReaderCore->getVerse(
                bibleReaderCore->getCurrentVersion(),
                bibleReaderCore->getCurrentBookNumber(),
                bibleReaderCore->getCurrentChapterNumber(),
                bibleReaderCore->getCurrentVerseNumber());
    cb->setText(verse.text(), QClipboard::Clipboard);
}

void BibleReaderMainWindow::showCurrentVerseInfo()
{
    BibleInfo info = bibleReaderCore->getCurrentBibleInfo();
    BibleVerse verse = bibleReaderCore->getVerse(bibleReaderCore->getCurrentBookNumber(),
                                                 bibleReaderCore->getCurrentChapterNumber(),
                                                 bibleReaderCore->getCurrentVerseNumber());

    statusBar->showMessage(info.getFullname().append(" ").append(verse.getVerseHeader()));
}

void BibleReaderMainWindow::navToNextChapter()
{
    bibleReaderCore->navNextChapter();
}

void BibleReaderMainWindow::navToPrevChapter()
{
    bibleReaderCore->navPrevChapter();
}
