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

// for layout setting
#include <QXmlStreamReader>
#include "biblereaderlayout.h"

// for open system default browser
#include <QDesktopServices>

#ifdef Q_OS_MAC
#include <QtMacExtras/qmacfunctions.h>
#endif

#include "bibletreewidget.h"
#include "bibledictionarywidget.h"
#include "biblereaderaboutdlg.h"
#include "biblereaderlayout.h"

// for logging
#include "Logger.h"
#include "ConsoleAppender.h"

BibleReaderMainWindow::BibleReaderMainWindow(BibleReaderCore *brc, QWidget *parent)
    : QMainWindow(parent)
{
    LOG_DEBUG("constructor");
    setObjectName("mainwindow");
    brConfigDlg = NULL;
    brProjectDlg = NULL;

    bibleReaderCore = brc;

    //Set main window
    this->setMinimumSize(860, 640);
    this->setWindowState(Qt::WindowMaximized);
    this->setWindowTitle(tr("Bible Reader"));

    // get all layouts
    buildLayoutActions();
    // create all widgets and do layout
    createWidgets();
#ifdef Q_OS_MAC
    this->setUnifiedTitleAndToolBarOnMac(true);
#endif

    updater = QSimpleUpdater::getInstance();

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

void BibleReaderMainWindow::onChapterVerseChanged(QTreeWidgetItem *item, int column)
{
    int bookNumber = 0;
    int chapter = 0;
    int verse = 1;
    if (item) {
        bookNumber = item->data(1, Qt::DisplayRole).toInt();
        chapter = item->data(2, Qt::DisplayRole).toInt();
        QVariant temp = item->data(3, Qt::DisplayRole);
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

void BibleReaderMainWindow::navToNextBook()
{
    bibleReaderCore->navNextBook();
}

void BibleReaderMainWindow::navToPrevBook()
{
    bibleReaderCore->navPrevBook();
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
    QString appcast = "http://biblereader.cn/brupdate.xml";

    updater->setModuleName(appcast, "biblereader");
    updater->setModuleVersion(appcast, qApp->applicationVersion());
    updater->setNotifyOnUpdate(appcast, true);
    updater->setDownloaderEnabled(appcast, true);
    updater->setNotifyOnFinish(appcast, true);
    updater->checkForUpdates(appcast);
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

    statusBar->showMessage(QString("[") + info.getFullname().append("] ").append(verse.getVerseHeader()));
}

void BibleReaderMainWindow::setHistoryNavActionsEnabled(int which)
{
    switch (which) {
    case 0:
        navToHistoryBack->setDisabled(true);
        navToHistoryForword->setEnabled(true);
        break;

    case -1:
        navToHistoryBack->setDisabled(false);
        navToHistoryForword->setEnabled(false);
        break;
    default:
        navToHistoryBack->setEnabled(true);
        navToHistoryForword->setEnabled(true);
        break;
    }
}

void BibleReaderMainWindow::windowMenuTrigger()
{
    windowMenu->clear();
    QList<QDockWidget *> allDockWidgets = findChildren<QDockWidget*>();
    //LOG_INFO() << allDockWidgets.size();
    QList<QAction *> windowActionList;
    for (int i = 0; i < allDockWidgets.size(); i++) {
        QAction *tmp = new QAction(allDockWidgets[i]->windowTitle(), windowMenu);
        Qt::Key win = Qt::Key_0;
        switch (i) {
        case 0:
            win = Qt::Key_0;
            break;
        case 1:
            win = Qt::Key_1;
            break;
        case 2:
            win = Qt::Key_2;
            break;
        case 3:
            win = Qt::Key_3;
            break;
        case 4:
            win = Qt::Key_4;
            break;
        default:
            break;
        }
        tmp->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + win));
        tmp->setCheckable(true);
        if (allDockWidgets[i]->isHidden()) {
            tmp->setChecked(false);
        } else {
            tmp->setChecked(true);
        }
        connect(tmp, SIGNAL(toggled(bool)), this, SLOT(toggleDockWidget(bool)));
        windowActionList.append(tmp);

    }
    windowMenu->addActions(windowActionList);

    // loading layout
    windowMenu->addSeparator();

    QMenu *layoutMenu = new QMenu(tr("Layout"),this);

    // add layouts menu item
    layoutMenu->addActions(layoutActions);

    // add "Layout" to Window menu
    windowMenu->addMenu(layoutMenu);
}

void BibleReaderMainWindow::toggleDockWidget(bool toggled)
{
    QAction *act = qobject_cast<QAction*>(sender());
    QList<QDockWidget *> allDockWidgets = findChildren<QDockWidget*>();

    for (int i = 0; i < allDockWidgets.size(); i++) {
        if (act->text() == allDockWidgets[i]->windowTitle()) {
            if (toggled) {
                allDockWidgets[i]->setVisible(true);
            } else {
                allDockWidgets[i]->setVisible(false);
            }
            break;
        }
    }
}

void BibleReaderMainWindow::doLayout()
{
    QAction *act = qobject_cast<QAction *>(sender());

    for (int i=0; i < layoutActions.size(); i++) {
        if (act->text() == layoutActions[i]->text()) {
            BibleReaderLayout layout = layouts[i];

            // do layout work
            for (int j = 0; j < layout.layouts.size(); j++) {
                WidgetLayout wl = layout.layouts[j];

                if (wl.widgetName == "bibletree") {
                    removeDockWidget(bibleTreeDockWidget);
                    if (wl.position == "south") {
                        addDockWidget(Qt::BottomDockWidgetArea, bibleTreeDockWidget);
                    } else if (wl.position == "north") {
                        addDockWidget(Qt::TopDockWidgetArea, bibleTreeDockWidget);
                    } else if (wl.position == "east") {
                        addDockWidget(Qt::RightDockWidgetArea, bibleTreeDockWidget);
                    } else if (wl.position == "west") {
                        addDockWidget(Qt::LeftDockWidgetArea, bibleTreeDockWidget);
                    }

                    if (wl.show) {
                        bibleTreeDockWidget->show();
                    } else {
                        bibleTreeDockWidget->hide();
                    }
                } else if (wl.widgetName == "biblesearch") {
                    removeDockWidget(bibleSearchDockWidget);
                    if (wl.position == "south") {
                        addDockWidget(Qt::BottomDockWidgetArea, bibleSearchDockWidget);
                    } else if (wl.position == "north") {
                        addDockWidget(Qt::TopDockWidgetArea, bibleSearchDockWidget);
                    } else if (wl.position == "east") {
                        addDockWidget(Qt::RightDockWidgetArea, bibleSearchDockWidget);
                    } else if (wl.position == "west") {
                        addDockWidget(Qt::LeftDockWidgetArea, bibleSearchDockWidget);
                    }

                    if (wl.show) {
                        bibleSearchDockWidget->show();
                    } else {
                        bibleSearchDockWidget->hide();
                    }
                } else if (wl.widgetName == "bibledict") {
                    removeDockWidget(dictDockWidget);
                    if (wl.position == "south") {
                        addDockWidget(Qt::BottomDockWidgetArea, dictDockWidget);
                    } else if (wl.position == "north") {
                        addDockWidget(Qt::TopDockWidgetArea, dictDockWidget);
                    } else if (wl.position == "east") {
                        addDockWidget(Qt::RightDockWidgetArea, dictDockWidget);
                    } else if (wl.position == "west") {
                        addDockWidget(Qt::LeftDockWidgetArea, dictDockWidget);
                    }

                    if (wl.show) {
                        dictDockWidget->show();
                    } else {
                        dictDockWidget->hide();
                    }
                } else if (wl.widgetName == "biblecommentary") {
                    removeDockWidget(commentaryDockWidget);
                    if (wl.position == "south") {
                        addDockWidget(Qt::BottomDockWidgetArea, commentaryDockWidget);
                    } else if (wl.position == "north") {
                        addDockWidget(Qt::TopDockWidgetArea, commentaryDockWidget);
                    } else if (wl.position == "east") {
                        addDockWidget(Qt::RightDockWidgetArea, commentaryDockWidget);
                    } else if (wl.position == "west") {
                        addDockWidget(Qt::LeftDockWidgetArea, commentaryDockWidget);
                    }

                    if (wl.show) {
                        commentaryDockWidget->show();
                    } else {
                        commentaryDockWidget->hide();
                    }
                }
            }

            break;
        }
    }
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
    toolBar->setMovable(false);
    configAction = toolBar->addAction(QIcon(QString(":/img/assets/images/wrench.png")),tr("Configure"));
    configAction->setMenuRole(QAction::PreferencesRole);
    connect(configAction, SIGNAL(triggered(bool)), this, SLOT(showCfgDlg()));
    resourceManagerAction = toolBar->addAction(QIcon(QString(":/img/assets/images/package.png")),tr("Resources manager"));
    //resourceManagerAction->setEnabled(false);
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

    navToPrevBookAction = new QAction(QIcon(QString(":/img/assets/images/book_previous.png")), tr("previous book"),this);
    navToPrevBookAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    txtBrowserToolBar->addAction(navToPrevBookAction);
    connect(navToPrevBookAction, SIGNAL(triggered(bool)), this, SLOT(navToPrevBook()));

    navToNextBookAction = new QAction(QIcon(QString(":/img/assets/images/book_next.png")),tr("next book"),this);
    navToNextBookAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    txtBrowserToolBar->addAction(navToNextBookAction);
    connect(navToNextBookAction, SIGNAL(triggered(bool)), this, SLOT(navToNextBook()));

    navToPrevChapterAction = new QAction(QIcon(QString(":/img/assets/images/arrow_up.png")), tr("previous chapter"),this);
    navToPrevChapterAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    txtBrowserToolBar->addAction(navToPrevChapterAction);
    connect(navToPrevChapterAction, SIGNAL(triggered()), this, SLOT(navToPrevChapter()));

    navToNextChapterAction = new QAction(QIcon(QString(":/img/assets/images/arrow_down.png")),tr("next chapter"),this);
    navToNextChapterAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    txtBrowserToolBar->addAction(navToNextChapterAction);
    connect(navToNextChapterAction, SIGNAL(triggered()), this, SLOT(navToNextChapter()));

    navToHistoryBack = new QAction(QIcon(QString(":/img/assets/images/arrow_left.png")), tr("Back"), this);
    navToHistoryBack->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    txtBrowserToolBar->addAction(navToHistoryBack);
    connect(navToHistoryBack, SIGNAL(triggered(bool)), this, SLOT(navToBackHistory()));
    navToHistoryBack->setDisabled(true);

    navToHistoryForword = new QAction(QIcon(QString(":/img/assets/images/arrow_right.png")), tr("Forword"), this);
    navToHistoryForword->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    txtBrowserToolBar->addAction(navToHistoryForword);
    navToHistoryForword->setDisabled(true);
    connect(navToHistoryForword, SIGNAL(triggered(bool)), this, SLOT(navToForwordHistory()));

    connect(bibleReaderCore,
            SIGNAL(navToHistoryItem(int)), this, SLOT(setHistoryNavActionsEnabled(int)));

    btZoomInAction = new QAction(QIcon(QString(":/img/assets/images/zoom_in.png")), tr("Zoom in"), this);
    btZoomInAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
    txtBrowserToolBar->addAction(btZoomInAction);
    connect(btZoomInAction, SIGNAL(triggered(bool)), this, SLOT(btZoomIn()));

    btZoomOutAction = new QAction(QIcon(QString(":/img/assets/images/zoom_out.png")), tr("Zoom out"), this);
    btZoomOutAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
    txtBrowserToolBar->addAction(btZoomOutAction);
    connect(btZoomOutAction, SIGNAL(triggered(bool)), this, SLOT(btZoomOut()));

    goVerseAction = new QAction(QIcon(QString(":/img/assets/images/page_go.png")), tr("Jump directly"), this);
    goVerseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_J));
    QMenu *m = buildBibleTreeMenu();
    goVerseAction->setMenu(m);
    txtBrowserToolBar->addAction(goVerseAction);
    txtBrowserToolBar->addSeparator();

    copyCurVerseAction = new QAction(QIcon(QString(":/img/assets/images/page_copy.png")), tr("Copy current verse"), this);
    copyCurVerseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_C));
    txtBrowserToolBar->addAction(copyCurVerseAction);
    connect(copyCurVerseAction, SIGNAL(triggered(bool)), this, SLOT(copyCurrentVerse()));

    compareVerseAction = new QAction(QIcon(QString(":/img/assets/images/compare.png")), tr("Compare current verse"), this);
    compareVerseAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_P));
    txtBrowserToolBar->addAction(compareVerseAction);
    connect(compareVerseAction, SIGNAL(triggered(bool)), bibleReaderCore, SLOT(fireCmpCurVerse()));

    txtBrowserToolBar->addSeparator();
    printBTAction = new QAction(QIcon(QString(":/img/assets/images/printer.png")), tr("Print current chapter"), this);
    printBTAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_P));
    txtBrowserToolBar->addAction(printBTAction);
    connect(printBTAction, SIGNAL(triggered(bool)), this, SLOT(btPrint()));

    showBibleInfoAction = new QAction(QIcon(QString(":/img/assets/images/information.png")), tr("Show current bible information"), this);
    showBibleInfoAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_I));
    txtBrowserToolBar->addAction(showBibleInfoAction);
    connect(showBibleInfoAction, SIGNAL(triggered(bool)), this, SLOT(showBibleInfo()));
    /*
    QLineEdit *verseJumper = new QLineEdit(this);
    verseJumper->setMaximumWidth(40);
    txtBrowserToolBar->addWidget(verseJumper);
    */

    btTabWidget = new BibleTextTabWidget(bibleReaderCore, this);
    // test
    // btTabWidget->setCornerWidget(new QLineEdit(this));
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setMargin(0);
    centralLayout->setSpacing(0);
    centralLayout->addWidget(txtBrowserToolBar);
    centralLayout->addWidget(btTabWidget);
    // TOOLBAR bible select
    txtBrowserToolBar->addSeparator();
    selectBibleList = new QComboBox(this);
    for (int i = 0; i < btTabWidget->count(); i++) {
        selectBibleList->addItem(btTabWidget->tabText(i));
    }
    selectBibleList->setCurrentText(btTabWidget->tabText(btTabWidget->currentIndex()));
    connect(selectBibleList, SIGNAL(currentIndexChanged(int)), btTabWidget, SLOT(setCurrentIndex(int)));
    connect(btTabWidget, SIGNAL(currentChanged(int)), selectBibleList, SLOT(setCurrentIndex(int)));
    txtBrowserToolBar->addWidget(selectBibleList);

    central->setLayout(centralLayout);
    this->setCentralWidget(central);
}

