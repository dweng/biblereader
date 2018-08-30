#ifndef BIBLEREADERABOUTDLG_H
#define BIBLEREADERABOUTDLG_H

#include <QDialog>
#include <QLabel>
#include <QString>
#include <QUrl>

class BibleReaderAboutDlg : public QDialog
{
    Q_OBJECT
public:
    explicit BibleReaderAboutDlg(QWidget *parent = 0);
    ~BibleReaderAboutDlg();

private:
    QString readDescription(QString path);
signals:

public slots:

private slots:
    void openUrl(QUrl url);
private:
    QLabel *description;
};

#endif // BIBLEREADERABOUTDLG_H
