#include <QSplitter>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenuBar>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QMessageBox>

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
    setMinimumSize(800, 600);
    setModal(true);

    // create UI
    createWidgets();
    doLayout();

    // update resource list
    refreshBtn->click();
}

BibleReaderResourceManagerDlg::~BibleReaderResourceManagerDlg()
{
    if (resItemsWidget) {
        delete resItemsWidget;
    }
}

void BibleReaderResourceManagerDlg::createWidgets()
{
    resItemsWidget = new QTreeWidget(this);
    resItemsWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resItemsWidget->setColumnCount(5);
    QStringList headers;
    headers << tr("Name") << tr("Size")
            << tr("Description") << tr("Version") << tr("Operation");
    resItemsWidget->setHeaderLabels(headers);
    resItemsWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    resItemsWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    refreshBtn = new QPushButton(QIcon(":/img/assets/images/arrow_refresh.png"),tr("Refresh"));
    closeBtn = new QPushButton(QIcon(":/img/assets/images/cancel.png"),tr("Close"));
}

void BibleReaderResourceManagerDlg::doLayout() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *btnLayout = new QHBoxLayout(this);

    btnLayout->addWidget(refreshBtn);
    //btnLayout->addWidget(installUpdateBtn);
    //btnLayout->addWidget(removeBtn);
    btnLayout->addWidget(closeBtn);

    connect(refreshBtn, SIGNAL(clicked()), manager, SLOT(refresh()));
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(accept()));

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

QWidget *BibleReaderResourceManagerDlg::createButtons(BRResource resource)
{
    QWidget *widget = new QWidget(resItemsWidget);

    QHBoxLayout *layout = new QHBoxLayout(widget);

    QPushButton *updateBtn = new QPushButton( QIcon(":/img/assets/images/arrow_up.png"), tr("Update"));
    updateBtn->setProperty("resname", QVariant(resource.shortName));
    updateBtn->setProperty("resurl", QVariant(resource.url));
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(updateRes()));
    if (!resource.isinstalled || (resource.isinstalled && resource.isupdated)) {
        updateBtn->setEnabled(false);
    }
    QPushButton *installBtn = new QPushButton(QIcon(":/img/assets/images/add.png"), tr("Install"));
    installBtn->setProperty("resname", QVariant(resource.shortName));
    installBtn->setProperty("resurl", QVariant(resource.url));
    connect(installBtn, SIGNAL(clicked()), this, SLOT(installRes()));
    if (resource.isinstalled) {
        installBtn->setEnabled(false);
    }
    QPushButton *removeBtn = new QPushButton(QIcon(":/img/assets/images/delete.png"), tr("Remove"));
    removeBtn->setProperty("resname", QVariant(resource.shortName));
    removeBtn->setProperty("resurl", QVariant(resource.url));
    connect(removeBtn, SIGNAL(clicked()), this, SLOT(removeRes()));
    if (!resource.isinstalled) {
        removeBtn->setEnabled(false);
    }

    layout->addWidget(updateBtn);
    layout->addWidget(installBtn);
    layout->addWidget(removeBtn);

    widget->setLayout(layout);

    return widget;
}

void BibleReaderResourceManagerDlg::mergeResources(QList<BRResource> &resources)
{
    // read local resources
    QList<BibleDictInfo> dicts = brCore->getAllDictionarys();
    QList<BibleInfo> bibles = brCore->getAllBibleVersions();

    // merge local and online resources
    // dict
    for (int i = 0; i < dicts.count(); i++) {
        for (int j = 0; j < resources.count(); j++) {
            if (resources[j].type == Dict) {
                if (resources[j].shortName == dicts[i].getShortname()) {
                    if (dicts[i].getVersion() < resources[j].version) {
                        resources[j].isupdated = false;
                        resources[j].isinstalled = true;
                    } else {
                        resources[j].isupdated = true;
                        resources[j].isinstalled = true;
                    }
                    break;
                }
            }
        }
    }

    // bible
    for (int i = 0; i < bibles.count(); i++) {
        for (int j = 0; j < resources.count(); j++) {
            if (resources[j].type == Bible) {
                if (resources[j].shortName == bibles[i].getVersion()) {
                    if (bibles[i].getNumber() < resources[j].version) {
                        resources[j].isupdated = false;
                        resources[j].isinstalled = true;
                    } else {
                        resources[j].isupdated = true;
                        resources[j].isinstalled = true;
                    }
                    break;
                }
            }
        }
    }
}

