#ifndef BIBLEREADERDONATIONDLG_H
#define BIBLEREADERDONATIONDLG_H

#include <QTextEdit>
#include <QDialog>
#include <QString>

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
