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
#ifndef BIBLEDICTTABWIDGET_H
#define BIBLEDICTTABWIDGET_H

#include "biblereadercore.h"
#include "bibledictionarywidget.h"
#include "bibledictinfo.h"

#include <QList>
#include <QTabWidget>

class BibleDictTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    BibleDictTabWidget(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleDictTabWidget();

private:
    BibleReaderCore *brCore;

    QList<BibleDictionaryWidget *> dictWidgets;

    QList<BibleDictInfo> dicts;

private slots:
    void updateCurrentDict(int index);
    void showDictItem(QString dictName, QString itemName);
};

#endif // BIBLEDICTTABWIDGET_H
