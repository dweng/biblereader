/**
 * [1 Timothy 3:16-17 ESV]
 * All Scripture is breathed out by God and profitable
 * for teaching, for reproof, for correction, and for training in righteousness,
 * that the man of God may be complete, equipped for every good work.
 *
 * BibleReader is an simple application for you to study God's Word.
 * The main user of this application maybe the pastor who's main language
 * is Chinese Simplified.
 *
 * You can use it totally free, and if you want to get source code of
 * this application, please email me at dweng123@gmail to get the source
 * code.
 *
 */
#include "bibletextcomparebrowser.h"
#include "bibletextblockdata.h"
#include <QTextTable>
#include <QTextTableCell>

BibleTextCompareBrowser::BibleTextCompareBrowser(BibleReaderCore *brc, QWidget *parent):
    QTextEdit(parent)
{
    brCore = brc;
}

BibleTextCompareBrowser::~BibleTextCompareBrowser()
{

}

void BibleTextCompareBrowser::showComparedBibleText()
{
    QString bibleVersion;
    QString bibleFullName;
    int currentBook = brCore->getCurrentBookNumber();
    int currentChapter = brCore->getCurrentChapterNumber();
    int currentVerse = brCore->getCurrentVerseNumber();
    QList<BibleBook> allBooks = brCore->getAllBooks();
    QString currentBookName = brCore->getCurrentBook(allBooks).getLongName();

    // add head title
    clear();

    QTextCursor *cursor = new QTextCursor(this->document());
    QTextCharFormat fmt;
    QTextBlockFormat bfmt;
    QTextTableFormat tfmt;
    fmt.setForeground(QColor("green"));
    fmt.setFontPointSize(16.0);
    fmt.setFontFamily(QString("Microsoft YaHei"));
    cursor->insertText(tr("Compare verse "), fmt);
    cursor->insertText(QString("["), fmt);
    cursor->insertText(currentBookName, fmt);
    cursor->insertText(QString::number(currentChapter), fmt);
    cursor->insertText(QString(":"), fmt);
    cursor->insertText(QString::number(currentVerse), fmt);
    cursor->insertText(QString("]"), fmt);
    bfmt.setAlignment(Qt::AlignCenter);
    cursor->setBlockFormat(bfmt);

    QList<BibleInfo> bibles = brCore->getAllBibleVersions();
    tfmt.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
    QTextTable *cmpTable = cursor->insertTable(bibles.count(), 2, tfmt);
    for (int i = 0; i < bibles.count(); i++) {
        bibleVersion = bibles[i].getVersion();
        bibleFullName = bibles[i].getFullname();

        BibleVerse verse = brCore->getVerse(bibleVersion,
                        currentBook, currentChapter, currentVerse
                    );
        fmt.setFontPointSize(12.0);
        fmt.setForeground(QColor("black"));
        // bible version
        QTextTableCell tc = cmpTable->cellAt(i, 0);
        QTextCursor tmp = tc.firstCursorPosition();
        tmp.insertText(bibleFullName, fmt);
        // verse text
        tc = cmpTable->cellAt(i, 1);
        tmp = tc.firstCursorPosition();
        tmp.insertText(verse.text(), fmt);

        BibleTextBlockData *d = new BibleTextBlockData(
                    bibleVersion, currentBook, currentChapter, currentVerse);
        cursor->block().setUserData(d);
    }
    /*
    for (int i = 0; i < bibles.count(); i++) {
        bibleVersion = bibles[i].getVersion();

        BibleVerse verse = brCore->getVerse(bibleVersion,
                        currentBook, currentChapter, currentVerse
                    );

        cursor->insertBlock();
        bfmt.setAlignment(Qt::AlignLeft);
        bfmt.setTopMargin(3.0);
        bfmt.setBottomMargin(3.0);
        cursor->setBlockFormat(bfmt);
        fmt.setForeground(QColor("black"));
        fmt.setFontPointSize(14.0);
        cursor->insertText(bibleVersion, fmt);
        cursor->insertText(QString(" "), fmt);
        cursor->insertText(verse.getBookName(), fmt);
        cursor->insertText(QString(" "), fmt);
        cursor->insertText(verse.text(), fmt);
        BibleTextBlockData *d = new BibleTextBlockData(
                    bibleVersion, currentBook, currentChapter, currentVerse);
        cursor->block().setUserData(d);
    }
    */

    setReadOnly(true);
}
