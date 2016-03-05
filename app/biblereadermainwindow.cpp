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

    // create all widgets and do layout
    createWidgets();

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

void BibleReaderMainWindow::showResMgrDlg()
{
    BibleReaderResourceManagerDlg dlg(bibleReaderCore, this);

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

void BibleReaderMainWindow::createWidgets()
{
    // create global toolbar
    createGlobalToolbar();

    // set bible text tab widget
    createCentralWidget();

    // set search dockable widget
    createBSDockWidget();

    // Set dockable widgets
    // bible tree dock widget
    createBibleTreeDockWidget();

    // commentary dock widget
    createBCDockWidget();

    // add dict dockable widget
    createDictDockWidget();

    // create menus
    createTopMenus();

    // status bar
    createStatusBar();
}

void BibleReaderMainWindow::createGlobalToolbar()
{
    // set global toolbar
    toolBar = new QToolBar(tr("Bible Navigator"), this);
    configAction = toolBar->addAction(QIcon(QString(":/img/assets/images/wrench.png")),tr("Configure"));
    connect(configAction, SIGNAL(triggered(bool)), this, SLOT(showCfgDlg()));
    resourceManagerAction = toolBar->addAction(QIcon(QString(":/img/assets/images/package.png")),tr("Resources manager"));
    connect(resourceManagerAction, SIGNAL(triggered(bool)), this, SLOT(showResMgrDlg()));
    toolBar->show();

    this->addToolBar(toolBar);
}

void BibleReaderMainWindow::createCentralWidget()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(central);
    QToolBar *txtBrowserToolBar = new QToolBar(central);
    txtBrowserToolBar->setOrientation(Qt::Horizontal);
    navToPrevChapterAction = new QAction(QIcon(QString(":/img/assets/images/book_previous.png")), tr("previous chapter"),this);
    navToPrevChapterAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    txtBrowserToolBar->addAction(navToPrevChapterAction);
    connect(navToPrevChapterAction, SIGNAL(triggered()), this, SLOT(navToPrevChapter()));

    navToNextChapterAction = new QAction(QIcon(QString(":/img/assets/images/book_next.png")),tr("next chapter"),this);
    navToNextChapterAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    txtBrowserToolBar->addAction(navToNextChapterAction);
    connect(navToNextChapterAction, SIGNAL(triggered()), this, SLOT(navToNextChapter()));

    navToHistoryBack = new QAction(QIcon(QString(":/img/assets/images/arrow_left.png")), tr("Back"), this);
    navToHistoryBack->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    txtBrowserToolBar->addAction(navToHistoryBack);
    connect(navToHistoryBack, SIGNAL(triggered(bool)), this, SLOT(navToBackHistory()));

    navToHistoryForword = new QAction(QIcon(QString(":/img/assets/images/arrow_right.png")), tr("Forword"), this);
    navToHistoryForword->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    txtBrowserToolBar->addAction(navToHistoryForword);
    connect(navToHistoryForword, SIGNAL(triggered(bool)), this, SLOT(navToForwordHistory()));

    btZoomInAction = new QAction(QIcon(QString(":/img/assets/images/zoom_in.png")), tr("Zoom in"), this);
    btZoomInAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
    txtBrowserToolBar->addAction(btZoomInAction);
    connect(btZoomInAction, SIGNAL(triggered(bool)), this, SLOT(btZoomIn()));

    btZoomOutAction = new QAction(QIcon(QString(":/img/assets/images/zoom_out.png")), tr("Zoom out"), this);
    btZoomOutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
    txtBrowserToolBar->addAction(btZoomOutAction);
    connect(btZoomOutAction, SIGNAL(triggered(bool)), this, SLOT(btZoomOut()));

    txtBrowserToolBar->addSeparator();

    copyCurVerseAction = new QAction(QIcon(QString(":/img/assets/images/page_copy.png")), tr("Copy current verse"), this);
    copyCurVerseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_C));
    txtBrowserToolBar->addAction(copyCurVerseAction);
    connect(copyCurVerseAction, SIGNAL(triggered(bool)), this, SLOT(copyCurrentVerse()));

    txtBrowserToolBar->addSeparator();
    goVerseAction = new QAction(QIcon(QString(":/img/assets/images/page_go.png")), tr("Jump directly"), this);
    goVerseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_J));
    txtBrowserToolBar->addAction(goVerseAction);

    txtBrowserToolBar->addSeparator();
    printBTAction = new QAction(QIcon(QString(":/img/assets/images/printer.png")), tr("Print current chapter"), this);
    printBTAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_P));
    txtBrowserToolBar->addAction(printBTAction);
    connect(printBTAction, SIGNAL(triggered(bool)), this, SLOT(btPrint()));

    /*
    QLineEdit *verseJumper = new QLineEdit(this);
    verseJumper->setMaximumWidth(40);
    txtBrowserToolBar->addWidget(verseJumper);
    */

    btTabWidget = new BibleTextTabWidget(bibleReaderCore, this);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setMargin(0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(txtBrowserToolBar);
    centralLayout->addWidget(btTabWidget);

    central->setLayout(centralLayout);
    this->setCentralWidget(central);
}

