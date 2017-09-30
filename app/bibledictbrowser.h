#ifndef BIBLEDICTBROWSER_H
#define BIBLEDICTBROWSER_H

#include <QUrl>
#include <QTextBrowser>
#include "biblereadercore.h"

class BibleDictBrowser : public QTextBrowser
{
public:
    BibleDictBrowser(BibleReaderCore *bibleReaderCore, QWidget *parent);

protected:
    void mouseMoveEvent(QMouseEvent *e);

private slots:
    void navTo(QUrl brUrl);
private:
    BibleReaderCore *brCore;
};

#endif // BIBLEDICTBROWSER_H