void BibleReaderMainWindow::createBibleTreeDockWidget()
{
    bibleTreeDockWidget = new QDockWidget(tr("Bible Tree"),this);
    bibleTreeDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                     QDockWidget::DockWidgetClosable|
                                     QDockWidget::DockWidgetFloatable);
    bibleTreeDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea
                                         | Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea , bibleTreeDockWidget);
    // bible tree
    bibleTreeWidget = new BibleTreeWidget(bibleReaderCore, this);
    bibleTreeDockWidget->setWidget(bibleTreeWidget);
    connect(bibleTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
                   this, SLOT(onChapterVerseChanged(QTreeWidgetItem*,int)), Qt::DirectConnection);
    connect(bibleReaderCore, SIGNAL(currentBibleVersionChanged(QString)), bibleTreeWidget,
            SLOT(onBibleVersionChanged(QString)));


}

void BibleReaderMainWindow::createBSDockWidget()
{
    bibleSearchDockWidget = new QDockWidget(tr("Bible Search"),this);
    bibleSearchDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                       QDockWidget::DockWidgetClosable|
                                       QDockWidget::DockWidgetFloatable);
    bibleSearchDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea
                                           | Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);
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
    commentaryDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                      QDockWidget::DockWidgetClosable|
                                      QDockWidget::DockWidgetFloatable);
    commentaryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea
                                          | Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, commentaryDockWidget);
    bcTabWidget = new BibleCommentaryTabWidget(bibleReaderCore, this);
    commentaryDockWidget->setWidget(bcTabWidget);
}

