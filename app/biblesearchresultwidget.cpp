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
#include <QMouseEvent>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include "bibleverse.h"
#include "biblesearchresultwidget.h"

BibleSearchResultWidget::BibleSearchResultWidget(BibleReaderCore *brc, QWidget *parent) :
    QTreeWidget(parent)
{
    brCore = brc;
}

/*
void BibleSearchResultWidget::mousePressEvent(QMouseEvent *event)
{
    QModelIndex current = currentIndex();
    if (event->button() == Qt::RightButton) {
        emit clicked(current);
    } else {
        QTreeWidget::mousePressEvent(event);
    }
}
*/

void BibleSearchResultWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex current = currentIndex();
    if (current.parent() != QModelIndex()) {
        QMenu *menu = new QMenu(this);
        QAction *copyVerse = menu->addAction(tr("Copy this verse"));
        connect(copyVerse, SIGNAL(triggered()), this, SLOT(copyCurVerse()));
        menu->exec(event->globalPos());

        delete menu;
    }
    QTreeWidget::contextMenuEvent(event);
}

bool BibleSearchResultWidget::copyCurVerse()
{
    QClipboard *cb = QApplication::clipboard();
    QTreeWidgetItem *current = currentItem();
    if (current->parent()) {
        BibleVerse verse = brCore->getVerse(
                    current->data(2, Qt::DisplayRole).toString(),
                    current->data(3, Qt::DisplayRole).toInt(),
                    current->data(4, Qt::DisplayRole).toInt(),
                    current->data(5, Qt::DisplayRole).toInt());
        cb->setText(verse.text(), QClipboard::Clipboard);
    }
    return true;
}
