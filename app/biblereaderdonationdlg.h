#ifndef BIBLEREADERDONATIONDLG_H
#define BIBLEREADERDONATIONDLG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QTextEdit;

QT_END_NAMESPACE

class BibleReaderDonationDlg : public QDialog
{
    Q_OBJECT
public:
    explicit BibleReaderDonationDlg(QWidget *parent = 0);
    ~BibleReaderDonationDlg();

signals:

public slots:

private:
    QTextEdit *donation;
    QString readDonation(QString path);
};

#endif // BIBLEREADERDONATIONDLG_H
