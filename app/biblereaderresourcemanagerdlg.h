#ifndef BIBLEREADERRESOURCEMANAGERDLG_H
#define BIBLEREADERRESOURCEMANAGERDLG_H
#include <QDialog>
#include <QListWidget>
#include <QTableWidget>
#include "biblereadercore.h"


class BibleReaderResourceManagerDlg: public QDialog
{
    Q_OBJECT
public:
    BibleReaderResourceManagerDlg(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleReaderResourceManagerDlg();

private:
    QTableWidget *resItemsWidget;

    QPushButton *refreshBtn;
    QPushButton *closeBtn;
    QPushButton *installUpdateBtn;
    QPushButton *removeBtn;


    BibleReaderCore *brCore;
    BibleReaderResourceManager *manager;

private:
    void createWidgets();
    void doLayout();
    QWidget* createButtons(BRResource resource);

    // combine resources
    void mergeResources(QList<BRResource> &resources);

private slots:
    void updateResList();
    bool installRes();
    bool updateRes();
    bool removeRes();
};

#endif // BIBLEREADERRESOURCEMANAGERDLG_H
