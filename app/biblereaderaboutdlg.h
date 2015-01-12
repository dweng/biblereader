#ifndef BIBLEREADERABOUTDLG_H
#define BIBLEREADERABOUTDLG_H

#include <QDialog>
#include <QTextEdit>
#include <QString>

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

private:
    QTextEdit *description;
};

#endif // BIBLEREADERABOUTDLG_H
