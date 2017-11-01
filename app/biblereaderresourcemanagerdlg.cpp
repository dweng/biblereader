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
    if (resUrlWidget) {
        delete resUrlWidget;
    }

    if (resItemsWidget) {
        delete resItemsWidget;
    }
}

void BibleReaderResourceManagerDlg::createWidgets()
{
    resUrlWidget = new QListWidget(this);
    resUrlWidget->setMinimumWidth(50);
    resUrlWidget->setMaximumWidth(100);
    connect(resUrlWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(refreshResources(QModelIndex)));

    QMap<QString, QUrl> resUrls = manager->getResourceUrls();
    QMap<QString, QUrl>::const_iterator i = resUrls.constBegin();
    while (i != resUrls.constEnd()) {
        QListWidgetItem *item = new QListWidgetItem(i.key());

        resUrlWidget->addItem(item);
        ++i;
    }

    resItemsWidget = new QTableWidget(this);
    resItemsWidget->setColumnCount(6);
    QStringList headers;
    headers << tr("Name") << tr("Size")
            << tr("Description") << tr("Version") << tr("Operation");
    resItemsWidget->setHorizontalHeaderLabels(headers);
    resItemsWidget->horizontalHeader()->setVisible(true);
}

void BibleReaderResourceManagerDlg::doLayout() {
    QSplitter *splitter = new QSplitter(this);
    QHBoxLayout *layout = new QHBoxLayout(this);

    splitter->addWidget(resUrlWidget);
    splitter->addWidget(resItemsWidget);

    layout->addWidget(splitter);

    /*
    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->addAction(tr("File"));
    menuBar->addAction(tr("Operation"));

    layout->setMenuBar(menuBar);
    */

    setLayout(layout);
}

void BibleReaderResourceManagerDlg::refreshResources(QModelIndex index)
{
    QListWidgetItem *item = resUrlWidget->item(index.row());

    QList<BRResource> resources = manager->getResources(item->text());

    resItemsWidget->clearContents();
    resItemsWidget->setRowCount(resources.size());

    for (int i = 0; i < resources.size(); i++) {
        BRResource res = resources[i];
        QTableWidgetItem *tmp = new QTableWidgetItem(res.key);
        resItemsWidget->setItem(i, 0, tmp);

        QTableWidgetItem *tmp2 = new QTableWidgetItem(res.typeStr);
        resItemsWidget->setItem(i, 1, tmp2);

        QTableWidgetItem *tmp3 = new QTableWidgetItem(QString::number(res.size));
        resItemsWidget->setItem(i, 2, tmp3);

        QTableWidgetItem *tmp4 = new QTableWidgetItem(res.description);
        resItemsWidget->setItem(i, 3, tmp4);

        QTableWidgetItem *tmp5 = new QTableWidgetItem(QString::number(res.version));
        resItemsWidget->setItem(i, 4, tmp5);

        QPushButton *btn = new QPushButton(tr("Update"));
        resItemsWidget->setCellWidget(i, 5, btn);
    }
}