void BibleReaderMainWindow::createDictDockWidget()
{
    dictDockWidget = new QDockWidget(tr("Dictionary Window"),this);
    dictDockWidget->setFeatures(QDockWidget::DockWidgetMovable |
                                QDockWidget::DockWidgetClosable|
                                QDockWidget::DockWidgetFloatable);
    dictDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea
                                    | Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);
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
    windowMenu = new QMenu(tr("Window(&W)"), mainMenuBar);
    helpMenu = new QMenu(tr("Help(&H)"), mainMenuBar);

    // add actions to File menu
    mainMenuBar->addMenu(fileMenu);
    projectVersesAction = fileMenu->addAction(QIcon(QString(":/img/assets/images/television.png")),tr("Project Verses"));
    projectVersesAction->setEnabled(false);
    connect(projectVersesAction, SIGNAL(triggered()), this, SLOT(projectVerses()));
    exitAppAction = fileMenu->addAction(QIcon(QString(":/img/assets/images/cancel.png")), tr("Exit"));
    exitAppAction->setMenuRole(QAction::QuitRole);
    exitAppAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_X));
    connect(exitAppAction, SIGNAL(triggered()), this, SLOT(quitBibleReader()));

    // add actions to Edit menu
    mainMenuBar->addMenu(editMenu);
    editMenu->addAction(copyCurVerseAction);

    // add action to view menu
    mainMenuBar->addMenu(viewMenu);
    viewMenu->addAction(navToHistoryBack);
    viewMenu->addAction(navToHistoryForword);
    viewMenu->addAction(navToPrevChapterAction);
    viewMenu->addAction(navToNextChapterAction);
    viewMenu->addAction(navToPrevBookAction);
    viewMenu->addAction(navToNextBookAction);
    viewMenu->addAction(btZoomInAction);
    viewMenu->addAction(btZoomOutAction);
    viewMenu->addAction(compareVerseAction);
    viewMenu->addAction(goVerseAction);


    // add actions to Tool menu
    mainMenuBar->addMenu(toolMenu);
    toolMenu->addAction(configAction);
    toolMenu->addAction(resourceManagerAction);

    // add actions to Window Menu
    mainMenuBar->addMenu(windowMenu);
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(windowMenuTrigger()));

    // add actions to Help menu
    mainMenuBar->addMenu(helpMenu);
    aboutMeAction = helpMenu->addAction(QIcon(QString(":/img/assets/images/32X32.ico")), tr("About [Bible Reader]..."));
    aboutMeAction->setMenuRole(QAction::AboutRole);
    connect(aboutMeAction, SIGNAL(triggered()), this, SLOT(showAboutDlg()));

    checkUpdate = helpMenu->addAction(QIcon(QString(":/img/assets/images/arrow_refresh.png")),tr("Check Update..."));
    connect(checkUpdate, SIGNAL(triggered(bool)), this, SLOT(checkNewVersion()));
    showHelpContent = helpMenu->addAction(QIcon(QString(":/img/assets/images/help.png")),tr("Show Help Content..."));
    connect(showHelpContent, SIGNAL(triggered(bool)), this, SLOT(openHelpPage()));

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

