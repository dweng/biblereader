#include "biblereaderprojectdialog.h"
#include <QGridLayout>
#include <QTextCharFormat>

BibleReaderProjectDialog::BibleReaderProjectDialog(QWidget *parent, QString text)
    :QDialog(parent)
{
    setWindowFlags(Qt::Window);
    //setStyleSheet();
    panel = new QTextEdit(this);

    // set font and color
    QTextCharFormat fmt;
    fmt.setFontPointSize(30);
    fmt.setForeground(QColor("yellow"));
    QTextCursor cursor = panel->textCursor();
    cursor.mergeCharFormat(fmt);
    panel->mergeCurrentCharFormat(fmt);

    //panel->setReadOnly(true);
    versesText = text;

    panel->setText(setFormat());

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(panel);
    setLayout(layout);

}

BibleReaderProjectDialog::~BibleReaderProjectDialog()
{
    delete panel;
}

QString BibleReaderProjectDialog::setFormat()
{
    return "<span style='font-size: 30pt; color: yellow;'>" + versesText + "</span>";
}

