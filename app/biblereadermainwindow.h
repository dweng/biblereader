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
#ifndef BIBLEREADERMAINWINDOW_H
#define BIBLEREADERMAINWINDOW_H

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMainWindow>
#include <QListWidgetItem>
#include "bibletexttabwidget.h"
#include "bibledicttabwidget.h"
#include "biblereadercore.h"
#include "bibletextbrowser.h"
#include "bibletreewidget.h"
#include "biblesearchwidget.h"
#include "bibledictionarywidget.h"
#include "biblecommentarytabwidget.h"
#include "biblereaderprojectdialog.h"
#include "biblereaderconfigdlg.h"
#include "biblereaderresourcemanagerdlg.h"

QT_BEGIN_NAMESPACE

class QDockWidget;
class QWidget;
class QGridLayout;
class QToolBar;
class QAction;
class QModelIndex;
class QLineEdit;
class QPushButton;
class QStatusBar;

QT_END_NAMESPACE

/**
 * @brief The BibleReaderMainWindow class
 * This class represents the main window of BibleReader
 *
 * @version 1.0
 */
class BibleReaderMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent parent widget, default to NULL
     */
    BibleReaderMainWindow(BibleReaderCore *brc, QWidget *parent = 0);
    /**
     * @brief Destructor.
     */
    ~BibleReaderMainWindow();

private:
    /**
     * @brief Dock widget contains bible tree widget.
     */
    QDockWidget *bibleTreeDockWidget;

    /**
     * @brief Dock widget contains bible search widget.
     */
    QDockWidget *bibleSearchDockWidget;

    /**
     * @brief Dock widget contains dictionary widget.
     */
    QDockWidget *dictDockWidget;

    /**
     * @brief commentary Dock Widget contains commentary widget.
     */
    QDockWidget *commentaryDockWidget;

    /**
     * @brief bibleReaderCore instance.
     */
    BibleReaderCore *bibleReaderCore;

    /**
     * @brief bibleTreeWidget instance
     */
    BibleTreeWidget *bibleTreeWidget;

    /**
     * @brief bibleSearchWidget instance
     */
    BibleSearchWidget *bibleSearchWidget;

    /**
     * @brief bible commentary widget instance
     */
    BibleCommentaryTabWidget *bcTabWidget;

    /**
     * @brief Tool bar for main application
     */
    QToolBar *toolBar;

    /**
     * @brief Bible Text Tab widget
     */
    BibleTextTabWidget *btTabWidget;

    /**
     * @brief bible dict tab widget
     */
    BibleDictTabWidget *bdTabWidget;

    /**
     * @brief brProjectDlg
     */
    BibleReaderProjectDialog *brProjectDlg;

    /**
     * @brief bible reader config dialog
     */
    BibleReaderConfigDlg *brConfigDlg;

    /**
     * @brief resource manager dialog
     */
    BibleReaderResourceManagerDlg *brMgrDlg;

    // toolbar actions
    QAction *navToNextChapterAction;
    QAction *navToPrevChapterAction;
    QAction *navToHistoryBack;
    QAction *navToHistoryForword;
    QAction *btZoomInAction;
    QAction *btZoomOutAction;
    QAction *copyCurVerseAction;
    QAction *goVerseAction;
    QAction *printBTAction;

    // File menu actions
    QAction *exitAppAction;
    QAction *projectVersesAction;

    // Edit menu actions
    // QAction *copyAction;

    // Tool menu actions
    QAction *configAction;
    QAction *resourceManagerAction;

    // Help menu actions
    QAction *aboutMeAction;
    QAction *checkUpdate;
    QAction *showHelpContent;
    QAction *donateBibleReader;

    // menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QMenu *toolMenu;
    QMenuBar *mainMenuBar;


    /**
     * @brief status bar.
     */
    QStatusBar *statusBar;

signals:
    void chapterChanged(int book, int chapter, int verse);

private slots:
    void onChapterVerseChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onChapterChanged(int book, int chapter, int verse);
    void navToNextChapter();
    void navToPrevChapter();
    void navToBackHistory();
    void navToForwordHistory();
    void btZoomIn();
    void btZoomOut();
    void btPrint();
    void btGoVerse();

    // menu actions slots
    void quitBibleReader();
    void projectVerses();
    void showCfgDlg();
    void checkNewVersion();
    void openHelpPage();

    void replyFinished(QNetworkReply *reply);

    // show about dialog
    void showAboutDlg();
    // show donate dialog
    void showDonationDlg();

    // show resource manager dilaog
    void showResMgrDlg();

    // copy current verse
    void copyCurrentVerse();

    // show current verse information in status bar.
    void showCurrentVerseInfo();

    void setHistoryNavActionsEnabled(int which);

    void windowMenuTrigger();

    void toggleDockWidget(bool toggled);


private:
    // create all widgets
    void createWidgets();

    // create global toolbar
    void createGlobalToolbar();

    // create main centre widget
    void createCentralWidget();

    // create bible tree dockable widget
    void createBibleTreeDockWidget();

    // create bible search dock widget
    void createBSDockWidget();

    // create bible comentary dock widget
    void createBCDockWidget();

    // create dict dock widget
    void createDictDockWidget();

    // create menus
    void createTopMenus();

    // create status bar
    void createStatusBar();

    // create bible go menu
    QMenu* buildBibleTreeMenu();
};

#endif // MAINWINDOW_H
