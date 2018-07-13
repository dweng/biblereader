#ifndef BIBLEREADERCONFIGBIBLEPAGE_H
#define BIBLEREADERCONFIGBIBLEPAGE_H

#include <QWidget>
#include <QComboBox>
#include "biblereaderconfigurator.h"
#include "biblereadercore.h"

class BibleReaderConfigBiblePage : public QWidget
{
    Q_OBJECT
public:
    explicit BibleReaderConfigBiblePage(BibleReaderConfigurator *cfg = NULL, QWidget *parent = NULL);

signals:

private:
    BibleReaderConfigurator *configurator;
    BibleReaderCore *brCore;
    QComboBox *defaultXrefBibleCB;
public slots:
};

#endif // BIBLEREADERCONFIGBIBLEPAGE_H
