#include "biblereaderdonationdlg.h"

#include <QFile>
#include <QGridLayout>
#include <QTextEdit>

BibleReaderDonationDlg::BibleReaderDonationDlg(QWidget *parent) :
    QDialog(parent)
{
    donation = NULL;

    setFixedSize(480, 320);
    setModal(true);

    donation = new QTextEdit(this);
    donation->setReadOnly(true);
    donation->setHtml(readDonation(QString(":/data/assets/data/donation.txt")));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(donation);
    setLayout(layout);
}

BibleReaderDonationDlg::~BibleReaderDonationDlg()
{

}

QString BibleReaderDonationDlg::readDonation(QString path)
{
    QFile file(path);

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    return QString(file.readAll());
}
