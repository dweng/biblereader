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

    connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(showStrongNumberDict(QUrl)));
}

BibleTextBrowser::~BibleTextBrowser()
{
    if (cursor) {
        delete cursor;
    }
}

bool BibleTextBrowser::event(QEvent *event)
{
    /*
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QTextCursor cursor = cursorForPosition(helpEvent->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        if (!cursor.selectedText().isEmpty())
            QToolTip::showText(helpEvent->globalPos(), cursor.selectedText());
        else
            QToolTip::hideText();
        return true;
    }*/

    return QTextBrowser::event(event);
}

void BibleTextBrowser::mouseMoveEvent(QMouseEvent *e)
{
    QTextCursor cursor = cursorForPosition(e->pos());
    cursor.select(QTextCursor::WordUnderCursor);
    if (!cursor.selectedText().isEmpty()) {
        QString text = cursor.selectedText();
        if (text.startsWith("H") || text.startsWith("G")) {
            //QString sn = convertSNForDict(text);
            QToolTip::showText(e->globalPos(),
                               brCore->getExplaination(
                                   QString("SNCHS"), text).replace("\\r\\n", "\r\n"));
        }

    } else {
        QToolTip::hideText();
    }

    QTextBrowser::mouseMoveEvent(e);
}

void BibleTextBrowser::mousePressEvent(QMouseEvent *e)
{
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
        addVerse(cursor, verses[i].getVerseText(), fmt.font());
        BibleTextBlockData *d = new BibleTextBlockData(
                    bv, bookId, chapterId, verses[i].getVerse());
        cursor->block().setUserData(d);
    }

    setReadOnly(true);
    highlight(brCore->getCurrentVerseNumber(), bgColor);
    if (brCore->getCurrentVerseNumber() != 1) {
        preVerseId = brCore->getCurrentVerseNumber();
    }

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
        tmpCursor->movePosition(QTextCursor::EndOfBlock);
        tmpCursor->movePosition(QTextCursor::NextBlock);
        tmpCursor->position();
        setTextCursor(*tmpCursor);
        ensureCursorVisible();

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

void BibleTextBrowser::addVerse(QTextCursor *cursor, QString verseText, QFont f)
{
    QTextCharFormat supFmt;

    supFmt.setForeground(QColor("blue"));
    supFmt.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    supFmt.setFontPointSize(btFontSize);

    // convert strong number for dict usage
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
        verseText.prepend(QString("<p style='font-family:%1; font-size:%2pt'>").arg(
                              f.family(), QString::number(f.pointSizeF())) );
        verseText.append("</p>");
    }

    cursor->insertHtml(verseText);
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

void BibleTextBrowser::showStrongNumberDict(QUrl url)
{
    LOG_INFO() << "Strong Number Clicked:";
    // br://dict/[dict name]/[item name]
    QString dictPath = url.path();
    LOG_INFO() << dictPath;

    if (url.scheme() == "br") {
        QStringList dictItemInfo = dictPath.split('/', QString::SkipEmptyParts);
        LOG_INFO() << dictItemInfo;
        brCore->fireShowDictItem(dictItemInfo[0].toUpper(), dictItemInfo[1]);
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

