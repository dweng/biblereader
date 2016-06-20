#include "bibletextbrowser.h"
#include <QDebug>
#include <QToolTip>
#include <QTextCursor>
#include <QTextBlock>
#include <QString>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QRegExp>
#include <QPrinter>
#include <QPrintDialog>
#include "biblereaderprojectdialog.h"
#include "bibletextblockdata.h"
#include "bibleversexref.h"
// for logging
#include "Logger.h"
#include "ConsoleAppender.h"

BibleTextBrowser::BibleTextBrowser(BibleReaderCore *brc, QWidget *parent) :
    QTextBrowser(parent)
{
    brCore = brc;
    btFontFamily = brCore->getConfigurator()->getBibleTextFontFamily();
    btFontSize = brCore->getConfigurator()->getBibleTextFontSize();

    connect(brCore->getConfigurator(), SIGNAL(selectedVerseBGChanged(QColor)),
            this, SLOT(changeSelectedVerseBGColor(QColor)));
    connect(brCore->getConfigurator(), SIGNAL(bibleTextFontFamilyChanged(QString)),
            this, SLOT(changeBibleTextFontFamily(QString)));
    connect(brCore->getConfigurator(), SIGNAL(bibleTextFontSizeChanged(double)),
            this, SLOT(changeBibleTextFontSize(double)));

    preVerseId = 1;

    bgColor = brCore->getConfigurator()->getSelectedVerseBG();
    fgColor = QColor("blue");

    cursor = NULL;
#ifdef Q_OS_WIN
    setFontFamily(btFontFamily);
#endif
    setMouseTracking(true);
    setContentsMargins(2, 2, 2, 2);

    // word wrap
    setLineWrapMode(QTextEdit::WidgetWidth);

    // disable open link
    setOpenLinks(false);

    connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(navTo(QUrl)));
}

BibleTextBrowser::~BibleTextBrowser()
{
    if (cursor) {
        delete cursor;
    }
}

bool BibleTextBrowser::event(QEvent *event)
{
    return QTextBrowser::event(event);
}

void BibleTextBrowser::mouseMoveEvent(QMouseEvent *e)
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

void BibleTextBrowser::mousePressEvent(QMouseEvent *e)
{
    // block anchorClicked signal
    //blockSignals(true);

    QTextCursor cursor = cursorForPosition(e->pos());
    QTextBlock block = cursor.block();
    BibleTextBlockData *d = (BibleTextBlockData*)block.userData();
    if (d && d->getVerse() != 0) {
        if (d->getVerse() != preVerseId ) {
            // de hilight previous verse
            highlight(preVerseId, QColor("white"));
            // high light current verse
            highlight(d->getVerse(), bgColor);
            preVerseId = d->getVerse();

            brCore->setCurrentBCV(d->getBook(),
                                  d->getChapter(),
                                  d->getVerse());

            //block.layout()->lineAt(0).setLineWidth(this->width());
        }
    }
    QTextBrowser::mousePressEvent(e);

    // reset signal
    //blockSignals(false);
}

void BibleTextBrowser::wheelEvent(QWheelEvent *e)
{
    int numDegrees = e->delta() / 8;
    int numSteps = numDegrees / 15;

    QTextBrowser::wheelEvent(e);
}

