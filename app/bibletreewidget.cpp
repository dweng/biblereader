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
#include "bibletreewidget.h"
#include "biblebook.h"
#include "Logger.h"

BibleTreeWidget::BibleTreeWidget(BibleReaderCore *brc, QWidget *parent) :
    QTreeWidget(parent)
{
    LOG_INFO() << "bible tree widget initing...";
    brCore = brc;

    // add bible tree widget headers
    QStringList headers;
    headers << tr("book") << tr("book number") << tr("chapter") << tr("verse");
    this->setHeaderLabels(headers);
    // build tree widget items
    buildBibleTreeData();
    // add top tree widget item
    this->addTopLevelItems(bookItems);
    // hide "book number" column
    this->setColumnHidden(1, true);
    LOG_INFO() << "bible tree widget inited.";
}

BibleTreeWidget::~BibleTreeWidget()
{
    LOG_INFO() << "Destructor";
    destroyBookItems();
}

void BibleTreeWidget::rebuildBibleTreeData()
{
    LOG_INFO() << "Rebuilding bible tree data...";
    this->clear();
    this->buildBibleTreeData();
    this->addTopLevelItems(bookItems);
    this->setColumnHidden(1, true);
    LOG_INFO() << "Rebuilt bible tree data";
}

void BibleTreeWidget::onBibleVersionChanged(QString version)
{
    rebuildBibleTreeData();
}

void BibleTreeWidget::buildBibleTreeData()
{
    LOG_DEBUG("Building Bible Tree widget data...");
    // reset books
    bookItems.clear();

    // get all bible books
    QList<BibleBook> books = brCore->getAllBooks();
    for(int i=0;i<books.count();i++)
    {
        BibleBook book = books.value(i);
        // book item
        QTreeWidgetItem *bookItem = new QTreeWidgetItem();
        bookItem->setData(0, Qt::DisplayRole, book.getShortName());
        bookItem->setData(1, Qt::DisplayRole, book.getBookNumber());
        // for default, show chapter 1 text of each book of Bible
        bookItem->setData(2, Qt::DisplayRole, 1);

        // get all chapters for one book
        QList<int> chapters = brCore->getChaptersCountOfOneBook(book.getBookNumber());
        for(int j=0;j<chapters.count();j++)
        {
            QTreeWidgetItem *chapterItem = new QTreeWidgetItem(bookItem);
            chapterItem->setData(0, Qt::DisplayRole, book.getShortName());
            chapterItem->setData(1, Qt::DisplayRole, book.getBookNumber());
            chapterItem->setData(2, Qt::DisplayRole, j+1);
            int versesCount = chapters.value(j);
            // add verses to chapter
            for (int k=0; k<versesCount;k++)
            {
                QTreeWidgetItem *verseItem = new QTreeWidgetItem(chapterItem);
                verseItem->setData(0, Qt::DisplayRole, book.getShortName());
                verseItem->setData(1, Qt::DisplayRole, book.getBookNumber());
                verseItem->setData(2, Qt::DisplayRole, j+1);
                verseItem->setData(3, Qt::DisplayRole, k+1);
            }
        }

        bookItems.push_back(bookItem);
    }
    LOG_DEBUG("Built Bible Tree widget data");
}

void BibleTreeWidget::destroyBookItems()
{
    LOG_INFO() << "Destroying book items";
    for (int i = 0; i < bookItems.count(); i++) {
        this->takeTopLevelItem(i);
    }

    bookItems.clear();
    LOG_INFO() << "Book items destroyed.";
}