void BibleReaderResourceManagerDlg::updateResList()
{
    QList<BRResource> resources = manager->getResources();

    // merge local and online resources
    mergeResources(resources);


    QTreeWidgetItem *biblesItem = new QTreeWidgetItem();
    biblesItem->setData(0, Qt::DisplayRole, tr("Bibles(%1)"));
    QTreeWidgetItem *dictsItem = new QTreeWidgetItem();
    dictsItem->setData(0, Qt::DisplayRole, tr("Dictionarys(%1)"));

    QTreeWidgetItem *temp = NULL;
    int biblesCount = 0;
    int dictsCount = 0;
    resItemsWidget->clear();

    for (int i = 0; i < resources.count(); i++) {
        BRResource res = resources[i];
        if (res.type == Bible) {
            biblesCount++;
            temp = new QTreeWidgetItem(biblesItem);
        } else if (res.type == Dict) {
            dictsCount++;
            temp = new QTreeWidgetItem(dictsItem);
        }

        // name
        temp->setData(0, Qt::DisplayRole, res.longName);

        // size
        temp->setData(1, Qt::DisplayRole, QString::number(res.size / (1024)) + "KB");

        // description
        temp->setData(2, Qt::DisplayRole, res.description);


        // version
        temp->setData(3, Qt::DisplayRole, QString::number(res.version));

        // operation
        QWidget *tempBtns = createButtons(res);
        resItemsWidget->setItemWidget(temp, 4, tempBtns);
    }

    biblesItem->setText(0, biblesItem->text(0).arg(biblesCount));
    dictsItem->setText(0, dictsItem->text(0).arg(dictsCount));

    resItemsWidget->addTopLevelItem(biblesItem);
    resItemsWidget->addTopLevelItem(dictsItem);
}

bool BibleReaderResourceManagerDlg::installRes()
{
    bool ret = true;

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    LOG_INFO() << btn->property("resname");
    QList<BRResource> resources = manager->getResources();
    for (int i = 0; i < resources.size(); i++) {
        if (btn->property("resname").toString() == resources[i].shortName) {
            ret = manager->installRes(resources[i], brCore);

            if (ret) {
                QMessageBox::information(this, tr("Done"), tr("Resource installed succeed! Please click Ok to restart Bible Reader to apply changes!"));
                qApp->exit(773);
            } else {
                QMessageBox::warning(this, tr("Sorry"), tr("Resource installed failed!"));
            }
            break;
        }
    }

    updateResList();
    return ret;
}

bool BibleReaderResourceManagerDlg::updateRes()
{
    bool ret = true;

    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    LOG_INFO() << btn->property("resname").toString();
    QList<BRResource> resources = manager->getResources();
    for (int i = 0; i < resources.size(); i++) {
        if (btn->property("resname").toString() == resources[i].shortName) {
            ret = manager->updateRes(resources[i], brCore);

            if (ret) {
                QMessageBox::information(this, tr("Done"), tr("Resource updated succeed! Please click Ok to restart Bible Reader to apply changes!"));
                qApp->exit(773);
            } else {
                QMessageBox::warning(this, tr("Sorry"), tr("Resource updated failed!"));
            }
            break;
        }
    }
    return true;
}

bool BibleReaderResourceManagerDlg::removeRes()
{
    bool ret = true;
    QPushButton* btn = qobject_cast<QPushButton*>(sender());

    QList<BRResource> resources = manager->getResources();
    for (int i = 0; i < resources.size(); i++) {
        if (btn->property("resname").toString() == resources[i].shortName) {
            ret = manager->removeRes(resources[i], brCore);

            if (ret) {
                QMessageBox::information(this, tr("Done"), tr("Resource removed succeed! Please click Ok to restart Bible Reader to apply changes!"));
                qApp->exit(773);
            } else {
                QMessageBox::warning(this, tr("Sorry"), tr("Resource removed failed!"));
            }
            break;
        }
    }

    updateResList();

    return ret;
}



