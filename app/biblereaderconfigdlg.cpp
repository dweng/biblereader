#include "biblereaderconfigdlg.h"
#include <QtWidgets>

#include "biblereaderconfiggeneralpage.h"

BibleReaderConfigDlg::BibleReaderConfigDlg(BibleReaderConfigurator *cfg,
                                           QWidget *parent) : QDialog(parent)
{
    configurator = cfg;

    contentsWidget = new QListWidget;
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(96);
    //contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(new BibleReaderConfigGeneralPage(configurator, this));

    QPushButton *closeButton = new QPushButton(tr("Close"));
    QPushButton *applyButton = new QPushButton(tr("Apply"));
    QPushButton *okButton = new QPushButton(tr("Ok"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(close()));


    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);
    buttonsLayout->addWidget(applyButton);
    buttonsLayout->addWidget(okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));
}

BibleReaderConfigDlg::~BibleReaderConfigDlg()
{
}

void BibleReaderConfigDlg::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    //configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("General"));
    configButton->setTextAlignment(Qt::AlignLeft);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void BibleReaderConfigDlg::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void BibleReaderConfigDlg::applySettings()
{

}



