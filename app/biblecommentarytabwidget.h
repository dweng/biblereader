#ifndef BIBLECOMMENTARYTABWIDGET_H
#define BIBLECOMMENTARYTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QList>

#include "biblecommentarywidget.h"
#include "biblereadercore.h"

class BibleCommentaryTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    BibleCommentaryTabWidget(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleCommentaryTabWidget();

private:
    QList<BibleCommentaryWidget *> bcWidgets;
    BibleReaderCore *brCore;
};

#endif // BIBLECOMMENTARYTABWIDGET_H