QMenu *BibleReaderMainWindow::buildBibleTreeMenu()
{
    LOG_DEBUG("Building Bible menu data...");

    QMenu *jumpGoMenu = new QMenu(this);

    // get all bible books
    QList<BibleBook> books = bibleReaderCore->getAllBooks();
    for(int i=0;i<books.count();i++)
    {
        BibleBook book = books.value(i);
        QMenu *bookMenu = new QMenu(book.getLongName(), jumpGoMenu);
        bookMenu->setProperty("bid", book.getBookNumber());
        bookMenu->setIcon(QIcon(QString(":/img/assets/images/book.png")));
        jumpGoMenu->addMenu(bookMenu);
        int bookNumber = book.getBookNumber();
        QFont f = bookMenu->menuAction()->font();
        if (bookNumber % 2 == 0) {
            f.setBold(true);
            bookMenu->menuAction()->setFont(f);
        } else {
            // do nothing, normal
        }
        // get all chapters for one book
        QList<int> chapters = bibleReaderCore->getChaptersCountOfOneBook(book.getBookNumber());
        for(int j=0;j<chapters.count();j++)
        {
            QMenu *chapterMenu = new QMenu(QString::number(j+1), bookMenu);
            bookMenu->addMenu(chapterMenu);
            int versesCount = chapters.value(j);
            // add verses to chapter
            for (int k=0; k<versesCount;k++)
            {
                QAction *verseAction = new QAction(QString::number(k+1), chapterMenu);
                QList<QVariant> d;
                d.push_back(book.getBookNumber());
                d.push_back(j+1);
                d.push_back(k+1);
                verseAction->setData(d);
                connect(verseAction, SIGNAL(triggered(bool)), this, SLOT(btGoVerse()));
                chapterMenu->addAction(verseAction);
            }
        }

    }
    //jumpGoMenu->setStyleSheet(qss);
    LOG_DEBUG("Built Bible menu data");
    return jumpGoMenu;

}

