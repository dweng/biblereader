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

BibleCommentaryWidget::BibleCommentaryWidget(BibleReaderCore *brc, QWidget *parent) :
    QWidget(parent)
{
    brCore = brc;
    createWidgets();
}

BibleCommentaryWidget::~BibleCommentaryWidget()
{
    destroyWidgets();
}

void BibleCommentaryWidget::createWidgets()
{
    section = new QTreeWidget(this);
    section->setMaximumWidth(150);
    content = new QTextEdit(this);
    content->setReadOnly(true);


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
