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


    BibleReaderCore *brCore;
    BibleReaderResourceManager *manager;

private:
    void createWidgets();
    void doLayout();

private slots:
    void updateResList();
};

#endif // BIBLEREADERRESOURCEMANAGERDLG_H
