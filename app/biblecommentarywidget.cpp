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

#include <QHBoxLayout>
#include "biblecommentarywidget.h"
#include "bibletreewidget.h"
BibleCommentaryWidget::BibleCommentaryWidget(BibleReaderCore *brc, QString cn, QWidget *parent) :
    QWidget(parent)
{
    brCore = brc;
    cmtName = cn;
    connect(brCore, SIGNAL(currentChapterChanged(int,int)), this, SLOT(changeChapterCmt(int,int)));
    createWidgets();
}

BibleCommentaryWidget::~BibleCommentaryWidget()
{
    destroyWidgets();
}

QString BibleCommentaryWidget::changeChapterCmt(int book, int chapter)
{
    QString text = brCore->getChapterCmt(cmtName, book, chapter);
    content->setHtml(text);

    return text;
}

QString BibleCommentaryWidget::changeChapterCmt(QTreeWidgetItem *current,
                                                QTreeWidgetItem *previous)
{
    int book = current->data(1, Qt::DisplayRole).toInt();
    int chapter = current->data(2, Qt::DisplayRole).toInt();

    if (book == 0) {
        book = brCore->getCurrentBookNumber();
    }

    if (chapter == 0) {
        chapter = brCore->getCurrentChapterNumber();
    }

    return changeChapterCmt(book, chapter);
}

void BibleCommentaryWidget::createWidgets()
{
    section = new BibleTreeWidget(brCore, this);
    section->setMaximumWidth(150);
    connect(section, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(changeChapterCmt(QTreeWidgetItem*,QTreeWidgetItem*)));
    content = new QTextEdit(this);
    content->setReadOnly(true);
    content->setHtml(brCore->getChapterCmt(
                         cmtName,
                         brCore->getCurrentBookNumber(),
                         brCore->getCurrentChapterNumber()
                         ));


    dictSplitter = new QSplitter(Qt::Horizontal, this);
    dictSplitter->addWidget(section);
    dictSplitter->addWidget(content);

    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->addWidget(dictSplitter);
}

void BibleCommentaryWidget::destroyWidgets()
{
    delete section;
    delete content;
}
