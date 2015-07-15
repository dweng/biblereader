#include "biblereaderprojectdialog.h"
#include <QGridLayout>
BibleReaderProjectDialog::BibleReaderProjectDialog(QWidget *parent, QString text)
    :QDialog(parent)
{
    setWindowFlags(Qt::Window);
    setStyleSheet("background-color: #000");
    panel = new QTextEdit(this);
    panel->setReadOnly(true);
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

