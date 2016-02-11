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

#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QLineEdit>
#include "biblereaderhtmldelegate.h"
#include "bibleverse.h"
#include "biblesearchwidget.h"

// for logging
#include "Logger.h"

BibleSearchWidget::BibleSearchWidget(BibleReaderCore *brc, QWidget *parent) :
    QWidget(parent)
{
    // set widgets to NULL
    searchRangeAdvEnd = NULL;
    searchRangeAdvStart = NULL;

    brCore = brc;
    createWidgets();
    doLayout();
}

BibleSearchWidget::~BibleSearchWidget() {
    destoryWidgets();
}

void BibleSearchWidget::onBibleVersionChanged(QString version)
{
    addAllBooks(searchRangeAdvStart);
    addAllBooks(searchRangeAdvEnd);
}

bool BibleSearchWidget::navToChapter(QTreeWidgetItem* current, int column)
{
    if(current->data(2, Qt::DisplayRole).isValid()) {
        QString version = current->data(2, Qt::DisplayRole).toString();
        int book = current->data(3, Qt::DisplayRole).toInt();
        int chapter = current->data(4, Qt::DisplayRole).toInt();
        int verse = current->data(5, Qt::DisplayRole).toInt();

        // emit chapterChanged signal
        emit goToVerse(version, book, chapter, verse);
    }

    return true;
}



void BibleSearchWidget::doLayout() {
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(searchInput, 0, 0, 1, 3);
    gridLayout->addWidget(searchBtn, 0, 3, 1, 1);

    QGridLayout *gridLayout2 = new QGridLayout(searchRangeBox);
    gridLayout2->addWidget(searchBaseLabel, 0, 0, 1, 1);
    gridLayout2->addWidget(searchRangeBase, 1, 0, 1, 4);

    gridLayout2->addWidget(searchAdvLabel, 2, 0, 1, 4);
    gridLayout2->addWidget(searchAdvStartLabel, 3, 0, 1, 1);
    gridLayout2->addWidget(searchRangeAdvStart, 3, 1, 1, 3);
    gridLayout2->addWidget(searchAdvEndLabel, 4, 0, 1, 1);
    gridLayout2->addWidget(searchRangeAdvEnd, 4, 1, 1, 3);
    searchRangeBox->setLayout(gridLayout2);


    gridLayout->addWidget(searchRangeBox, 1, 0, 1, 4);
    gridLayout->addWidget(searchResult, 2, 0, 1, 4);
    setLayout(gridLayout);
}

void BibleSearchWidget::createWidgets()
{
    searchInput = new QComboBox(this);
    searchInput->setEditable(true);
    connect(searchInput->lineEdit(), SIGNAL(returnPressed()),
            this, SLOT(getSearchResult()));

    searchBtn = new QPushButton(tr("Search"), this);
    connect(searchBtn, SIGNAL(clicked()), this, SLOT(getSearchResult()));

    searchRangeBox = new QGroupBox(tr("Search range"), this);
    searchBaseLabel = new QLabel(tr("Base:"), this);
    searchRangeBase = new QComboBox(this);
    connect(searchRangeBase, SIGNAL(currentIndexChanged(int)),
            this, SLOT(applySearchScope(int)));

    QStringList searchScopeTitles;
    searchScopeTitles << tr("All") << tr("Pentateuch") << tr("History")
                      << tr("Wisdom") << tr("Prophets") << tr("Gospels")
                      << tr("Acts") << tr("Paul Epistles") << tr("General Epistles")
                      << tr("Revelation") << tr("User Defined");
    for (int i = 1; i <= 11; i++) {
        searchRangeBase->addItem(searchScopeTitles[i-1], i);
    }

    searchAdvLabel = new QLabel(tr("<b>OR</b> Advanced:"), this);
    // Advanced search range: Start
    searchAdvStartLabel = new QLabel(tr("Start:"), this);
    searchRangeAdvStart = new QComboBox(this);
    addAllBooks(searchRangeAdvStart);
    searchRangeAdvStart->setEnabled(false);
    // Advanced search range: End
    searchAdvEndLabel = new QLabel(tr("End:"), this);
    searchRangeAdvEnd = new QComboBox(this);
    addAllBooks(searchRangeAdvEnd);
    searchRangeAdvEnd->setEnabled(false);

    // search result tree widget
    searchResult = new BibleSearchResultWidget(brCore, this);
    QStringList headers;
    headers << tr("Verse") << tr("Text") << tr("Version") << tr("Book")
            << tr("Chapter") << tr("Verse");
    searchResult->setHeaderLabels(headers);
    searchResult->setColumnHidden(2, true);
    searchResult->setColumnHidden(3, true);
    searchResult->setColumnHidden(4, true);
    searchResult->setColumnHidden(5, true);
    // set item delegate
    searchResult->setItemDelegate(new BibleReaderHTMLDelegate());

    // click widget item signal catch.
    connect(searchResult, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(navToChapter(QTreeWidgetItem*, int)));
    // connect(searchResult, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
    //        this, SLOT());
}

