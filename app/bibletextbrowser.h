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

#include <QTextEdit>
#include <QMouseEvent>
#include <QColor>
#include <QTextCursor>
#include "biblereadercore.h"

class BibleTextBrowser : public QTextEdit
{
    Q_OBJECT
public:
    explicit BibleTextBrowser(BibleReaderCore *brc, QWidget *parent = 0);
    ~BibleTextBrowser();

    QString getBibleVersion() const;
    void setBibleVersion(const QString &value);

public slots:
    bool showCurrentChapter();

protected:
    bool event(QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

    void contextMenuEvent(QContextMenuEvent *e);
private:/* vars */
    int preVerseId;

    BibleReaderCore *brCore;

    /* bible highlight text format */
    QColor bgColor;
    QColor fgColor;
    QFont font;

    QTextCursor *cursor;

    /**
     * @brief the bible version of this browser
     */
    QString bibleVersion;
private:/* functions */
    void highlight(int verse, const QColor &color);

    void addVerse(QTextCursor *cursor, QString verseText);
    QTextBlock getTextBlockByVerse(int verse);

private slots:
    bool copyCurVerse();
    void projectVerse();
    void copyVerseTo();
};

#endif // BIBLETEXTBROWSER_H
