#include <QSplitter>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenuBar>
#include <QPushButton>

#include "biblereaderresourcemanagerdlg.h"
#include "Logger.h"
#include "ConsoleAppender.h"

BibleReaderResourceManagerDlg::BibleReaderResourceManagerDlg(
        BibleReaderCore *brc, QWidget *parent):
    QDialog(parent)
{
    brCore = brc;
    manager = brCore->getResManager();

    setWindowTitle(tr("Resource Manager"));
    setMinimumSize(600, 480);
    setModal(true);

    // create UI
    createWidgets();
    doLayout();


}

BibleReaderResourceManagerDlg::~BibleReaderResourceManagerDlg()
{
    if (resItemsWidget) {
        delete resItemsWidget;
    }
}

void BibleReaderResourceManagerDlg::createWidgets()
{
    QMap<QString, QUrl> resUrls = manager->getResourceUrls();


    resItemsWidget = new QTableWidget(this);
    resItemsWidget->setColumnCount(6);
    QStringList headers;
    headers << tr("Type") << tr("Name") << tr("Size")
            << tr("Description") << tr("Version") << tr("Operation");
    resItemsWidget->setHorizontalHeaderLabels(headers);
    resItemsWidget->horizontalHeader()->setVisible(true);

    refreshBtn = new QPushButton(tr("Refresh"));
    closeBtn = new QPushButton(tr("Close"));
}

void BibleReaderResourceManagerDlg::doLayout() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(this);

    btnLayout->addWidget(refreshBtn);
    btnLayout->addWidget(closeBtn);

    layout->addWidget(resItemsWidget, 9);
    layout->addLayout(btnLayout, 1);


    /*
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->addAction(tr("File"));
    menuBar->addAction(tr("Operation"));

    layout->setMenuBar(menuBar);
    */

    setLayout(layout);
}