void BibleTextBrowser::contextMenuEvent(QContextMenuEvent *e) {
    QMenu *menu = createStandardContextMenu(e->pos());

    QTextCursor cursor = cursorForPosition(e->pos());
    QTextBlock block = cursor.block();
    BibleTextBlockData *d = (BibleTextBlockData*)block.userData();
    if (d && d->getVerse() != 0) {
        if (d->getVerse() != preVerseId) {
            // de hilight previous verse
            highlight(preVerseId, QColor("white"));

            highlight(d->getVerse(), bgColor);
            preVerseId = d->getVerse();

            brCore->setCurrentBookNumber(d->getBook());
            brCore->setCurrentChapterNumber(d->getChapter());
            brCore->setCurrentVerseNumber(d->getVerse());
        }
        QAction *copyCurrentVerse = menu->addAction(tr("Copy this verse"));
        QAction *copyCurrentVerseHeader = menu->addAction(tr("Copy this verse header"));
        QAction *copyCurrentVerseLongHeader = menu->addAction(tr("Copy this verse with long book name"));

        // add sub menu
        QMenu *copyTo = new QMenu(tr("Copy verses from current verse to..."), menu);
        BibleChapter bc = brCore->getChapter(bibleVersion, d->getBook(), d->getChapter());
        for (int i = d->getVerse(); i <= bc.getVersesCount(); i++) {
            QAction *tmp = copyTo->addAction(QString::number(i, 10));
            tmp->setData(QVariant(i));
            connect(tmp, SIGNAL(triggered(bool)), this, SLOT(copyVerseTo()));
        }
        menu->addMenu(copyTo);
        QAction *compareCurrentVerse = menu->addAction(tr("Compare this verse"));
        QAction *projectCurrentVerse = menu->addAction(tr("Project this verse"));

        if (this->textCursor().hasSelection()) {
            QString q = this->textCursor().selectedText();
            QString menuTxt = tr("Search \"%1\"");
            QAction *searchText = menu->addAction(menuTxt.arg(q));
            searchText->setData(q);
            connect(searchText, SIGNAL(triggered(bool)), this, SLOT(searchBible()));
        }

        //add print function
        QAction *printAct = menu->addAction(tr("Print..."));
        connect(printAct, SIGNAL(triggered(bool)), this, SLOT(printBibleText()));


        connect(copyCurrentVerse, SIGNAL(triggered()), this, SLOT(copyCurVerse()));
        connect(copyCurrentVerseHeader, SIGNAL(triggered()), this, SLOT(copyCurVerseHeader()));
        connect(copyCurrentVerseLongHeader, SIGNAL(triggered(bool)), this, SLOT(copyCurVerseLongHeader()));
        connect(compareCurrentVerse, SIGNAL(triggered()), brCore, SLOT(fireCmpCurVerse()));
        connect(projectCurrentVerse, SIGNAL(triggered()), this, SLOT(projectVerse()));
    }
    menu->exec(e->globalPos());
    delete menu;
}

void BibleTextBrowser::keyPressEvent(QKeyEvent *e)
{
    BibleChapter chapter = brCore->getChapter(
                brCore->getCurrentBookNumber(),
                brCore->getCurrentChapterNumber());
    switch (e->key()) {
    case Qt::Key_Up:
        if (preVerseId > 1) {
            // de hilight previous verse
            highlight(preVerseId, QColor("white"));
            // high light current verse
            highlight(preVerseId-1, bgColor);
            preVerseId--;

            brCore->setCurrentBCV(brCore->getCurrentBookNumber(),
                                  brCore->getCurrentChapterNumber(),
                                  preVerseId);
        }
        break;

    case Qt::Key_Down:
        if (preVerseId < chapter.getVersesCount()) {
            // de hilight previous verse
            highlight(preVerseId, QColor("white"));
            // high light current verse
            highlight(preVerseId+1, bgColor);
            preVerseId++;

            brCore->setCurrentBCV(brCore->getCurrentBookNumber(),
                                  brCore->getCurrentChapterNumber(),
                                  preVerseId);
        }
        break;
    default:
        break;
    }

    //QTextEdit::keyPressEvent(e);
}

void BibleTextBrowser::mouseReleaseEvent(QMouseEvent *e)
{
    QTextBrowser::mouseReleaseEvent(e);
}


QString BibleTextBrowser::getBibleVersion() const
{
    return bibleVersion;
}

void BibleTextBrowser::setBibleVersion(const QString &value)
{
    bibleVersion = value;
}

bool BibleTextBrowser::showCurrentChapter()
{
    // clear all bible text
    clear();

    cursor = new QTextCursor(this->document());
    QTextCharFormat fmt;
    QTextBlockFormat bfmt;

    BibleChapter chapter = brCore->getChapter(bibleVersion,
                    brCore->getCurrentBookNumber(),
                    brCore->getCurrentChapterNumber()
                );
    QList<BibleVerse> verses = chapter.getVersesList();
    QList<BibleVerseXref> xrefs = brCore->getXrefsByChapter(brCore->getCurrentBookNumber(),
                                                            brCore->getCurrentChapterNumber());

    for (int i = 0; i < verses.size(); i++) {
        for (int j = 0; j < xrefs.size(); j++) {
            if (xrefs[j].getFbi() == verses[i].getBookNumber() &&
                    xrefs[j].getFci() == verses[i].getChapter() &&
                    xrefs[j].getFvi() == verses[i].getVerse()) {
                verses[i].addXref(xrefs[j]);
            }
        }
        LOG_INFO() << "xrefs: " << verses[i].getXrefs().size();
    }

    int chapterId = chapter.getChapter();
    int bookId = chapter.getBook().getBookNumber();
    QString bv = chapter.getBibleVersion();

    // add book name and chapter number
    fmt.setForeground(QColor("green"));
    fmt.setFontPointSize(btFontSize+6.0);
    fmt.setFontFamily(btFontFamily);
    cursor->insertText(chapter.getBook().getLongName(), fmt);
    cursor->insertText(QString(" "), fmt);
    cursor->insertText(QString::number(chapter.getChapter()), fmt);
    bfmt.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(bfmt);
    BibleTextBlockData *td = new BibleTextBlockData(
                bv, bookId, chapterId, 0);
    cursor->block().setUserData(td);

    for (int i = 0; i < verses.count(); i++) {
        cursor->insertBlock();
        bfmt.setAlignment(Qt::AlignLeft);
        cursor->setBlockFormat(bfmt);
        // verse number format
        fmt.setForeground(QColor("blue"));
        fmt.setFontPointSize(btFontSize);
        fmt.setFontFamily(btFontFamily);
        cursor->insertText(QString("%1:%2 ").arg(QString::number(chapterId),
                                                QString::number(verses[i].getVerse())), fmt);
        // verse format
        addVerse(cursor, verses[i], fmt.font());
        BibleTextBlockData *d = new BibleTextBlockData(
                    bv, bookId, chapterId, verses[i].getVerse());
        cursor->block().setUserData(d);
    }

    highlight(brCore->getCurrentVerseNumber(), bgColor);
    preVerseId = brCore->getCurrentVerseNumber();

    return true;
}

