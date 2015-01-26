#ifndef BIBLEDICTTABWIDGET_H
#define BIBLEDICTTABWIDGET_H

#include "biblereadercore.h"
#include "bibledictionarywidget.h"
#include "bibledictinfo.h"

#include <QList>
#include <QTabWidget>

class BibleDictTabWidget : public QTabWidget
{
public:
    BibleDictTabWidget(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleDictTabWidget();

private:
    BibleReaderCore *brCore;

    QList<BibleDictionaryWidget *> dictWidgets;

    QList<BibleDictInfo> dicts;
};

#endif // BIBLEDICTTABWIDGET_H
