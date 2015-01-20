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
#ifndef BIBLESEARCHRESULTWIDGET_H
#define BIBLESEARCHRESULTWIDGET_H

#include <QTreeWidget>
#include <QContextMenuEvent>
#include "biblereadercore.h"
class BibleSearchResultWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit BibleSearchResultWidget(BibleReaderCore *brc, QWidget *parent = 0);

public slots:

protected:
    //void mousePressEvent();

private slots:
    bool copyCurVerse();

protected:
    //void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

private:
    BibleReaderCore *brCore;

private:
    void setCurrentVerse();
};

#endif // BIBLESEARCHRESULTWIDGET_H
