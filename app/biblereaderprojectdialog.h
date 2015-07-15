#ifndef BIBLEREADERPROJECTDIALOG_H
#define BIBLEREADERPROJECTDIALOG_H

#include <QTextEdit>
#include <QDialog>

class BibleReaderProjectDialog : public QDialog
{
public:
    BibleReaderProjectDialog(QWidget *parent, QString text);
    ~BibleReaderProjectDialog();

private:
    QTextEdit *panel;
    QString versesText;

private:
    QString setFormat();
};

#endif // BIBLEREADERPROJECTDIALOG_H
