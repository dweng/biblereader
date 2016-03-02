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
#ifndef BIBLESEARCHWIDGET_H
#define BIBLESEARCHWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QList>
#include "bibleverse.h"
#include "biblebook.h"
#include "biblereadercore.h"
#include "biblesearchresultwidget.h"
/**
 * @brief The BibleSearchWidget class
 * This class represents the Bible search window.
 * We can search verse in this widget and it will show
 * results to you.
 *
 * @version 1.0
 */
class BibleSearchWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent parent widget, default to NULL(0)
     */
    explicit BibleSearchWidget(BibleReaderCore *brc, QWidget *parent = 0);

    /**
     * @brief Destructor
     */
    ~BibleSearchWidget();
signals:
    void goToVerse(QString version, int book, int chapter, int verse);

public slots:
    void onBibleVersionChanged(QString version);

private slots:
    bool navToChapter(QTreeWidgetItem *current, int column);
    void searchText(QString q);

private:
    // UI related members
    QComboBox *searchInput;
    QLabel *searchBaseLabel;
    QComboBox *searchRangeBase;
    QPushButton *searchBtn;
    BibleSearchResultWidget *searchResult;
    QGroupBox *searchRangeBox;
    QLabel *searchAdvLabel;
    QLabel *searchAdvStartLabel;
    QLabel *searchAdvEndLabel;
    QComboBox *searchRangeAdvStart;
    QComboBox *searchRangeAdvEnd;

private:
    // None UI related members
    BibleReaderCore *brCore;

    QList<BibleVerse> result;

    QList<BibleBook> allBooks;

private:
    // private functions
    /**
     * @brief doLayout
     * Layout all widgets in search widget.
     */
    void doLayout();

    /**
     * @brief createWidgets
     * Create all widgets in search widget.
     */
    void createWidgets();

    /**
     * @brief destoryWidgets
     * Destory all widgets in search widget.
     */
    void destoryWidgets();

    /**
     * @brief addAllBooks
     * Add 66 books of Bible into ComboBox
     * @param cb the target ComboBox
     */
    void addAllBooks(QComboBox *cb);

private slots:
    /**
     * @brief Get Search Result
     */
    void getSearchResult();

    /**
     * @brief Apply search scope.
     * @param index current index
     */
    void applySearchScope(int index);

};

#endif // BIBLESEARCHWIDGET_H