void BibleTextBrowser::highlight(int verse, const QColor &color)
{
    QTextBlock block = getTextBlockByVerse(verse);
    if (block.isValid()) {
        QTextCursor *tmpCursor = new QTextCursor(block);
        // change block format (will set the color background)
        QTextBlockFormat format = tmpCursor->blockFormat();
        format.setBackground(color);

        tmpCursor->setBlockFormat(format);
        // move 3 times to make sure the cursor visible
        for (int i = 0; i < 2; i++) {
            tmpCursor->movePosition(QTextCursor::EndOfBlock);
            tmpCursor->movePosition(QTextCursor::NextBlock);
        }
        setTextCursor(*tmpCursor);

        delete tmpCursor;
    }
    // change font for current block's fragments
    /*
    for (QTextBlock::iterator it = cursor.block().begin(); !(it.atEnd()); ++it)
    {
        LOG_INFO() << it.fragment().text();
        QTextCharFormat charFormat = it.fragment().charFormat();
        //charFormat.setFont(QFont("Times", 15, QFont::Bold));

        QTextCursor tempCursor = cursor;
        tempCursor.setPosition(it.fragment().position());
        tempCursor.setPosition(it.fragment().position() + it.fragment().length(), QTextCursor::KeepAnchor);
        tempCursor.setCharFormat(charFormat);
    }
    */

}

void BibleTextBrowser::addVerse(QTextCursor *cursor, BibleVerse verse, QFont f)
{
    QTextCharFormat supFmt;

    supFmt.setForeground(QColor("blue"));
    supFmt.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    supFmt.setFontPointSize(btFontSize);

    // convert strong number for dict usage
    QString wholeVerseString = "";
    QString verseText = verse.getVerseText();
    QStringList SNs = verseText.split(QRegExp("[<>]"), QString::SkipEmptyParts);
    if (!SNs.isEmpty()) {
        for (int i = 0; i < SNs.size(); i++) {
            if (SNs[i].startsWith("W")) {
                // example:
                // <WH00003> -> <a href="br://dict/snchs/H00003"><H00003></a>
                QString newSN("<a href='br://dict/snchs/%1'><sup>&lt;%2&gt;</sup></a>");
                QString sn = convertSNForDict(SNs[i]);
                newSN = newSN.arg(sn, sn);

                verseText.replace("<"+SNs[i]+">", newSN);
            }
        }
        /*
        verseText.replace("<", "&lt;");
        verseText.replace(">", "&gt;");
        verseText.replace("&lt;", "<a href='br://dict/snchs'><sup>&lt;");
        verseText.replace("&gt;", "&gt;</sup></a>");
        */
    }

    wholeVerseString = verseText;
    QList<BibleVerseXref> xrefs = verse.getXrefs();
    QString xrefStr = "";
    int tbi, tci, tvi, tspan;
    QString tbname, tmp;
    for (int i = 0; i < xrefs.size(); i++) {
        tbi = xrefs[i].getTbi();
        tci = xrefs[i].getTci();
        tvi = xrefs[i].getTvi();
        tspan = xrefs[i].getTspan();
        tbname = xrefs[i].getTbname();
        tmp = "";
        if (xrefs[i].getTspan() > 0) {
            tmp.sprintf
                    ("<sub><a href=\"br://bible/%d/%d/%d/%d/%d/%d\">%s %d:%d-%d;</a> </sub>",
                     tbi, tci, tvi, tbi, tci, tvi+tspan, tbname.toStdString().c_str(), tci, tvi, tvi+tspan);
            xrefStr += tmp;
        } else {
            tmp.sprintf
                    ("<sub><a href=\"br://bible/%d/%d/%d\">%s %d:%d;</a> </sub>",
                     tbi, tci, tvi, tbname.toStdString().c_str(), tci, tvi);
            xrefStr += tmp;
        }
    }
    LOG_INFO() << verse.getVerse() <<":"<< xrefStr;
    wholeVerseString += xrefStr;
    wholeVerseString.prepend(QString("<p style='font-family:%1; font-size:%2pt'>").arg(
                          f.family(), QString::number(f.pointSizeF())) );
    wholeVerseString.append("</p>");
    cursor->insertHtml(wholeVerseString);
}

