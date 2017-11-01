/**
 * Bible Reader about dialog
 *
 * \author dweng
 * \version 0.0.5
 */

#include <QFile>
#include <QGridLayout>
#include <QApplication>
#include <QDesktopServices>

#include "biblereaderaboutdlg.h"

BibleReaderAboutDlg::BibleReaderAboutDlg(QWidget *parent) :
    QDialog(parent)
{
    description = NULL;

    setFixedSize(480, 320);
    setModal(true);

    description = new QTextBrowser(this);
    description->setOpenLinks(false);
    connect(description, SIGNAL(anchorClicked(QUrl)), this, SLOT(openUrl(QUrl)));
    description->setHtml(readDescription(QString(":/data/assets/data/description.txt")).
                         append(qApp->applicationVersion()));

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

void BibleReaderAboutDlg::openUrl(QUrl url)
{
    QDesktopServices::openUrl(url);
}
