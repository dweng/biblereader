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
 */
#ifndef BIBLETEXTBROWSER_H
#define BIBLETEXTBROWSER_H

#include <QTextBrowser>
#include <QMouseEvent>
#include <QColor>
#include <QTextCursor>
#include "biblereadercore.h"

class BibleTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    enum ShowMethod {
        NewChapter = 1,
        Zoom
    };

    explicit BibleTextBrowser(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleTextBrowser();

    QString getBibleVersion() const;
    void setBibleVersion(const QString &value);

public slots:
    bool showCurrentChapter(ShowMethod method = NewChapter);
    void printBibleText();

protected:
    bool event(QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:/* vars */
    int preVerseId;
    int preChapterId;
    int preBookId;

    BibleReaderCore *brCore;

    /* bible highlight text format */
    QColor bgColor;
    QColor fgColor;
    // bible text font and size
    QString btFontFamily;
    double btFontSize;

    QTextCursor *cursor;

    /**
     * @brief the bible version of this browser
     */
    QString bibleVersion;
private:/* functions */
    void highlight(int verse, const QColor &color);

    void addVerse(QTextCursor *cursor, BibleVerse verse, QFont f);
    QTextBlock getTextBlockByVerse(int verse);
    QString convertSNForDict(QString oldSN);

private slots:
    bool copyCurVerseHeader();
    bool copyCurVerse();
    bool copyCurVerseLongHeader();
    void projectVerse();
    void copyVerseTo();
    void changeSelectedVerseBGColor(QColor nc);
    void changeBibleTextFontFamily(QString family);
    void changeBibleTextFontSize(double size);

    // show strong number dict in dict win
    void navTo(QUrl brUrl);
    void searchBible();
    void copySelectedText();

    // save current bible chapter
    void saveCurrentChapter();
};

#endif // BIBLETEXTBROWSER_H
