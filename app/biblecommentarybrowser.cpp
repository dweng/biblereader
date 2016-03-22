#include <QMouseEvent>
#include <QUrl>
#include <QToolTip>
#include <QStringList>
#include <QTextBrowser>
#include <QApplication>

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

void BibleCommentaryBrowser::showToolTip(QString text, QPoint pos)
{
    if (!tooltip) {
        tooltip = new BibleReaderToolTip();
    }
    tooltip->setTextFont(QFont(brCore->getConfigurator()->getBibleTextFontFamily(),
                           brCore->getConfigurator()->getBibleTextFontSize()));
    tooltip->showText(pos, text, 100000);
}