QTextBlock BibleTextBrowser::getTextBlockByVerse(int verse)
{
    QTextDocument *doc = document();
    for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
        BibleTextBlockData *d = (BibleTextBlockData *)it.userData();
        if (d->getVerse() == verse) {
            return it;
        }
    }
    return QTextBlock();
}

QString BibleTextBrowser::convertSNForDict(QString oldSN)
{
    QString sn = "";
    int index = -1;
    if ((index = oldSN.indexOf(QString("H"))) != -1) {
        sn = "H"+ oldSN.mid(index+1).rightJustified(5, QChar('0'));
    } else if ((index = oldSN.indexOf(QString("G"))) != -1) {
        sn = "G"+ oldSN.mid(index+1).rightJustified(5, QChar('0'));;
    }

    return sn;
}

bool BibleTextBrowser::copyCurVerseHeader()
{
    QClipboard *cb = QApplication::clipboard();
    BibleVerse verse = brCore->getVerse(
                brCore->getCurrentVersion(),
                brCore->getCurrentBookNumber(),
                brCore->getCurrentChapterNumber(),
                brCore->getCurrentVerseNumber());
    cb->setText(verse.header(), QClipboard::Clipboard);
    return true;
}

bool BibleTextBrowser::copyCurVerse()
{
    QClipboard *cb = QApplication::clipboard();
    BibleVerse verse = brCore->getVerse(
                brCore->getCurrentVersion(),
                brCore->getCurrentBookNumber(),
                brCore->getCurrentChapterNumber(),
                brCore->getCurrentVerseNumber());
    cb->setText(verse.text(), QClipboard::Clipboard);

    return true;
}

bool BibleTextBrowser::copyCurVerseLongHeader()
{
    QClipboard *cb = QApplication::clipboard();
    BibleVerse verse = brCore->getVerse(
                brCore->getCurrentVersion(),
                brCore->getCurrentBookNumber(),
                brCore->getCurrentChapterNumber(),
                brCore->getCurrentVerseNumber());
    cb->setText(verse.textLongHeader(), QClipboard::Clipboard);

    return true;
}

void BibleTextBrowser::projectVerse()
{
    BibleVerse verse = brCore->getVerse(
                brCore->getCurrentVersion(),
                brCore->getCurrentBookNumber(),
                brCore->getCurrentChapterNumber(),
                brCore->getCurrentVerseNumber());

    BibleReaderProjectDialog *pDlg = new BibleReaderProjectDialog(brCore, verse.text(), this);
    pDlg->setBgColor(QColor(brCore->getConfigurator()->getProjectDlgBG()));
    pDlg->setFgColor(QColor(brCore->getConfigurator()->getProjectDlgFG()));
    pDlg->showVerseText();
    pDlg->showFullScreen();
}

void BibleTextBrowser::copyVerseTo()
{
    QAction *tmp = qobject_cast<QAction *>(sender());
    int cbn = brCore->getCurrentBookNumber();
    int ccn = brCore->getCurrentChapterNumber();
    BibleVersePos begin = BibleVersePos(cbn, ccn, preVerseId);
    BibleVersePos end = BibleVersePos(cbn, ccn, tmp->data().toInt());

    QString bt = brCore->getVerses(begin, end);

    QClipboard *cb = QApplication::clipboard();
    cb->setText(bt, QClipboard::Clipboard);
}

void BibleTextBrowser::changeSelectedVerseBGColor(QColor nc)
{
    bgColor = nc;
    highlight(preVerseId, bgColor);
}

void BibleTextBrowser::changeBibleTextFontFamily(QString family)
{
    btFontFamily = family;
    showCurrentChapter();
}

void BibleTextBrowser::changeBibleTextFontSize(double size)
{
    btFontSize = size;
    showCurrentChapter();
}

void BibleTextBrowser::navTo(QUrl brUrl)
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

void BibleTextBrowser::searchBible()
{
    QAction *m = qobject_cast<QAction *>(sender());

    brCore->fireSearchRequest(m->data().toString());
}

void BibleTextBrowser::printBibleText()
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

