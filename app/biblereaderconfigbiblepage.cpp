#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>

#include "biblereaderconfigbiblepage.h"

BibleReaderConfigBiblePage::BibleReaderConfigBiblePage(BibleReaderConfigurator *cfg, QWidget *parent) : QWidget(parent)
{
    configurator = cfg;

    QGroupBox *bibleBox = new QGroupBox(tr("Bible Setting"), this);
    QFormLayout *bibleLayout = new QFormLayout(bibleBox);
    defaultXrefBibleCB = new QComboBox(this);

    bibleLayout->addRow(tr("default bible version:"), defaultXrefBibleCB);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(bibleBox);


}
