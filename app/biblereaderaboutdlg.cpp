#include <QFile>
#include <QGridLayout>
#include "biblereaderaboutdlg.h"

BibleReaderAboutDlg::BibleReaderAboutDlg(QWidget *parent) :
    QDialog(parent)
{
    description = NULL;

    setFixedSize(480, 320);
    setModal(true);

    description = new QTextEdit(this);
    description->setReadOnly(true);
    description->setHtml(readDescription(QString(":/data/assets/data/description.txt")));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(description);
    setLayout(layout);
}

BibleReaderAboutDlg::~BibleReaderAboutDlg()
{
    delete description;
}

QString BibleReaderAboutDlg::readDescription(QString path)
{
    QFile file(path);

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    return QString(file.readAll());
}
