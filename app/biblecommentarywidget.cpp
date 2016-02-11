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
    connect(brCore, SIGNAL(currentVerseChanged(int,int,int)), this, SLOT(changeChapterCmt(int,int,int)));
    createWidgets();
}

BibleCommentaryWidget::~BibleCommentaryWidget()
{
    destroyWidgets();
}

QString BibleCommentaryWidget::changeChapterCmt(int book, int chapter, int verse)
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

    if (book == 1 && chapter == 0 && previous == NULL) {
        book = brCore->getCurrentBookNumber();
        chapter = brCore->getCurrentChapterNumber();
    }

    return changeChapterCmt(book, chapter, 1);
}

QString BibleCommentaryWidget::getCmtTitle() const
{
    return cmtTitle;
}

void BibleCommentaryWidget::setCmtTitle(const QString &value)
{
    cmtTitle = value;
}

QString BibleCommentaryWidget::getCmtFullName() const
{
    return cmtFullName;
}

void BibleCommentaryWidget::setCmtFullName(const QString &value)
{
    cmtFullName = value;
}

void BibleCommentaryWidget::createWidgets()
{
    int curBookNum = brCore->getCurrentBookNumber();
    int curChapterNum = brCore->getCurrentChapterNumber();

    section = new BibleTreeWidget(brCore, this);
    section->setMaximumWidth(150);
    QList<QTreeWidgetItem *> items =
            section->findItems(QString::number(curBookNum), Qt::MatchExactly, 1);
    section->expandItem(items.value(0));
    section->setCurrentItem(items.value(0));

    connect(section, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(changeChapterCmt(QTreeWidgetItem*,QTreeWidgetItem*)));
    content = new QTextEdit(this);
    content->setReadOnly(true);
    content->setHtml(brCore->getChapterCmt(
                         cmtName,
                         curBookNum,
                         curChapterNum
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
