#include "bibledictbrowser.h"
#include <QMouseEvent>
#include <QToolTip>
#include "Logger.h"

BibleDictBrowser::BibleDictBrowser(BibleReaderCore *bibleReaderCore, QWidget *parent = 0):
    QTextBrowser(parent)
{
    brCore = bibleReaderCore;

    connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(navTo(QUrl)));
}

void BibleDictBrowser::mouseMoveEvent(QMouseEvent *e)
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
                    QToolTip::showText(e->globalPos(), verseText, this, rect(), 100000);

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
                    QToolTip::showText(e->globalPos(), versesText, this, rect(), 100000);

                } else {
                    // do nothing
                    ;
                }
            } else if (brUrl.host() == "dict"){
                LOG_INFO() << brUrl.path();
                QString dictPath = brUrl.path();
                QStringList dictItemInfo = dictPath.split('/', QString::SkipEmptyParts);
                QString dict = dictItemInfo[0].toUpper();
                QString text = dictItemInfo[1].toUpper();
                if (dict == "SNCHS") {
                    if (text.startsWith("H") || text.startsWith("G")) {
                        QToolTip::showText(e->globalPos(),
                                           brCore->getExplaination(
                                           QString("SNCHS"), text).replace("\\r\\n", "\r\n"),
                                           this, rect(), 100000);
                    }
                }
            }
        }
    } else {
        QToolTip::hideText();
    }
    QTextBrowser::mouseMoveEvent(e);
}

void BibleDictBrowser::navTo(QUrl brUrl)
{
    if (brUrl.scheme() == "br") {
        if (brUrl.host() == "bible") {
            QStringList verseInfo = brUrl.path().split('/', QString::SkipEmptyParts);
            brCore->setCurrentBCV(
                        verseInfo[0].toInt(),
                        verseInfo[1].toInt(),
                        verseInfo[2].toInt());
        } else if (brUrl.host() == "dict") {
            QString dictPath = brUrl.path();
            QStringList dictItemInfo = dictPath.split('/', QString::SkipEmptyParts);
            brCore->fireShowDictItem(dictItemInfo[0].toUpper(), dictItemInfo[1]);
        }
    }
}

