#ifndef BIBLEREADERCONFIGDLG_H
#define BIBLEREADERCONFIGDLG_H

#include <QDialog>
#include <QStackedWidget>
#include <QListWidget>

#include "biblereaderconfigurator.h"

class BibleReaderConfigDlg : public QDialog
{
    Q_OBJECT
public:
    explicit BibleReaderConfigDlg(BibleReaderConfigurator *cfg, QWidget *parent = 0);
    ~BibleReaderConfigDlg();


public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private slots:
    void applySettings();
private:
    void createIcons();
private:

    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;

    BibleReaderConfigurator *configurator;
};

#endif // BIBLEREADERCONFIGRATIONWINDOW_H
