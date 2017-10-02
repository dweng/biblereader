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

BibleTextCompareBrowser::BibleTextCompareBrowser(BibleReaderCore *brc, QWidget *parent):
    QWebView(parent)
{
    brCore = brc;

    btFontFamily = brc->getConfigurator()->getBibleTextFontFamily();
    btFontSize = brc->getConfigurator()->getBibleTextFontSize();

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

BibleTextCompareBrowser::~BibleTextCompareBrowser()
{

}

void BibleTextCompareBrowser::showComparedBibleText()
{
    QString bibleVersion;
    bibleVersion = brCore->getCurrentVersion();
    int currentBook = brCore->getCurrentBookNumber();
    int currentChapter = brCore->getCurrentChapterNumber();
    int currentVerse = brCore->getCurrentVerseNumber();
    QList<BibleBook> allBooks = brCore->getAllBooks();
    QString currentBookName = brCore->getCurrentBook(allBooks).getLongName();
    BibleChapter chapter = brCore->getChapter(bibleVersion, currentBook, currentChapter);
    // add head title
    setHtml("");

    QString html = "<head></head><body><h2 style='text-align:center; color:green;'>";
    html.append(currentBookName).append(" ").append(QString::number(currentChapter)).append("</h2>");
    html.append("<table style='border-width: 1px; border-collapse: collapse;'><tr>");

    QList<BibleInfo> bibles = brCore->getAllBibleVersions();
    QList<BibleChapter> chapters;
    for (int i = 0; i < bibles.count(); i++) {
        // bible version
        html.append("<td style='border-width: 1px; padding: 8px;border-style: solid; color:green; '>").append(bibles[i].getFullname()).append("</td>");
        chapters.push_back(brCore->getChapter(bibles[i].getVersion(), currentBook, currentChapter));
    }
    html.append("</tr>");
    for (int j = 0; j < chapter.getVersesCount(); j++) {
        html.append("<tr>");
        for (int i = 0; i < bibles.count(); i++) {
            QList<BibleVerse> verses = chapters[i].getVersesList();

            html.append("<td style='border-width: 1px; padding: 8px;border-style: solid;'>");

            BibleVerse verse = verses.value(j);

            QString verseText = verse.text();
            verseText.replace("<", "&lt;");
            verseText.replace(">", "&gt;");

            verseText.replace("&lt;", "<sup style='color:blue; font-size: 20px; font-family: Courier New, Arial;'>&lt;");
            verseText.replace("&gt;", "&gt;</sup>");

            verseText.prepend
                    ("<span style='font-size: "+QString::number(btFontSize)+"pt; font-family: "+btFontFamily+"'>");
            verseText.append("</span>");
            html.append(verseText).append("</td>");
        }
        html.append("</tr>\n");
    }

    chapters.clear();
    setHtml(html.append("</table></body>"));
}
