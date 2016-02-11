#ifndef BIBLEREADERPROJECTDIALOG_H
#define BIBLEREADERPROJECTDIALOG_H

#include <QTextEdit>
#include <QDialog>
#include "biblereadercore.h"

class BibleReaderProjectDialog : public QDialog
{
    Q_OBJECT
public:
    BibleReaderProjectDialog(BibleReaderCore *brc, QString text, QWidget *parent = 0);
    ~BibleReaderProjectDialog();

    QColor getBgColor() const;
    QColor getFgColor() const;


public slots:
    void setBgColor(const QColor value);
    void setFgColor(const QColor value);

private:
    QTextEdit *panel;
    QString versesText;

    QColor bgColor;
    QColor fgColor;
    QString bgColorQSS;

    BibleReaderCore *brCore;

public:
    QString showVerseText();
};

#endif // BIBLEREADERPROJECTDIALOG_H
