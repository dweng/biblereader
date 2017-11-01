/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 *
 * \author dweng
 * \version 0.0.5
 * \date 2017-11-1
 */
#include <QMouseEvent>
#include <QUrl>
#include <QToolTip>
#include <QStringList>
#include <QTextBrowser>
#include <QApplication>
#include <QMenu>
#include <QPrinter>
#include <QPrintDialog>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "biblecommentarybrowser.h"
#include "bibleversepos.h"
#include "ConsoleAppender.h"
#include "Logger.h"

BibleCommentaryBrowser::BibleCommentaryBrowser(BibleReaderCore *brc, QWidget *parent)
    :QTextBrowser(parent)
{
    tooltip = NULL;
    shiftPressed = false;
    brCore = brc;
    // disable open link
    setOpenLinks(false);

    connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(setCurrentBCV(QUrl)));
}

BibleCommentaryBrowser::~BibleCommentaryBrowser()
{
    if (tooltip) {
        delete tooltip;
    }
}

void BibleCommentaryBrowser::mouseMoveEvent(QMouseEvent *e)
{
    QString url = anchorAt(e->pos());
    if (!url.isEmpty()) {
        QUrl brUrl = QUrl(url);
        if (brUrl.scheme() == "br") {
            if (brUrl.host() == "bible") {
                QStringList verseInfo = brUrl.path().split('/', QString::SkipEmptyParts);
                if (verseInfo.size() == 3) {
                    QString verseText = brCore->getVerse(
                                verseInfo[0].toInt(),
                                verseInfo[1].toInt(),
                                verseInfo[2].toInt()
                                ).text();
                    showToolTip(verseText, e->globalPos());

                } else if (verseInfo.size() == 6) {
                    BibleVersePos start = BibleVersePos(
                                verseInfo[0].toInt(),
                                verseInfo[1].toInt(),
                                verseInfo[2].toInt()
                                );
                    BibleVersePos end = BibleVersePos(
                                verseInfo[3].toInt(),
                                verseInfo[4].toInt(),
                                verseInfo[5].toInt()
                                );
                    QString versesText = brCore->getVerses(start, end);
                    showToolTip(versesText, e->globalPos());

                } else {
                    // do nothing
                    ;
                }
            }
        }
    } else {
        if (tooltip && !shiftPressed) {
            tooltip->hide();
        }
    }
    QTextBrowser::mouseMoveEvent(e);
}

void BibleCommentaryBrowser::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift) {
        shiftPressed = true;
    }
    QTextBrowser::keyPressEvent(e);
}

void BibleCommentaryBrowser::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Shift) {
        shiftPressed = false;
    }
    QTextBrowser::keyReleaseEvent(e);
}

void BibleCommentaryBrowser::enterEvent(QEvent *e)
{
    if (!hasFocus()) {
        setFocus();
    }
    QTextBrowser::enterEvent(e);
}

void BibleCommentaryBrowser::leaveEvent(QEvent *e)
{
    if (hasFocus()) {
        shiftPressed = false;
        clearFocus();
    }
    QTextBrowser::leaveEvent(e);
}

void BibleCommentaryBrowser::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu *menu = createStandardContextMenu();

    QAction *prtAct = menu->addAction(QIcon(":/img/assets/images/printer.png"), tr("Print"));
    connect(prtAct, SIGNAL(triggered(bool)), this, SLOT(printCmt()));

    QAction *saveAct = menu->addAction(QIcon(":/img/assets/images/disk.png"), tr("Save"));
    connect(saveAct, SIGNAL(triggered(bool)), this, SLOT(saveCmt()));

    menu->exec(e->globalPos());
}

void BibleCommentaryBrowser::setCurrentBCV(QUrl url)
{
    if (url.scheme() == "br") {
        if (url.host() == "bible") {
            QStringList verseInfo = url.path().split('/', QString::SkipEmptyParts);
            int b = verseInfo[0].toInt();
            int c = verseInfo[1].toInt();
            int v = verseInfo[2].toInt();
            brCore->setCurrentBCV(b, c, v);
        }
    }
}

void BibleCommentaryBrowser::printCmt()
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (this->textCursor().hasSelection())
       dialog->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (dialog->exec() != QDialog::Accepted)
       return;

    this->print(&printer);
}

void BibleCommentaryBrowser::saveCmt()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Commentary"), ".", tr("Word Files(*.doc *.docx)"));
    if(path.length() == 0) {
        return;
    } else {
        QFile f(path);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            f.write(this->toHtml().toUtf8());
            f.close();
        }
    }
}

void BibleCommentaryBrowser::showToolTip(QString text, QPoint pos)
{
    if (!tooltip) {
        tooltip = new BibleReaderToolTip();
    }
    tooltip->setTextFont(QFont(brCore->getConfigurator()->getBibleTextFontFamily(),
                           brCore->getConfigurator()->getBibleTextFontSize()));
    tooltip->showText(pos, text, 100000);
}
