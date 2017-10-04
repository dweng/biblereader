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
#include <QWebFrame>
#include "bibletextcomparebrowser.h"
#include "bibletextblockdata.h"

BibleTextCompareBrowser::BibleTextCompareBrowser(BibleReaderCore *brc, QWidget *parent):
    QWebView(parent)
{
    brCore = brc;

    btFontFamily = brc->getConfigurator()->getBibleTextFontFamily();
    btFontSize = brc->getConfigurator()->getBibleTextFontSize();

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(this->page()->mainFrame(), SIGNAL( javaScriptWindowObjectCleared()),
            this, SLOT(addBrCoreToJS()));
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

    QString html = "";
    html.append("<head>");
    html.append("<link rel=\"stylesheet\" type=\"text/css\" href=\"qrc:/others/assets/others/bootstrap-3.3.7-dist/css/bootstrap.min.css\" />");
    html.append("<link rel=\"stylesheet\" type=\"text/css\" href=\"qrc:/others/assets/others/style.css\" />");
    html.append("<script type=\"text/javascript\" src=\"qrc:/others/assets/others/jquery-3.2.1.min.js\"></script>");
    html.append("<script type=\"text/javascript\" src=\"qrc:/others/assets/others/bootstrap-3.3.7-dist/js/bootstrap.min.js\"></script>");
    html.append("<script type=\"text/javascript\" src=\"qrc:/others/assets/others/script.js\"></script>");
    html.append("</head><body>");
    html.append("<h2>").append(currentBookName).append(" ").append(QString::number(currentChapter)).append("</h2>");
    html.append("<p><button type=\"button\" class=\"btn btn-lg btn-primary prevbook\">");
    html.append("<span class=\"glyphicon glyphicon-chevron-left\" aria-hidden=\"true\"></span> 上一卷");
    html.append("</button>");
    html.append("<button type=\"button\" class=\"btn btn-lg btn-primary nextbook\">");
    html.append("<span class=\"glyphicon glyphicon-chevron-right\" aria-hidden=\"true\"></span> 下一卷");
    html.append("</button>");
    html.append("<button type=\"button\" class=\"btn btn-lg btn-success prevchapter\">");
    html.append("<span class=\"glyphicon glyphicon-chevron-left\" aria-hidden=\"true\"></span> 上一章");
    html.append("</button>");
    html.append("<button type=\"button\" class=\"btn btn-lg btn-success nextchapter\">");
    html.append("<span class=\"glyphicon glyphicon-chevron-right\" aria-hidden=\"true\"></span> 下一章");
    html.append("</button></p>");

    html.append("<p>");
    QList<BibleInfo> bibles = brCore->getAllBibleVersions();
    for (int i = 0; i < bibles.count(); i++) {
        html.append(
                    QString("<button type=\"button\" class=\"btn btn-sm btn-success active bible\" value='%1'>").arg(bibles[i].getVersion())
                    );
        html.append(bibles[i].getFullname());
        html.append("</button>");
    }
    html.append("</p>");
    html.append("<div><table class='table table-bordered table-hover'><thead><tr>");


    QList<BibleChapter> chapters;
    for (int i = 0; i < bibles.count(); i++) {
        // bible version
        html.append(QString("<th nowrap=\"nowrap\" value='%1'>%2</th>").arg
                    (bibles[i].getVersion(),
                     bibles[i].getFullname()));
        chapters.push_back(brCore->getChapter(bibles[i].getVersion(), currentBook, currentChapter));
    }
    html.append("</tr></thead><tbody>");
    for (int j = 0; j < chapter.getVersesCount(); j++) {
        html.append("<tr>");
        for (int i = 0; i < bibles.count(); i++) {
            QList<BibleVerse> verses = chapters[i].getVersesList();

            if (currentVerse == j+1) {
                html.append(QString("<td class='success' value='%1'>").arg(bibles[i].getVersion()));
            } else {
                html.append(QString("<td value='%1'>").arg(bibles[i].getVersion()));
            }

            BibleVerse verse = verses.value(j);

            QString verseText = verse.text();
            verseText.replace("<", "&lt;");
            verseText.replace(">", "&gt;");

            verseText.replace("&lt;", "<sup>&lt;");
            verseText.replace("&gt;", "&gt;</sup>");

            verseText.prepend("<span>");
            verseText.append("</span>");
            html.append(verseText).append("</td>");
        }
        html.append("</tr>\n");
    }

    chapters.clear();
    setHtml(html.append("</tbody></table></div></body>"));
}

void BibleTextCompareBrowser::addBrCoreToJS()
{
    this->page()->mainFrame()->addToJavaScriptWindowObject("brcore", brCore);
}