void BibleReaderMainWindow::buildLayoutActions()
{
    QString layoutPathBase = bibleReaderCore->getConfigurator()->getLayoutPath();
    QDir layoutPath = QDir(layoutPathBase);
    if (!layoutPath.exists()) {
        return;
    } else {
        layoutPath.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        QFileInfoList layoutFiles = layoutPath.entryInfoList();
        QString layout, layoutFilePath;

        for (int i = 0; i < layoutFiles.count(); i++) {
            layout = layoutFiles[i].fileName();

            layoutFilePath = layoutPathBase + layout;

            // open xml file
            QFile f(layoutFilePath);
            if (!f.open(QIODevice::ReadOnly))
                continue;

            QXmlStreamReader xmlReader(&f);
            // get layout information

            QString layoutName;
            QString layoutShortName;
            BibleReaderLayout layout;
            while(!xmlReader.atEnd() && !xmlReader.hasError())
            {
                xmlReader.readNext();
                if(xmlReader.isStartElement()) {
                    if(xmlReader.name()=="name") {
                        layoutName = xmlReader.readElementText();
                        LOG_INFO()<<"layout name: " << layoutName;
                        QAction *tmp = new QAction(layoutName, this);
                        layoutActions.push_back(tmp);
                        layout.layoutName = layoutName;
                    } else if (xmlReader.name() == "shortname") {
                        layoutShortName = xmlReader.readElementText();
                        LOG_INFO()<<"layout short name: " << layoutShortName;
                        layout.layoutShortName = layoutShortName;
                    } else if (xmlReader.name() == "bibletree") {
                        LOG_INFO() << "bibletree: " << xmlReader.attributes().value("show");
                        WidgetLayout tmp_layout;
                        tmp_layout.widgetName = "bibletree";
                        tmp_layout.show = xmlReader.attributes().value("show") == "true" ? true : false;
                        tmp_layout.position = xmlReader.attributes().value("area").toString();
                        layout.layouts.push_back(tmp_layout);
                    } else if (xmlReader.name() == "biblesearch") {
                        LOG_INFO() << "biblesearch: " << xmlReader.attributes().value("show");
                        WidgetLayout tmp_layout;
                        tmp_layout.widgetName = "biblesearch";
                        tmp_layout.show = xmlReader.attributes().value("show") == "true" ? true : false;
                        tmp_layout.position = xmlReader.attributes().value("area").toString();
                        layout.layouts.push_back(tmp_layout);
                    } else if (xmlReader.name() == "bibledict") {
                        LOG_INFO() << "bibledict: " << xmlReader.attributes().value("show");
                        WidgetLayout tmp_layout;
                        tmp_layout.widgetName = "bibledict";
                        tmp_layout.show = xmlReader.attributes().value("show") == "true" ? true : false;
                        tmp_layout.position = xmlReader.attributes().value("area").toString();
                        layout.layouts.push_back(tmp_layout);
                    } else if (xmlReader.name() == "biblecommentary") {
                        LOG_INFO() << "biblecommentary: " << xmlReader.attributes().value("show");
                        WidgetLayout tmp_layout;
                        tmp_layout.widgetName = "biblecommentary";
                        tmp_layout.show = xmlReader.attributes().value("show") == "true" ? true : false;
                        tmp_layout.position = xmlReader.attributes().value("area").toString();
                        layout.layouts.push_back(tmp_layout);
                    }
                }

            }

            layouts.push_back(layout);
            // create action and connect signal
            for (int i = 0; i < layoutActions.size(); i++) {
                connect(layoutActions[i], SIGNAL(triggered(bool)), this, SLOT(doLayout()));
            }
            // add action
        }
    }

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

void BibleReaderMainWindow::btGoVerse()
{
    QAction *act = qobject_cast<QAction *>(sender());

    QList<QVariant> params = act->data().toList();

    bibleReaderCore->setCurrentBCV(params[0].toInt(), params[1].toInt(), params[2].toInt());
}

void BibleReaderMainWindow::showBibleInfo()
{
    BibleInfo info = bibleReaderCore->getCurrentBibleInfo();

    QMessageBox::information(this, tr("Bible Information"), info.getDescription() + "\n" + info.getCopyright(), QMessageBox::Ok);
}