void BibleSearchWidget::destoryWidgets()
{
    delete searchInput;
    delete searchBtn;
    delete searchRangeBase;
    delete searchRangeAdvStart;
    delete searchRangeAdvEnd;
    delete searchRangeBox;
    delete searchResult;
}

void BibleSearchWidget::addAllBooks(QComboBox *cb)
{
    // reset ComboBox
    cb->clear();

    // get all books of current bible version
    allBooks = brCore->getAllBooks();
    // add books into combo box
    for (int i = 1; i <= allBooks.count(); i++) {
        cb->addItem(allBooks[i-1].getLongName(), allBooks[i-1].getBookNumber());
    }
}

void BibleSearchWidget::getSearchResult()
{
    QString q = searchInput->currentText();
    if (q.isEmpty()) return;
    BibleTextDAO::SearchScope scope = static_cast<BibleTextDAO::SearchScope>
            (
            #if QT_VERSION >= 0x050200
                searchRangeBase->currentData().toInt()
            #else
                searchRangeBase->itemData(searchRangeBase->currentIndex()).toInt()
            #endif
            );
    if (scope == BibleTextDAO::UserDefined) {
#if QT_VERSION >= 0x050200
        int start = searchRangeAdvStart->currentData().toInt();
        int end = searchRangeAdvEnd->currentData().toInt();
#else
        int start = searchRangeAdvStart->itemData(searchRangeAdvStart->currentIndex()).toInt();
        int end = searchRangeAdvEnd->itemData(searchRangeAdvStart->currentIndex()).toInt();
#endif
        if (start > end) {
            QMessageBox::warning(this, tr("BibleReader Warning"), tr("start book should before end book"));
            return;
        } else {
            QList<int> customBooks;
            for (int i = start; i <= end; i++) {
                customBooks.push_back(i);
            }
            result = brCore->search(q, scope, customBooks);
        }
    } else {
        result = brCore->search(q, scope);
    }

    QTreeWidgetItem *root = new QTreeWidgetItem();
    root->setData(0, Qt::DisplayRole, QString("[%1:%2]").arg(q, QString::number(result.count())));
    QString searchTip = tr("Searched version:<font color=\"blue\">%1</font><br />Query string:<font color=\"blue\">%2</font>");
    root->setToolTip(0, searchTip.arg(brCore->getCurrentBibleInfo().getFullname(), q));
    for (int i = 0; i < result.count(); i++) {
        BibleVerse b = result[i];

        QTreeWidgetItem *item = new QTreeWidgetItem(root);
        QString verse = QString("<font color=\"green\">%1 %2:%3</font>").arg(b.getBookName(),
                  QString::number(b.getChapter()),
                  QString::number(b.getVerse()));
        // apply verse text shower to QLabel

        item->setData(0, Qt::DisplayRole, verse);
        QString hilightText =  b.getVerseText().replace(q, QString("<font color=\"red\">"+q+"</font>"));
        item->setData(1, Qt::DisplayRole, hilightText);
        item->setData(2, Qt::DisplayRole, b.getBibleVersion());
        item->setData(3, Qt::DisplayRole, b.getBookNumber());
        item->setData(4, Qt::DisplayRole, b.getChapter());
        item->setData(5, Qt::DisplayRole, b.getVerse());
        item->setToolTip(0, verse.append(" ").append(hilightText));
    }

    searchResult->addTopLevelItem(root);
}

void BibleSearchWidget::applySearchScope(int index)
{
    QComboBox *cb = qobject_cast<QComboBox*>(sender());
#if QT_VERSION >= 0x050200
    int data = cb->currentData().toInt();
#else
    int data = cb->itemData(index).toInt();
#endif
    BibleTextDAO::SearchScope scope =
            static_cast<BibleTextDAO::SearchScope>(data);

    if (searchRangeAdvStart && searchRangeAdvEnd) {
        if (scope == BibleTextDAO::UserDefined) {
            searchRangeAdvStart->setEnabled(true);
            searchRangeAdvEnd->setEnabled(true);
        } else {
            searchRangeAdvStart->setEnabled(false);
            searchRangeAdvEnd->setEnabled(false);
        }
    }
}

