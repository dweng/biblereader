#ifndef BIBLEREADERCONFIGDLG_H
#define BIBLEREADERCONFIGDLG_H

#include <QDialog>
#include <QStackedWidget>
#include <QListWidget>

class BibleReaderConfigDlg : public QDialog
{
    Q_OBJECT
public:
    explicit BibleReaderConfigDlg(QWidget *parent = 0);
    ~BibleReaderConfigDlg();


public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void createIcons();
private:

    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
};

#endif // BIBLEREADERCONFIGRATIONWINDOW_H
