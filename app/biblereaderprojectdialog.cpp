#include "biblereaderprojectdialog.h"
#include <QGridLayout>
#include <QTextCharFormat>
#include "Logger.h"
#include "ConsoleAppender.h"

BibleReaderProjectDialog::BibleReaderProjectDialog(BibleReaderCore *brc,
                                                   QString text,
                                                   QWidget *parent)
    :QDialog(parent)
{
    LOG_INFO() << "project dlg creating";
    brCore = brc;
    connect(brCore->getConfigurator(), SIGNAL(projectDlgBGChanged(QColor)), this,
            SLOT(setBgColor(QColor)));
    connect(brCore->getConfigurator(), SIGNAL(projectDlgFGChanged(QColor)), this,
            SLOT(setFgColor(QColor)));

    setWindowFlags(Qt::Window);
    bgColorQSS = "QTextEdit {background-color: %1;}";

    panel = new QTextEdit(this);
    panel->setWindowFlags(Qt::FramelessWindowHint);
    panel->setReadOnly(true);

    // set font and color
    /*
    QTextCharFormat fmt;
    fmt.setFontPointSize(30);
    fmt.setForeground(QColor("yellow"));
    QTextCursor cursor = panel->textCursor();
    cursor.mergeCharFormat(fmt);
    panel->mergeCurrentCharFormat(fmt);
    */

    versesText = text;

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(panel);
    setLayout(layout);

}

BibleReaderProjectDialog::~BibleReaderProjectDialog()
{
    LOG_INFO() << "project dlg destorying";
    delete panel;
}

QColor BibleReaderProjectDialog::getBgColor() const
{
    return bgColor;
}

void BibleReaderProjectDialog::setBgColor(const QColor value)
{
    bgColor = value;

    setStyleSheet(bgColorQSS.arg(value.name()));
}

QColor BibleReaderProjectDialog::getFgColor() const
{
    return fgColor;
}

void BibleReaderProjectDialog::setFgColor(const QColor value)
{
    fgColor = value;
}

QString BibleReaderProjectDialog::showVerseText()
{
    QString t = QString("<span style='font-size: 30pt; color: %1;'>%2</span>");
    panel->setText(t.arg(fgColor.name(), versesText));

    return t;
}

