#include <QSplitter>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenuBar>
#include <QPushButton>
#include <QTableWidgetItem>

#include "biblereaderresourcemanagerdlg.h"
#include "Logger.h"
#include "ConsoleAppender.h"

BibleReaderResourceManagerDlg::BibleReaderResourceManagerDlg(
        BibleReaderCore *brc, QWidget *parent):
    QDialog(parent)
{
    brCore = brc;
    manager = brCore->getResManager();

    connect(manager, SIGNAL(refreshed()), this, SLOT(updateResList()));

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
    resItemsWidget = new QTableWidget(this);
    resItemsWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

    connect(refreshBtn, SIGNAL(clicked()), manager, SLOT(refresh()));

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

void BibleReaderResourceManagerDlg::updateResList()
{
    QList<BRResource> resources = manager->getResources();
    QTableWidgetItem *temp;

    for (int i = 0; i < resources.count(); i++) {
        // add type
        resItemsWidget->setRowCount(resItemsWidget->rowCount() + 1);
        int row = resItemsWidget->rowCount();

        BRResource res = resources[i];

        // type
        temp = new QTableWidgetItem(res.typeStr);
        resItemsWidget->setItem(row-1, 0, temp);

        // name
        temp = new QTableWidgetItem(res.longName);
        resItemsWidget->setItem(row-1, 1, temp);

        // size
        temp = new QTableWidgetItem(QString::number(res.size));
        resItemsWidget->setItem(row-1, 2, temp);

        // description
        temp = new QTableWidgetItem(res.description);
        resItemsWidget->setItem(row-1, 3, temp);

        // version
        temp = new QTableWidgetItem(QString::number(res.version));
        resItemsWidget->setItem(row-1, 4, temp);

        // operation
    }
}



