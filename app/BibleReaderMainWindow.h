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

#include <QMainWindow>
#include <QListWidgetItem>
#include "bibletexttabwidget.h"
#include "biblereadercore.h"
#include "bibletextbrowser.h"
#include "bibletreewidget.h"
#include "biblesearchwidget.h"

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
     * @brief Tool bar for main application
     */
    QToolBar *toolBar;

    /**
     * @brief Bible Text Tab widget
     */
    BibleTextTabWidget *btTabWidget;

    // toolbar actions
    QAction *navToNextChapterAction;
    QAction *navToPrevChapterAction;

    // File menu actions
    QAction *exitAppAction;

    // Edit menu actions
    QAction *copyAction;

    // Help menu actions
    QAction *aboutMeAction;
    QAction *checkUpdate;
    QAction *showHelpContent;
    QAction *donateBibleReader;

    // menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenuBar *mainMenuBar;


    /**
     * @brief status bar.
     */
    QStatusBar *statusBar;

    /**
     * @brief dict show explaination window
     */
    QTextEdit *dictShowExplaination;
signals:
    void chapterChanged(int book, int chapter, int verse);

private slots:
    void onChapterVerseChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void onChapterChanged(int book, int chapter, int verse);
    void navToNextChapter();
    void navToPrevChapter();

    // menu actions slots
    void quitBibleReader();
    // show about dialog
    void showAboutDlg();
    // show donate dialog
    void showDonationDlg();

};

#endif // MAINWINDOW_H
