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
#include "biblereaderprojectdialog.h"
#include "bibletextblockdata.h"
// for logging
#include "Logger.h"
#include "ConsoleAppender.h"

BibleTextBrowser::BibleTextBrowser(BibleReaderCore *brc, QWidget *parent) :
    QTextEdit(parent)
{
    brCore = brc;
    preVerseId = 1;

    bgColor = QColor(0xe5, 0xe5, 0xe5);
    fgColor = QColor("blue");

    cursor = NULL;
#ifdef Q_OS_WIN
    setFontFamily(QString("Microsoft YaHei"));
#endif
    setMouseTracking(true);
    setContentsMargins(2, 2, 2, 2);

    // word wrap
    setLineWrapMode(QTextEdit::WidgetWidth);
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

    return QTextEdit::event(event);
}

void BibleTextBrowser::mouseMoveEvent(QMouseEvent *e)
{
    QTextCursor cursor = cursorForPosition(e->pos());
    cursor.select(QTextCursor::WordUnderCursor);
    if (!cursor.selectedText().isEmpty()) {
        QString text = cursor.selectedText();
        if (text.startsWith(QString("W"))) {
            QString sn = "";
            int index = -1;
            if ((index = text.indexOf(QString("H"))) != -1) {
                sn = "H"+ text.mid(index+1).rightJustified(5, QChar('0'));
            } else if ((index = text.indexOf(QString("G"))) != -1) {
                sn = "G"+ text.mid(index+1).rightJustified(5, QChar('0'));;
            }
            QToolTip::showText(e->globalPos(),
                               brCore->getExplaination(
                                   QString("SNCHS"), sn).replace("\\r\\n", "\r\n"));
        }

    } else {
        QToolTip::hideText();
    }

    QTextEdit::mouseMoveEvent(e);
}

void BibleTextBrowser::mousePressEvent(QMouseEvent *e)
{
    QTextCursor cursor = cursorForPosition(e->pos());
    QTextBlock block = cursor.block();
    BibleTextBlockData *d = (BibleTextBlockData*)block.userData();
    if (d) {
        if (d->getVerse() != preVerseId     ) {
            // de hilight previous verse
            highlight(preVerseId, QColor("white"));
            // high light current verse
            highlight(d->getVerse(), bgColor);
            preVerseId = d->getVerse();

            brCore->setCurrentBookNumber(d->getBook());
            brCore->setCurrentChapterNumber(d->getChapter());
            brCore->setCurrentVerseNumber(d->getVerse());

            //block.layout()->lineAt(0).setLineWidth(this->width());
        }
    }

    QTextEdit::mousePressEvent(e);
}

void BibleTextBrowser::contextMenuEvent(QContextMenuEvent *e) {
    QMenu *menu = createStandardContextMenu();

    QTextCursor cursor = cursorForPosition(e->pos());
    QTextBlock block = cursor.block();
    BibleTextBlockData *d = (BibleTextBlockData*)block.userData();
    if (d) {
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
        QAction *compareCurrentVerse = menu->addAction(tr("Compare this verse"));
        QAction *projectCurrentVerse = menu->addAction(tr("Project this verse"));
        connect(copyCurrentVerse, SIGNAL(triggered()), this, SLOT(copyCurVerse()));
        connect(compareCurrentVerse, SIGNAL(triggered()), brCore, SLOT(fireCmpCurVerse()));
        connect(projectCurrentVerse, SIGNAL(triggered()), this, SLOT(projectVerse()));
    }
    menu->exec(e->globalPos());
    delete menu;
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
    fmt.setFontPointSize(14.0);
    fmt.setFontFamily(QString("Microsoft YaHei"));
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
        fmt.setForeground(QColor("black"));
        fmt.setFontPointSize(12.0);
        cursor->insertText(QString("%1:%2 ").arg(QString::number(chapterId),
                                                QString::number(verses[i].getVerse())), fmt);
        addVerse(cursor, verses[i].getVerseText());
        BibleTextBlockData *d = new BibleTextBlockData(
                    bv, bookId, chapterId, verses[i].getVerse());
        cursor->block().setUserData(d);
    }

    setReadOnly(true);
    highlight(brCore->getCurrentVerseNumber(), bgColor);

    return true;
}

void BibleTextBrowser::highlight(int verse, const QColor &color)
{
    QTextBlock block = getTextBlockByVerse(verse);
    if (block.isValid()) {
        QTextCursor *tmpCursor = new QTextCursor(block);
        // change block format (will set the color background)
        QTextBlockFormat blockFormat = tmpCursor->blockFormat();
        blockFormat.setBackground(color);
        blockFormat.setNonBreakableLines(true);
        blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_Auto);

        tmpCursor->setBlockFormat(blockFormat);
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

void BibleTextBrowser::addVerse(QTextCursor *cursor, QString verseText)
{
    QTextCharFormat supFmt;

    supFmt.setForeground(QColor("blue"));
    supFmt.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
    supFmt.setFontPointSize(14.0);

    verseText.replace("<", "&lt;");
    verseText.replace(">", "&gt;");

    verseText.replace("&lt;", "<sup style='color:blue; font-size: 20px; font-family: Courier New, Arial;'>&lt;");
    verseText.replace("&gt;", "&gt;</sup>");

    verseText.prepend("<span style='font-size: 12pt; font-family: Microsoft YaHei'>");
    verseText.append("</span>");

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

    BibleReaderProjectDialog *pDlg = new BibleReaderProjectDialog(this, verse.text());
    pDlg->showFullScreen();
}