void BibleReaderMainWindow::createBibleTreeDockWidget()
{
    bibleTreeDockWidget = new QDockWidget(tr("Bible Tree"),this);
    bibleTreeDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    bibleTreeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, bibleTreeDockWidget);
    // bible tree
    bibleTreeWidget = new BibleTreeWidget(bibleReaderCore, this);
    bibleTreeDockWidget->setWidget(bibleTreeWidget);
    connect(bibleTreeWidget,
            SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                   this, SLOT(onChapterVerseChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(bibleReaderCore, SIGNAL(currentBibleVersionChanged(QString)), bibleTreeWidget,
            SLOT(onBibleVersionChanged(QString)));


}

void BibleReaderMainWindow::createBSDockWidget()
{
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
}

void BibleReaderMainWindow::createBCDockWidget()
{
    commentaryDockWidget = new QDockWidget(tr("Commentary"), this);
    commentaryDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    commentaryDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, commentaryDockWidget);
    bcTabWidget = new BibleCommentaryTabWidget(bibleReaderCore, this);
    commentaryDockWidget->setWidget(bcTabWidget);
}

void BibleReaderMainWindow::createDictDockWidget()
{
    dictDockWidget = new QDockWidget(tr("Dictionary Window"),this);
    dictDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
    dictDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    bdTabWidget = new BibleDictTabWidget(bibleReaderCore, this);
    dictDockWidget->setWidget(bdTabWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dictDockWidget);
    setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
}

void BibleReaderMainWindow::createTopMenus()
{
    mainMenuBar = new QMenuBar(this);
    fileMenu = new QMenu(tr("File(&F)"), mainMenuBar);
    editMenu = new QMenu(tr("Edit(&E)"), mainMenuBar);
    viewMenu = new QMenu(tr("View(&V)"), mainMenuBar);
    toolMenu = new QMenu(tr("Tools(&T)"), mainMenuBar);
    helpMenu = new QMenu(tr("Help(&H)"), mainMenuBar);

    // add actions to File menu
    mainMenuBar->addMenu(fileMenu);
    projectVersesAction = fileMenu->addAction(QIcon(QString(":/img/assets/images/television.png")),tr("Project Verses"));
    connect(projectVersesAction, SIGNAL(triggered()), this, SLOT(projectVerses()));
    exitAppAction = fileMenu->addAction(QIcon(QString(":/img/assets/images/cancel.png")), tr("Exit"));
    exitAppAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_X));
    connect(exitAppAction, SIGNAL(triggered()), this, SLOT(quitBibleReader()));

    // add actions to Edit menu
    mainMenuBar->addMenu(editMenu);
    editMenu->addAction(copyCurVerseAction);

    // add action to view menu
    mainMenuBar->addMenu(viewMenu);
    viewMenu->addAction(btZoomInAction);
    viewMenu->addAction(btZoomOutAction);

    // add actions to Tool menu
    mainMenuBar->addMenu(toolMenu);
    toolMenu->addAction(configAction);
    toolMenu->addAction(resourceManagerAction);

    // add actions to Help menu
    mainMenuBar->addMenu(helpMenu);
    aboutMeAction = helpMenu->addAction(QIcon(QString(":/img/assets/images/32X32.ico")), tr("About [Bible Reader]..."));
    connect(aboutMeAction, SIGNAL(triggered()), this, SLOT(showAboutDlg()));

    checkUpdate = helpMenu->addAction(QIcon(QString(":/img/assets/images/arrow_refresh.png")),tr("Check Update..."));
    connect(checkUpdate, SIGNAL(triggered(bool)), this, SLOT(checkNewVersion()));
    showHelpContent = helpMenu->addAction(QIcon(QString(":/img/assets/images/help.png")),tr("Show Help Content..."));
    connect(showHelpContent, SIGNAL(triggered(bool)), this, SLOT(openHelpPage()));
    donateBibleReader = helpMenu->addAction(QIcon(QString(":/img/assets/images/emoticon_smile.png")),tr("Donate [Bible Reader]..."));
    connect(donateBibleReader, SIGNAL(triggered()), this, SLOT(showDonationDlg()));

    // set main menu
    this->setMenuBar(mainMenuBar);
}

void BibleReaderMainWindow::createStatusBar()
{
    statusBar = new QStatusBar();
    this->setStatusBar(statusBar);
    showCurrentVerseInfo();
    connect(bibleReaderCore, SIGNAL(currentVerseChanged(int,int,int)),
            this, SLOT(showCurrentVerseInfo()));
    connect(bibleReaderCore, SIGNAL(currentBibleVersionChanged(QString)),
            this, SLOT(showCurrentVerseInfo()));
}

void BibleReaderMainWindow::navToNextChapter()
{
    bibleReaderCore->navNextChapter();
}

void BibleReaderMainWindow::navToPrevChapter()
{
    bibleReaderCore->navPrevChapter();
}

void BibleReaderMainWindow::navToBackHistory()
{
    bibleReaderCore->navBackHistory();
}

void BibleReaderMainWindow::navToForwordHistory()
{
    bibleReaderCore->navForwordHistory();
}

void BibleReaderMainWindow::btZoomIn()
{
    double size = bibleReaderCore->getConfigurator()->getBibleTextFontSize();
    if (size <= 46)
        bibleReaderCore->getConfigurator()->setBibleTextFontSize(size + 2.0);
}

void BibleReaderMainWindow::btZoomOut()
{
    double size = bibleReaderCore->getConfigurator()->getBibleTextFontSize();
    if (size >= 8)
        bibleReaderCore->getConfigurator()->setBibleTextFontSize(size - 2.0);
}

void BibleReaderMainWindow::btPrint()
{
    BibleTextBrowser *btBrowser = qobject_cast<BibleTextBrowser *>(btTabWidget->currentWidget());
    btBrowser->printBibleText();
}
